#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>

class a{
public:
    ~a(){std::cout << 'a' << std::endl;}
    int a{10};

};

class b{
public:
    ~b(){std::cout << 'b' << std::endl;}
    int a{10};

};


class test{
public:
    a a1;
    b b1;
    ~test(){std::cout << "test" << std::endl;};
};

std::vector<int> ve(){
    return {1,23,3};
}
int main() {
    test a{};

    return 0;
}