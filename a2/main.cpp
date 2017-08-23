#include <cassert>
#include <list>
#include <sstream>
#include <vector>

#include "EuclideanVector.h"

int main() {
    evec::EuclideanVector v1 = {6, 8};
    std::cout<< v1.getEuclideanNorm();
}
