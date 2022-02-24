#include<iostream>

template<class T>
bool IsPositive( T const & t) {
	return T(0) <t;
}

struct X {
	int m_x;
	
	X(int i=0) : m_x(i){}
	
	bool operator<(const X &rhs)
	{
		return m_x < rhs.m_x;
	}
};

int main(int argc, char* argv[]) {
	X x(3);
	std::cout << IsPositive<X>(x);
	std::cout << IsPositive<int>(-3);
	return 0;
}
