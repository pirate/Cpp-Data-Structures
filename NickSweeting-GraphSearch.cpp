/*  Nick Sweeting 2014/03/28
    Graph Creator
    MIT License
*/

#include <stdlib.h>
#include <ctype.h>      // isdigit (parsing for tokens/ops)
#include <string>       // duh
#include <vector>       // vector of tokenized input
#include <iostream>     // cout
#include <sstream>      // istringstream, setw
#include <math.h>       // pow, sqrt
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

class Graph {
private:
    struct point {
        int x;
        int y;
    };

    vector<vector<int>> graph;
    vector<point>

public:
    Graph() {
    }
    ~Graph() {
    }

    void addPoint(x,y) {

    }
};
