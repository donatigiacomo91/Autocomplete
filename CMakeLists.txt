cmake_minimum_required(VERSION 3.3)
project(Autocomplete)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/Users/giacomo/Desktop/Code/Autocomplete/Bin")

set(SOURCE_FILES
        TST_P.h
        rmmq.hpp
        Top_K.h
        Autocomplete.h
        main.cpp)

set(SOURCE_FILES2
        Trie.h
        TST.h
        Test/catch.hpp
        Test/catch_conf.cpp
        Test/Trie_Test.cpp)

add_executable(Main ${SOURCE_FILES})
add_executable(BasicTrieTest ${SOURCE_FILES2})