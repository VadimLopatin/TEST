FROM python:3.8-slim-buster

RUN python -m pip install --upgrade pip

COPY requirements.txt run_train.py /

RUN apt-get update

RUN apt-get -y install g++

RUN apt-get -y install cmake

RUN apt-get -y install openmpi-bin

RUN pip install --no-cache-dir -r requirements.txt

CMD ["sh", "-c", "horovodrun -np 3 python run_train.py"]

