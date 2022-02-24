#include<iostream>
#include<memory>

class Kernel {
public:
	using Ptr = std::shared_ptr<Kernel>;
	void printLog(std::string msg) {
		std::cout << msg <<std::endl;
	}
	
	static Ptr get() {
		static Ptr kernel = Ptr(new Kernel);
		//std::cout<<kernel<<std::endl;
		return Ptr(kernel);
	}
};

class Backend {
public:
	Backend() {
		m_kernel = Kernel::get();
		m_kernel->printLog("Backend created succesfully");
	}

private:
	Kernel::Ptr m_kernel;

};

class Frontend {
public:
	Frontend() {
		m_kernel = Kernel::get();
		m_kernel->printLog("Frontend created succesfully");
	}

private:
	Kernel::Ptr m_kernel;
};

int main() {
	Backend b;
	Frontend f;
	return 0;
}
