#include<iostream>

struct A {
	A( char * pName) {
		std::cout << (pName ? pName : "A");
	}
	
	~A() {
		std::cout << "a";
	}
};

struct B : A {
	B( char * pName) : A(0) {
		std::cout << (pName ? pName : "B");
	}
	
	~B() {
		std::cout << "b";
	}
};

struct C {
	C( char * pName) {
		std::cout << (pName ? pName : "C");
	}
	
	~C() {
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
