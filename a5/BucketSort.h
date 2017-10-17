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
    void thread_sort(unsigned long offset, unsigned long vector_len);

public:
    // void split(std::size_t start, std::size_t end, std::vector<std::vector<unsigned int>>& , unsigned int numCores);

    std::pair<unsigned int, int> getMax();

    int get_total_digits(unsigned int);

    inline int get_digits_in_pos(unsigned int num, int pos, const int&);

    void thread_exec(unsigned long thread_id, unsigned long total_thread);

    inline void merge_all(int total_thread, unsigned long step);

};


#endif //A5_BUCKETSORT_H
