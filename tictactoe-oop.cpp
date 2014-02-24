/*  Nick Sweeting 2013/09/09
    Tic Tac Toe in C++ (non OOP)
    MIT Liscense
*/

#include <stdlib.h>
#include <iostream>
using namespace std;


enum square {EMPTY=' ', O='O', X='X'};


// h file

class Game {
    public:
        Stack(int sz); // Constructor: initialize variables, allocate space.
        void Push(int value); // Push an integer, checking for overflow.
        bool Full(); // Returns TRUE if the stack is full, FALSE otherwise.
    private:
        int size; // The maximum capacity of the stack.
        int top; // Index of the lowest unused position.
        int* stack; // A pointer to an array that holds the contents.
};

// c file

Stack::Stack(int sz) {
    size = sz;
    top = 0;
    stack = new int[size]; // Let's get an array of integers.
}

void test() {
    Stack s1(17);
    Stack* s2 = new Stack(23);
}

int main () {
    test();
    return 0;
}
