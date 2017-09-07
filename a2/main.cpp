#include <cassert>
#include <list>
#include <sstream>
#include <vector>

#include "EuclideanVector.h"

int a(evec::EuclideanVector a){
    std::cout << a.getNumDimensions();
    return 1;
}
int main() {
    evec::EuclideanVector v1 = {6, 8};
    a(v1);
}
