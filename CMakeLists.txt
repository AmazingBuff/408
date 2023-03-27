cmake_minimum_required(VERSION 3.24)
project(408 CPP)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(dataStructure)
include_directories(dataStructure/reference)
include_directories(dataStructure/textbook)

add_executable(408
        dataStructure/reference/graph.h
        dataStructure/reference/linkedList.h
        dataStructure/reference/list.h
        dataStructure/reference/matrix.h
        dataStructure/reference/queue.h
        dataStructure/reference/sequentialList.h
        dataStructure/reference/sort.h
        dataStructure/reference/stack.h
        dataStructure/reference/string.h
        dataStructure/reference/tree.h
        dataStructure/reference/vector.h
        dataStructure/textbook/BasicVariable.h)
