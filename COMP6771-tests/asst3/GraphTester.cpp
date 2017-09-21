#include <iostream>
#include "Graph.h"
#include "Fuzzer.h"
#include "ProvidedTests.h"

int main() {
    std::cout << "Starting test suite.\n";
    providedTests();
    fuzzer();

    return 0;
}
