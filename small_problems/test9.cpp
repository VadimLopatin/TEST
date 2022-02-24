#include<iostream>
#include<set>

int main(int argc, char * argv[]) {
	srand((unsigned)time(0));
	std::set<int> setVals;
	
	for(int i=0; i<10; ++i) {
		setVals.insert(rand());
	}
	
	for(auto a = setVals.begin(); a!=setVals.end();) {
		std::cout << *a <<std::endl;
		a = setVals.erase(a);
	}
	
	return 0;
	
}
