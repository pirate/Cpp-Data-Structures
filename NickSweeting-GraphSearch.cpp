/*  Nick Sweeting 2014/03/28
    Graph Creator
    MIT License
*/

#include <stdlib.h>
#include <string>
#include <math.h>       // pow, sqrt
#include <ctype.h>      // isdigit (parsing for tokens/ops)
#include <vector>       // vector of tokenized input
#include <queue>        // breadth-first search
#include <sstream>      // istringstream, setw
#include <iostream>     // cout
#include <fstream>      // file()
using namespace std;

/*

  1 2 3 4 5 6 7 8 9 ...
0|_________________
1|
2|
3|
4|
5|
6|
7|
8|
9|
...

*/

int stoint(string s) {
    int out = 0;
    istringstream(string(s)) >> out;
    return out;
}

string itostring(int i) {
    std::stringstream out;
    out << i;
    return out.str();
}

vector<int> tokenize(string input) {
    vector<int> tokens;
    size_t prev = 0, pos;
    // find next occurence of next delimiter after prev delimiter, then make a token using everything between those two positions
    while ((pos = input.find_first_of(delims, prev)) != string::npos) {
        if (pos > prev)
            tokens.push_back(stoint(input.substr(prev, pos-prev)));
        //tokens.push_back(stoint(input.substr(pos, 1)));             // this adds the delimiter as a token as well, useful for catching operators as tokens (e.g. +-()*/^)
        prev = pos+1;
    }
    // catch trailing token after the last delimiter
    if (prev < input.length())
        tokens.push_back(stoint(input.substr(prev, string::npos)));

    return tokens;
}

class Graph {
private:
    struct point {
        string label;
    };

    vector<vector<int>> graph;
    vector<point*>

    typedef struct node *nodep;             // nodepointer
    struct node nil;
    nodep nilp = &nil;

public:
    Graph() {
    }
    ~Graph() {
    }

    void addPoint(string label) {

    }

    void addEdge(point *point1, point *point2) {

    }
};



/*

Input point <"a">
Input point <"b">
Input point <"c">
Input point <"d">
Input point <"e">
Input point <"f">
Input point <"g">

Input edge a-b
Input edge b-c
Input edge c-d
Input edge d-e
Input edge a-f
Input edge f-e
Input edge b-g

Find shortest path a-e

===================

1. push to queue all a connections
a-b
a-f
2. search continues and more connections get pushed to queue
b-c
f-e
match found!
shortest route: a-f-e
conintuining search...
c-d
e-d



*/
