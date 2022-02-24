#include<iostream>
#include <cstdlib>

int f( int ** p) {
	int size = 10;
	(*p) = new int [ size ];
	return size;
}

int main (int argc, char * argv[])
{
	int * p = 0;
	int size = f( &p );
	
	std::cout<<p<<std::endl;
	
	std::cout<<size<<std::endl;
	
	for( int i=0; i < size; ++i) {
		p[i] = i;
	}
	
	delete [] p;
	return 0;
}
