//
// Created by yu on 16/10/17.
//

#include "BucketSort.h"

#include <algorithm>
#include <cmath>
#include <thread>
#include <iostream>

bool aLessB(unsigned int x, unsigned int y) {
    if (x == y) return false; // if the two numbers are the same then one is not less than the other

    unsigned int a = x;
    unsigned int b = y;

    bool flag = false;
    if(a < b){
        flag = true;
    }
    unsigned a_digits = 0, b_digits = 0;

    while(a > 9){
        a /= 10;
        a_digits += 1;
    }
    while(b > 9){
        b /=10;
        b_digits += 1;
    }

    // cast to avoid overflow
    auto x_l = static_cast<unsigned long long>(x);
    auto y_l = static_cast<unsigned long long>(y);
    if(a_digits > b_digits){
        y_l *= (std::pow(10, a_digits-b_digits));
    }else{
        x_l *= (std::pow(10, b_digits-a_digits));
    }

    if(x_l == y_l){
        return flag;
    }
    return x_l < y_l;
}

// sort element from start index to end index.
void BucketSort::thread_sort(size_t start, size_t end) {
    if(numbersToSort.size() < 2){
        return;
    }

    std::vector<std::vector<unsigned int>> radix_array;
    int max_digits = getMax().second;

    for(int pos=1; pos <= max_digits; ++pos){
        radix_array.resize(11, std::vector<unsigned int>());
        for(auto i=start; i < end; ++i ){
            int num = get_digits_in_pos(numbersToSort[i], pos, max_digits) + 1;
            radix_array[num].emplace_back(numbersToSort[i]);
        }

        auto j = 0;
        for(const auto& i: radix_array){
            for(const auto& k: i){
                numbersToSort[start+j] = k;
                ++j;
            }
        }
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
    if(pos < 1){
        return -1;
    }
    if(pos > 1){
        temp *= std::pow(10, pos-1);
    }
    return (num/temp) % 10;
}

void BucketSort::merge_all(int total_thread, size_t step) {
    std::vector<unsigned > res;

    // init index list. store the starting index of each sorted block.
    std::vector<size_t> index;
    for(auto i=0; i < total_thread; ++i){
        index.emplace_back( i * step );
    }

    unsigned smallest = 0;
    for(auto i=0U; i < numbersToSort.size(); ++i){
        int smallest_index = -1;

        for(auto j=0; j < total_thread; ++j){
            if(index[j] < (j + 1) * step || (j == total_thread-1 && index[j] < numbersToSort.size())){
                smallest = numbersToSort[index[j]];
                smallest_index = j;
                break;
            }
        }

        for(auto j=0; j < total_thread; ++j){
            if(index[j] < (j+1) * step || (j == total_thread-1 && index[j] < numbersToSort.size())){
                if(aLessB(numbersToSort[index[j]], smallest)){
                    smallest = numbersToSort[index[j]];
                    smallest_index = j;
                }
            }
        }
        if(smallest_index != -1){
            index[smallest_index] += 1;
        } else{
            throw("error index.\n");
        }

        res.emplace_back(smallest);
    }
    numbersToSort = std::move(res);

}

void BucketSort::sort(int CoreNum) {
    std::vector<std::thread> vec_thr;

    auto step = static_cast<size_t>(std::floor(numbersToSort.size() / CoreNum ));
    size_t start = 0;

    for(auto i = 0; i < CoreNum; ++i){
        if(i == CoreNum-1){
            vec_thr.emplace_back(&BucketSort::thread_sort, this, start, numbersToSort.size());
        }else{
            vec_thr.emplace_back(&BucketSort::thread_sort, this, start, start + step);
        }
        start += step;
    }


    for(auto& i: vec_thr){
        i.join();
    }

    merge_all(CoreNum, step);
}
