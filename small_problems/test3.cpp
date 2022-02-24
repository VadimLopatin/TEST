#include<iostream>

struct A {
	A( char * pName) {
		std::cout << (pName ? pName : "A");
	}
	
	virtual ~A() {
		std::cout << "a";
	}
};

struct B : A {
	B( char * pName) : A(0) {
		std::cout << (pName ? pName : "B");
	}
	
	virtual ~B() {
		std::cout << "b";
	}
};

struct C {
	C( char * pName) {
		std::cout << (pName ? pName : "C");
	}
	
	virtual ~C() {
		std::cout << "c";
	}
};

struct E {
	E() : b(0), c(0), a(0) {}
	
	A a;
	B b;
	C c;
};

int main (int argc, char * argv[]) {
	E e;
	return 0;
}
