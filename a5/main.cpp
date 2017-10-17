#include <iostream>
#include <random>
#include <thread>

#include "BucketSort.h"

int main() {

    unsigned int totalNumbers = 1050000;
    unsigned int printIndex =	5;


    // use totalNumbers required as the seed for the random
    // number generator.
    std::mt19937 mt(totalNumbers);
    std::uniform_int_distribution<unsigned int> dist(1, std::numeric_limits<unsigned int>::max());

    // create a sort object
    BucketSort pbs;

    // insert random numbers into the sort object
    for (unsigned int i=0; i < totalNumbers; ++i) {
        pbs.numbersToSort.push_back(dist(mt));
    }

    // call sort giving the number of cores available.
    const unsigned int numCores = std::thread::hardware_concurrency();
    pbs.sort(numCores);

    std::cout << "number of cores used: " << numCores << std::endl;

    // print certain values from the buckets
    std::cout << "Demonstrating that all the numbers that start with 1 come first" << std::endl;

//    for(auto &i: pbs.numbersToSort){
//        std::cout << i << std::endl;
//    }
    std::cout << pbs.numbersToSort.size();
//    std::cout << pbs.numbersToSort.size();
//    std::cout << pbs.numbersToSort[0] << " " << pbs.numbersToSort[printIndex/5]
//              << " " << pbs.numbersToSort[printIndex/2] << " " << pbs.numbersToSort[pbs.numbersToSort.size() - 1]
//              << std::endl;

}