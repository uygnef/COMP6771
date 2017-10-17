//
// Created by yu on 16/10/17.
//

#ifndef A5_BUCKETSORT_H
#define A5_BUCKETSORT_H
#include <vector>


class BucketSort {
    // vector of numbers
public:
    std::vector<unsigned int> numbersToSort;

    void sort(int CoreNum);

public:
    void thread_sort(unsigned long offset, unsigned long vector_len);

    inline std::pair<unsigned int, int> getMax();

    inline int get_total_digits(unsigned int);

    inline int get_digits_in_pos(unsigned int num, int pos, const int&);

    inline void merge_all(int total_thread, unsigned long step);

};


#endif //A5_BUCKETSORT_H
