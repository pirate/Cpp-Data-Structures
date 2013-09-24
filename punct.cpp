#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

bool ispunct(char q) {
    char punct[26] = ".!-_? <>,*&^$!;:'()[]{}|`";
    int p;
    for (p=0;p<25;p++) {
        if (q == punct[p]) {
            return true;
        }
    }
    return false;
}

int main() {
    if (ispunct(' ')) {
        cout << "its punct" << endl;
    }
    else {
        cout << "not punct." << endl;
    }
}
