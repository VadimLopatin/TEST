#include<iostream>

struct X {
	int GetX() const{
		return m_x;
	}
	
	int m_x;
};

int main (int argc, char * argv[]) {
	static X x;
	std::cout << x.GetX();
	
	return 0;
}
