/*  Nick Sweeting 2013/10/09
    References vs Values in C++
    MIT License

    Takes input and removes puctuation and spaces, using two different methods.
*/

#include <stdlib.h>
#include <iostream>
using namespace std;

// returns true if input character q is puctuation, else false
bool ispunctuation(char q) {
    char punct[31] = ".!-_? <>,$#@=+*&^$!;:'()[]{}|`";
    int p;
    for (p=0;p<30;p++) if (q == punct[p]) return true;
    return false;
}

char* modifyAndCopy(char *raw_input) {
    // input cleanup
    char* newarray = new char[80];
    int q, position = 0;
    for (q=0;q<80;q++) {
        if (ispunctuation(raw_input[q])) true;
        else {
            newarray[position] = raw_input[q];
            position++;
        }
    }
    return newarray;
}

char* modifyInPlace(char *raw_input){
    // input cleanup
    int q, position = 0;
    for (q=0;q<80;q++) {
        if (ispunctuation(raw_input[q])) true;
        else {
            raw_input[position] = raw_input[q];
            position++;
        }
    }
    return raw_input;
}

int main() {
    // user input
    char raw_input[80] = {0};
    cout << "Please input something with punctuation in it: ";
    cin.getline(raw_input,80);

    cout << "Modify and Copy: " << endl;
    cout << "Original: " << raw_input << endl;
    cout << "Modified: " << modifyAndCopy(raw_input) << endl << endl;

    cout << "Modify in Place: " << endl;
    cout << "Original: " << raw_input << endl;
    cout << "Modified: " << modifyInPlace(raw_input) << endl;
}
