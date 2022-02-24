#include<iostream>

struct T1 {
	virtual int f1() const {
		return 1;
	}
	
	int f2() const {
		return 1;
	}
};

struct T2 : T1 {
	virtual int f1() const {
		return 2;
	}
	
	int f2() const {
		return 2;
	}
};

void f(T1 * p) {
	std::cout<< p->f1() << " " << p->f2() << "\n";
}

int main(int argc, char * argv[] ) {
	T2 t2;
	f( &t2 );
	return 0;
}
