#include <iostream>
#include "Graph.h"
#include "Fuzzer.h"
#include "ProvidedTests.h"

int main() {
    std::cout << "Starting test suite.\n";

    printf("Starting provided tests\n");
    providedTests();
    printf("Passed provided tests\n");

    fuzzer();

    return 0;
}
