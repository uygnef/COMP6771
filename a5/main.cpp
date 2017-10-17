#include <algorithm>
#include <chrono>
#include <iostream>
#include <future>
#include <random>
#include <string>
#include <thread>

#include "BucketSort.h"

int main() {
    static constexpr const size_t numbers = 500000;
    static constexpr const size_t runs = 500;
    std::cout << "init number\n";

    std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<unsigned int> numberDist;
    std::uniform_int_distribution<unsigned int> coreDist(1, std::thread::hardware_concurrency() * 10);

    BucketSort bs;

    for (size_t r = 0; r < runs; ++r){
        bs.sort(coreDist(mt));
    }

    bs.numbersToSort.resize(numbers);

    for (size_t r = 0; r < runs; ++r) {
        for (size_t n = 0; n < numbers; ++n)
            bs.numbersToSort[n] = numberDist(mt);

        std::vector<std::string> ref(numbers);
        std::transform(bs.numbersToSort.cbegin(), bs.numbersToSort.cend(), ref.begin(), static_cast<std::string (*)(unsigned int)>(std::to_string));
        auto refFuture = std::async([&]() {std::sort(ref.begin(), ref.end());});

        bs.sort(coreDist(mt));

        std::vector<std::string> result(numbers);
        std::transform(bs.numbersToSort.cbegin(), bs.numbersToSort.cend(), result.begin(), static_cast<std::string (*)(unsigned int)>(std::to_string));

        refFuture.get();
        for (size_t n = 0; n < numbers; ++n) {
            if (ref[n] != result[n]) {
                std::cerr << "Mismatch at index " << n << ": " << ref[n] << " != " << result[n] << "\n";
                for( auto s=n-10; s < n+10; ++s){
                    std::cout << ref[s] << " vs " << result[s] << std::endl;
                }
                return 1;
            }
        }

        std::cerr << ".";
    }

    return 0;
}
