#include <iostream>
#include <fstream>

namespace cs6771 {
	void order3(unsigned int& a, unsigned int& b, unsigned int& c) {
		if (a > c) {
			int t = a;
			a = c;
			c = t;
		}
		if (b > c) {
			int t = b;
			b = c;
			c = t;
		}
		if (a > b) {
			int t = a;
			a = b;
			b = t;
		}
	}
}

int main(int argc, char* argv[]) {
    unsigned int a, b, c;
    std::ifstream out;
    out.open(argv[1]);
    
    out>>a>>b>>c;
  
    cs6771::order3(a,b,c);
    std::cout<<a<<" "<<b<<" "<<c<<std::endl;
    return 0;
}
