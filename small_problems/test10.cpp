#include<cstddef>
#include<vector>
#include<map>
#include<iostream>

struct Type {
	Type(const int defX={}, const int defY={}, const size_t n=0) {
		fill(defX, defY, n);
	}
	
	void fill(const int defX, const int defY, const size_t n) {
		x.assign(n, defX);
		y.assign(n, defY);
	}
	
	std::vector<int> x;
	std::vector<int> y;
};

int main() {
	std::map<double, Type> points;
	
	Type * r = &points[0.1];
	r->fill(1,1,10000);
	
	r = &points[0.2];
	r->fill(2,2,10000);
	
	r = &points[0.3];
	r->fill(3,3,10000);
	
	
	for(auto [key, value] : points)
	{
		std::cout<<key<<" "<<value.x.size()<<std::endl;
	}
	return 0;
}
