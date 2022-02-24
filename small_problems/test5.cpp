#include<algorithm>
#include<vector>
#include<iostream>

size_t countValues(const std::vector<int>& arr, int min, int max)
{
	return std::count_if(arr.begin(), arr.end(), [&min, &max](int v){return (min <= v) && (v <= max);});
}

int main()
{
	std::vector<int> Z = {1,2,3,4,5,5,4,3,2,43,4,5};
	std::cout << countValues(Z, 2, 5) <<std::endl;
	return 0;	
}

