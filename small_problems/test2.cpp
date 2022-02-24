#include <iostream>

struct A {
	~A() {
		std::cout << "~A";
	};
};

struct B: public A {
	virtual ~B() {
		std::cout << "~B";
	};
};

int main() {
	A *a = new B();
	delete a;
	return 0;
}
