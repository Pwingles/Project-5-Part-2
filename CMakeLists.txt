cmake_minimum_required(VERSION 3.10)
project(Project5)

set(CMAKE_CXX_STANDARD 17)

# Harness
add_executable(HashTablesOpenAddressing main.cpp HashTableDictionary.cpp)

# Trace Generator
add_executable(LRUTraceGenerator LRUTraceGenerator.cpp)
