/*  Nick Sweeting 2013/11
    Punctuation Detector
    MIT License
    g++ Punctuation.cpp -o main; and ./main

    Detect if user's input is punctuation or not.
*/

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

const char punct[26] = " .!-_?<>,*&^$!;:'()[]{}|`";

bool ispunct(char q) {
    for (int p=0;p<25;p++) {
        if (q == punct[p]) return true;
    }
    return false;
}

int main() {
    if (ispunct(' '))
        cout << "its punct" << endl;
    else
        cout << "not punct." << endl;
}
