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
void BucketSort::thread_sort(size_t start, size_t end) {
    std::vector<std::vector<unsigned int>> radix_array;
    int max_digits = getMax().second;

    for(int pos=1; pos <= max_digits; ++pos){
        radix_array.resize(11, std::vector<unsigned int>());
        for(auto i=start; i < end; ++i ){
            int num = get_digits_in_pos(numbersToSort[i], pos, max_digits) + 1;
//            std::cout << "num is :" << num << std::endl;
            radix_array[num].emplace_back(numbersToSort[i]);
        }

        auto j = 0;
        for(const auto& i: radix_array){
//            std::cout << "-----" << std::endl;
            for(const auto& k: i){
//                std::cout << "i: " << k << std::endl;
                numbersToSort[start+j] = k;
                ++j;
            }
        }
        radix_array.clear();
//        std::cout << "$$$$$" << std::endl;
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


int BucketSort::get_total_digits(unsigned int num) {
    int digits = 1;
    while(num >= 10){
        num /= 10;
        ++digits;
    }
    return digits;
}

int BucketSort::get_digits_in_pos(unsigned int num, int pos, const int& max_pos) {
    int temp = 1;
    pos = pos - (max_pos - get_total_digits(num));
//    std::cout << "pos is: " << pos << std::endl;
    if(pos < 1){
        return -1;
    }
    for(auto i=1; i < pos; ++i){
        temp *= 10;
    }
    return (num/temp) % 10;
}

void BucketSort::thread_exec(size_t start, size_t end) {

    if(end == -1){
        end = numbersToSort.size();
    }

    // last thread sort all remaining data.

    thread_sort(start, end);
}

void BucketSort::merge_all(int total_thread, size_t step) {
    std::vector<unsigned > res;

    // init index list. store the starting index of each sorted block.
    std::vector<size_t> index;
    for(auto i=0; i < total_thread; ++i){
        index.emplace_back( i * step );
    }

    unsigned smallest = 0;
    for(auto i=0; i < numbersToSort.size(); ++i){
        int smallest_index = -1;

//        std::cout << "--index--:\n";
//        for(auto& d: index){
//            std::cout << d << std::endl;
//        }
        // get the first available element
        for(auto j=0; j < total_thread; ++j){
            if(index[j] < (j + 1) * step || (j == total_thread-1 && index[j] < numbersToSort.size())){
                smallest = numbersToSort[index[j]];
                smallest_index = j;
                break;
            }
        }
//        std::cout << "-1-  smallest : " << smallest << " index: " << smallest_index << "\n";

        for(auto j=0; j < total_thread; ++j){

            if(index[j] < (j+1) * step || (j == total_thread-1 && index[j] < numbersToSort.size())){
//                std::cout << "true \n";
//                std::cout << "^^^^^^^" << numbersToSort[index[j]] << " " << smallest <<  std::endl;
//                std::cout << "^^^^^^^" << index[j] << " " << j << std::endl;
                if(aLessB(numbersToSort[index[j]], smallest, 0)){
         //           std::cout << "-2- true \n";

                    smallest = numbersToSort[index[j]];
                    smallest_index = j;
                }
            }
        }
//        std::cout << "-2-  smallest : " << smallest << " index: " << smallest_index << "\n";

        if(smallest_index != -1){
//            std::cout << "emplace num: " << smallest << " " << smallest_index << " " << index[smallest_index] << '\n';
            index[smallest_index] += 1;
        } else{
//            std::cout << "nala\n";
            throw("error index.\n");
        }

        res.emplace_back(smallest);
    }
    numbersToSort = std::move(res);
}

void BucketSort::sort(int CoreNum) {
    std::vector<std::thread> vec_thr;

    size_t step = static_cast<size_t>(std::floor(numbersToSort.size() / CoreNum ));
    size_t start = 0;
    std::cout << "start sort\n";
    for(auto i = 0; i < CoreNum; ++i){
        if(i == CoreNum-1){
            vec_thr.emplace_back(&BucketSort::thread_exec, this, start, -1);
        }else{
            vec_thr.emplace_back(&BucketSort::thread_exec, this, start, start + step);
        }
        start += step;
    }

    for(auto& i: vec_thr){
        i.join();
    }


    merge_all(CoreNum, step);

}
