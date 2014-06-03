/*  Nick Sweeting 2014/05/12
    Hash Map
    MIT License
*/

#include <stdlib.h>
#include <memory.h>
#include <time.h>       // rand seed
#include <string>       // duh
#include <vector>       // vector of tokenized input
#include <queue>        // breadth-first search
#include <iostream>     // cout
#include <fstream>      // file()
using namespace std;
const string alphabet[26] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};    // for generating random words

static const int magic = 0xF423F;

vector<string> tokenize(string input, string delims=",; -") {
    vector<string> tokens;
    size_t prev = 0, pos;
    // find next occurence of next delimiter after prev delimiter, then make a token using everything between those two positions
    while ((pos = input.find_first_of(delims, prev)) != string::npos) {
        if (pos > prev)
            tokens.push_back(input.substr(prev, pos-prev));
        //tokens.push_back(input.substr(pos, 1));             // this adds the delimiter as a token as well, useful for catching operators as tokens (e.g. +-()*/^)
        prev = pos+1;
    }
    // catch trailing token after the last delimiter
    if (prev < input.length())
        tokens.push_back(input.substr(prev, string::npos));

    return tokens;
}

class StudentHashTable {
private:
    struct Student {
        char firstName[15];
        char lastName[15];
        double GPA;
    };

    static const int magic = 0xF423F;
    Student* hashtable[magic];
    int count;

    int hash(int key) {
        //hashtable_hash_fnv(key, magic);
        return key;
    }

    int hashtable_hash_fnv(void *key, int size){
        unsigned int hval = 0x811c9dc5;
        unsigned int hval2 = magic;
        unsigned char *bp = (unsigned char *)key;
        unsigned char *be = bp + size;

        while (bp < be) {
            hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
            hval ^= (unsigned int)*bp++;
        }

        return (int)hval;
    }    

public:
    StudentHashTable() {
        count = 0;
    };

    ~StudentHashTable() {
        empty();
    };

    Student* operator[](const int key) throw (const char *) {
        if (hashtable[hash(key)]->firstName)
            return hashtable[hash(key)];
        else
            return NULL;
    }

    void add(int key) {
        hashtable[key] = new Student;
        count++;
    }

    void remove(int key) {
        delete hashtable[hash(key)];
        count--;
    }

    void clear() {
        for (int i=0; i<magic;i++) {
            if (hashtable[i]->firstName)
                remove(i);
        }
        if (count != 0)
            cout << "WARNING: INCONSISTENCY DETECTED. count off by " << count << endl;
    }

    void empty() {
        clear();
    }

    int size() {
        return count;
    }

    void print() {
        for (int i=0; i<magic;i++) {
            if ((*hashtable[i]).firstName)
                cout << i << ": " << (*hashtable[i]).firstName << " " << (*hashtable[i]).lastName << " - " << (*hashtable[i]).GPA << endl;
        }   
    }
};


int main(int argc, char **argv) {




    StudentHashTable students;
    students.print();
    students.add(458209);
    students.print();
    students.add(458211);
    students.print();
    students.remove(458209);
    students.print();
}
