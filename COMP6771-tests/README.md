# COMP6771 tests
Publically shared tests for UNSW's Advanced C++ Programming course.
Feel free to make a pull request adding more tests.

This repository will become private and only include people who have contributed
tests in assignment 1 or who specifically ask me to be added later in the semester
so as to encourage contributions by everyone. :)

Note that duplicate test-cases are welcome since I realize not everyone necessarily wants
to comb through all the already existing test-cases to see if their added tests do not duplicate
them.

Assignment 1
============
To contribute please add a file titled `in-NAME` and `out-NAME` (where
`NAME` is an arbitrary alphanumeric string) into the asst1 directory. You can
also optionally specify your own custom dictionary of words by adding
`dict-NAME`.

The `in-NAME` file should simply contain stdin while `out-NAME` contains the valid
word ladders seperated by newlines and formatted according to the spec. (`test.sh` worries
about adding the input prompts).

To run all tests simply run `test.sh EXECUTABLE_PATH` from within the asst1 dir.

Assignment 2
============
To contribute please add a seperate .cc file and .h file to the asst2 directory. Then edit
`EuclideanVectorTester.cpp` to include your test entrypoint and `#include` your newly
defined header file.

To run all tests simply run `test.sh CODE_DIR` from within the asst2 dir, where `CODE_DIR`
is the directory that contains your `EuclideanVector.h` and `EuclideanVector.cc` files.
Note that we assume you only need to test these two files.

Assignment 3
============
To contribute please add a seperate .cc file and .h file to the asst3 directory. Then edit
`GraphTester.cpp` to include your test entrypoint and `#include` your newly defined header file.

To run all tests simply run `test.sh CODE_DIR` from within the asst3 dir, where `CODE_DIR`
is the directory that contains your `Graph.h` and `Graph.tem` files.
Note that we assume you only need to test these two files.

Assignment 4
============
Coming soon.
Assignment 5
============
Coming soon.

