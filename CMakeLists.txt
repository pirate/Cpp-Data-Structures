cmake_minimum_required(VERSION 2.8.4)
project(C__DataStructures)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    "hashtable example C code/hashtable.c"
    "hashtable example C code/hashtable.h"
    "hashtable example C code/main.c"
    "zuul-game/Nick Sweeting-Zuul/Room.cpp"
    "zuul-game/Nick Sweeting-Zuul/zuul.cpp"
    zuul-game/Item.cpp
    "zuul-game/linked_ old.cpp"
    zuul-game/Player.cpp
    zuul-game/zuul2.cpp
    crazyier.cpp
    crazyshit.cpp
    infix.cpp
    inlinea.c
    inlinea.cpp
    linked.cpp
    NickSweeting-GraphSearch.cpp
    NickSweeting-HashTable.cpp
    NickSweeting-Heap.cpp
    NickSweeting-LinkedList.cpp
    NickSweeting-RedBlack.cpp
    Nicksweeting-TicTacToe.cpp
    NickSweetingInfixv2.cpp
    odd.cpp
    palindrome.cpp
    punct.cpp
    refandval.cpp
    runtime_assembly_exec.cpp
    runtime_func_exec.cpp
    sample-redblack.cpp
    studentlist.cpp
    test.cpp
    tictactoe-oop.cpp
    tictactoe.cpp)

add_executable(C__DataStructures ${SOURCE_FILES})