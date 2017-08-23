#!/bin/bash

if [[ $# -ne 1 ]]
then
    echo "Usage: test.sh CODE_DIR"
    exit
fi

# Copy EuclideanVector code into this dir.
CODE_DIR="$1"
cp "${CODE_DIR}Graph.tem" .
cp "${CODE_DIR}Graph.h" .

# Compile our test-suite..
make

# remove the original files. We wouldn't want someone accidentally
# committing their solutions to this repo now, would we
rm -f Graph.tem
rm -f Graph.h

# Run the tests.
./GraphTester

