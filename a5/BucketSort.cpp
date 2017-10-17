//
// Created by yu on 16/10/17.
//

#include "BucketSort.h"

#include <algorithm>
#include <cmath>
#include <thread>
#include <iostream>

bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {

    if (x == y) return false; // if the two numbers are the same then one is not less than the other

    unsigned int a = x;
    unsigned int b = y;

    // work out the digit we are currently comparing on.
    if (pow == 0) {
        while (a / 10 > 0) {
            a = a / 10;
        }
        while (b / 10 > 0) {
            b = b / 10;
        }
    } else {
        while (a / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
            a = a / 10;
        }
        while (b / 10 >= (unsigned int) std::round(std::pow(10,pow))) {
            b = b / 10;
        }
    }

    if (a == b)
        return aLessB(x,y,pow + 1);  // recurse if this digit is the same
    else
        return a < b;
}

// TODO: replace this with a parallel version.
void BucketSort::thread_sort(unsigned int offset, unsigned vector_len) {
    std::vector<std::vector<unsigned int>> radix_array;
    int max_digits = getMax().second;

    std::cout << "max_digits: " << max_digits << std::endl;
    for(auto i: numbersToSort){
        std::cout << i << std::endl;
        std::cout << get_digits_in_pos(i, 10, max_digits) << std::endl;
    }

    for(int pos=1; pos <= max_digits; ++pos){
        radix_array.resize(11, std::vector<unsigned int>());
        for(auto i=0; i < vector_len; ++i ){
            int num = get_digits_in_pos(numbersToSort[i+offset], pos, max_digits) + 1;
            std::cout << "num is :" << num << std::endl;
            radix_array[num].emplace_back(numbersToSort[i+offset]);
        }
        auto index = numbersToSort.size() - 1;
        for(const auto& i: radix_array){
            std::cout << "-----" << std::endl;
            for(const auto& k: i){
                std::cout << "i: " << k << std::endl;
                numbersToSort[index + offset] = k;
                --index;
            }
        }
        std::cout << "$$$$$\n";
        radix_array.clear();
    }

}


std::pair<unsigned int, int> BucketSort::getMax() {
    auto max_num = *std::max_element(numbersToSort.begin(), numbersToSort.end());
    auto temp = max_num;
    int digits = 1;
    while(temp >= 10){
        temp /= 10;
        ++digits;
    }
    return std::make_pair(max_num, digits);
}


int BucketSort::get_digits(unsigned int num) {
    int digits = 1;
    while(num >= 10){
        num /= 10;
        ++digits;
    }
//    std::cout << "digits is: " << digits << std::endl;
    return digits;
}

int BucketSort::get_digits_in_pos(unsigned int num, int pos, const int& max_pos) {
    int temp = 1;
    pos = pos - (max_pos - get_digits(num));
    std::cout << "pos is: " << pos << std::endl;
    if(pos < 1){
        return -1;
    }
    for(auto i=1; i < pos; ++i){
        temp *= 10;
    }
    return (num/temp) % 10;
}
