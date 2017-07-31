#include <iostream>
#include <fstream>

namespace cs6771 {
	void order3(int *a, int *b, int *c) {
		if (*a < *c) {
			int t = *a;
			*a = *c;
			*c = t;
		}
		if (*a < *b) {
			int t = *a;
			*a = *b;
			*b = t;
		}
		if (*b < *c) {
			int t = *b;
			*b = *c;
			*c = t;
		}
	}
}

int main(int argc, char* argv[]) {
    int a, b, c;
    std::ofstream out;
    out.open(argv[1]);
    
    std::cin >>a>>b>>c;
  
    cs6771::order3(&a,&b,&c);
    std::cout<<a<<" "<<b<<" "<<c<<std::endl;
    out << a <<" "<<b<<" "<<c<<std::endl;
    return 0;
}
