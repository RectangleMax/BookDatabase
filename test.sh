#!/bin/bash
# g++ ./src/main.cpp -std=c++23 -lboost_container 
g++ ./tests/main.cpp ./tests/db_test.cpp -lgtest -std=c++23 -lboost_container -o test_me 