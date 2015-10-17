/*  Nick Sweeting 2013/09/09
    Palindrome Detector C++
    MIT License
    g++ Palindrome.cpp -o main; and ./main

    Takes input and removes puctuation and spaces, then checks to see if it's a palindrome or not.
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

// returns int: # of characters in the input array
int findlength(char input[]) {
    int i, length=0;
    for (i=0;i<80;i++) {
        if (input[i] != '\0') length++;
        else break;
    }
    //cout << "Length: " << length << endl;                                     // uncomment cout lines like this one to get more verbose output
    return length;
}

// returns int: # of characters in the same position forwards & backwards
int findequalcount(char input[], int length) {
    int equalcount = 0;
    int z;
    for (z=0;z<length;z++) {
        if (input[length-z-1] == input[z]) {
            equalcount++;
            //cout << "A: " << input[length-z-1] << " B: " << input[z] << endl;
        }
        else {
            //cout << "A: " << input[length-z-1] << " B: " << input[z] << endl;
        }
    }
    //cout << "Number of letters in the same position backwards & forwards: " << equalcount << endl;
    return equalcount;
}

int main() {
    // user input
    char raw_input[80] = {0};
    cout << "Please attempt a palindrome: ";
    cin.getline(raw_input,80);

    // input cleanup
    char input[80] = {0};
    int q, position = 0;
    for (q=0;q<80;q++) {
        if (ispunctuation(raw_input[q])) true;
        else {
            input[position] = raw_input[q];
            position++;
        }
    }
    //cout << "Forwards: " << input << endl;

    int length = findlength(input);
    int equalcount = findequalcount(input, length);

    // if length = number of character in the same position forwards and backwards, its a palindrome
    if (equalcount == length) {
        cout << "\033[1;38mIt's a palindrome!\033[0m" << endl;          // ansi escape sequence for a splash of color (green if true, red if false)
        return 0;
    }
    else {
        cout << "\033[1;31mIt's not palindrome.\033[0m" << endl;
        return 1;
    }
}
