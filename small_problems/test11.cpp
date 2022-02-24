#include<vector>
#include<iostream>

void sort12(std::vector<int> &arr)
{
	int num1=0, num2=0;
	for(auto el : arr) el==1 ? ++num1 : ++num2;
	for(int i=0; i<num1; ++i) arr[i]=1;
	for(int i=num1; i<(num1+num2); ++i) arr[i]=2;
}

int main()
{
	std::vector<int> Z = {2,1,1,2,2,1, 1};
	sort12(Z);
	
	for(auto el : Z) std::cout<<el<<" ";
	std::cout<<std::endl;
	return 0;
}
