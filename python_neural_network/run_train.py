import os
import numpy as np
import pandas as pd 


import torch
from torch import nn, optim
import torch.nn.functional as F
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms, utils, datasets

import horovod.torch as hvd



def get_cifar10_datalset():

    transform = transforms.Compose(
        [transforms.ToTensor(),
         transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])
    
    trainset = datasets.CIFAR10(
        root='./data',
        train=True,
        download=True,
        transform=transform
        )
    
    testset = datasets.CIFAR10(
        root='./data',
        train=False,
        download=True,
        transform=transform
        )
    
    return trainset, testset



def get_data_loaders(trainset, testset, batch_size, num_replicas, rank):
    
    train_sampler = torch.utils.data.distributed.DistributedSampler(
        trainset, num_replicas=num_replicas, rank=rank)
    
    test_sampler = torch.utils.data.distributed.DistributedSampler(
        testset, num_replicas=num_replicas, rank=rank)
    
    trainloader = torch.utils.data.DataLoader(
        trainset,
        batch_size=batch_size,
        sampler=train_sampler,
        )
    
    testloader = torch.utils.data.DataLoader(
        testset,
        batch_size=batch_size,
        sampler=test_sampler,
        )
    
    return trainloader, testloader




def fit(epochs, model, loss_func, opt, train_dl, valid_dl):
    train_losses = []
    val_losses = []
    valid_accuracies = []
    for epoch in range(epochs):
        model.train()
        loss_sum = 0
        for xb, yb in train_dl:
            # xb, yb = xb.to(device), yb.to(device)
            probs = model(xb)
            loss = loss_func(probs, yb)
            loss_sum += loss.item()
            
            loss.backward()
            opt.step()
            opt.zero_grad()
        train_losses.append(loss_sum / len(train_dl))

        model.eval()
        loss_sum = 0
        correct = 0
        num = 0
        with torch.no_grad():
            for xb, yb in valid_dl:
                # xb, yb = xb.to(device), yb.to(device)
                probs = model(xb)
                loss_sum += loss_func(probs, yb).item()
                
                _, preds = torch.max(probs, axis=-1)
                correct += (preds == yb).sum().item()
                num += len(xb)
                
        val_losses.append(loss_sum / len(valid_dl))
        valid_accuracies.append(correct / num)
        
    return train_losses, val_losses, valid_accuracies


class Model(nn.Module):
    def __init__(self):
        super(Model, self).__init__()
        # 1 input image channel, 6 output channels, 3x3 square conv kernel
        self.conv1 = nn.Conv2d(3, 6, 3)
        self.conv2 = nn.Conv2d(6, 16, 3)
        self.fc1 = nn.Linear(16 * 6 * 6, 120)  # 5x5 image dimension
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        x = F.max_pool2d(F.relu(self.conv1(x)), (2, 2))
        x = F.max_pool2d(F.relu(self.conv2(x)), 2)
        x = x.view(x.shape[0], -1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x


hvd.init()
hvd.size()


train, test = get_cifar10_datalset()


train_loader, test_loader = get_data_loaders(
    trainset=train,
    testset=test,
    batch_size=100,
    num_replicas=hvd.size(),
    rank=hvd.rank()
)

simple_model = Model()
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(simple_model.parameters())


dist_optimizer = hvd.DistributedOptimizer(optimizer, named_parameters=simple_model.named_parameters())
hvd.broadcast_parameters(simple_model.state_dict(), root_rank=0)

print('Run train')

training_statistics = fit(
    epochs=10,
     model=simple_model,
     loss_func=criterion,
     opt=dist_optimizer,
     train_dl=train_loader,
     valid_dl=test_loader
     )



for epoch, (train_loss, test_loss, test_accuracy) in enumerate(zip(*training_statistics)):
    print(f'Epoch: {epoch},Train Loss: {train_loss:.3f}, Test Loss: {test_loss:.3f}, Test Accuracy: {test_accuracy:.3f}')




