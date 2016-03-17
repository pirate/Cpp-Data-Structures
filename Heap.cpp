/*  Nick Sweeting 2014/02/13
    Heap
    MIT License
    g++ Heap.cpp -o main; and ./main

    A heap implemented as a flat array using index math to access elements.
*/

#include <stdlib.h>
#include <ctype.h>      // isdigit (parsing for tokens/ops)
#include <string>       // duh
#include <vector>       // vector of tokenized input
#include <iostream>     // cout
#include <sstream>      // istringstream, setw
#include <math.h>       // pow, sqrt
using namespace std;

const string delims = ",; ";

int stoint(string input) {
    int i = 0;
    istringstream(string(input)) >> i;
    return i;
}

string itostring(int i) {
    std::stringstream out;
    out << i;
    return out.str();
}

vector<int> tokenize(string input) {
    vector<int> tokens;
    size_t prev = 0, pos;
    // find next occurence of delimiter after prev delimiter, then append everything between those two positions
    while ((pos = input.find_first_of(delims, prev)) != string::npos) {
        if (pos > prev)
            tokens.push_back(stoint(input.substr(prev, pos-prev)));
        //tokens.push_back(stoint(input.substr(pos, 1)));                   // this adds the delimiter as a token as well, useful for catching operators as tokens (e.g. +-()*/^)
        prev = pos+1;
    }
    // catch trailing info after the last delimiter
    if (prev < input.length())
        tokens.push_back(stoint(input.substr(prev, string::npos)));

    return tokens;
}

// Heap implemented with a flat array and index math (implicit data structure)
class FHeap {
    // Math for flat heap indexes:
    //  Left = (index*2)+1
    //  Right = (index+1)*2
    //  Parent = floor((index-1)/2)
    //  Depth = floor(√(index)+0.5)  (0 indexed depth where the root is at depth=0, next level down is depth=1, ...)

    private:
        int heap[80];
        int heapsize;

        void sortbyparent(int index) {
            // recursive function that checks if(parent>current) {swap parent with current; then recurse with parent}
            if (index !=0) {
                int node = heap[index];
                int parent = floor((index-1)/2);
                //cout << "parent: " << heap[parent] << " node: " << node;
                if (node > heap[parent]) {
                    //cout << " SWAPPING";
                    heap[index] = heap[parent];
                    heap[parent] = node;
                }
                //cout << endl;
                sortbyparent(parent);
            }
        }

        void sortbychildren(int index) {
            // recursive function that checks if(left or right < current) {swap left or right with current; then recurse with left or right}
            int node = heap[index];
            int left = (index*2)+1;
            int right = (index+1)*2;
            //cout << "parent: " << heap[parent] << " node: " << node;
            if (node < heap[left] || node < heap[right]) {
                if (heap[left] > heap[right]) {
                    heap[index] = heap[left];
                    heap[left] = node;
                    sortbychildren(left);
                }
                else {
                    heap[index] = heap[right];
                    heap[right] = node;
                    sortbychildren(right);
                }
            }
        }

        string offset(int idx) {
            string spaces = "";
            int offset = int(floor(sqrt(heapsize-1)+0.5))-int(floor(sqrt(idx)+0.5));
            for (int i=0;i<offset;i++) {
                spaces += "    ";
            }
            if (!(offset))
                spaces += " ";
            return spaces;
        }

    public:
        FHeap() {heapsize=0;}
        ~FHeap() {};

        int size() {
            return heapsize;
        }

        void add(int item) {
            heap[heapsize] = item;
            heapsize++;
            sortbyparent(heapsize-1);
        }

        int pop() {
            int root;
            root = heap[0];
            heap[0] = heap[heapsize-1];
            heap[heapsize-1] = 0;
            heapsize--;
            sortbychildren(0);
            return root;
        }

        // TODO: allow removing of arbitrary nodes
        void remove(int value) {
            int idx = 0;
            while (heap[idx] != value && idx < heapsize) idx++;
            int node;
            node = heap[idx];
            heap[0] = heap[heapsize-1];  // swap node to remove and lower rightmost node
            heap[heapsize-1] = 0;
            heapsize--;
            sortbyparent(idx);
            sortbychildren(0);
        }

        string unsorted() {
            string unsortedlist = "";
            for (int i=0;i<heapsize;i++) {
                unsortedlist += itostring(heap[i]);
                unsortedlist += ",";
            }
            return unsortedlist;
        }

        string sorted() {
            string sortedlist = "";
            int size = heapsize;
            for (int i=0;i<size;i++) {
                sortedlist += itostring(pop());
                sortedlist += ",";
            }
            return sortedlist;
        }

        void print() {
            int idx = 0;
            // for each level in the tree
            cout << "\033[1;31m";
            for (int l=1;l<heapsize+1;l=l*2) {
                // print the nodes on that level
                for (int i=0;i<l;i++) {
                    if (idx < heapsize)
                        cout << "(" << heap[idx] << ")";
                    idx++;
                }
                cout << endl;
            }
            cout << "\033[0m";
        }
};


int main () {
    FHeap heap;

    string toaddtoheap;
    cout << "[i] Input numbers you would like added to the heap, separated by commas semicolons or spaces: ";
    cin >> toaddtoheap; cin.clear();

    vector<int> tokens = tokenize(toaddtoheap);
    if (tokens.size() == 0) return 1;
    for (int i=0; i<tokens.size(); i++)
        heap.add(tokens[i]);

    cout << "[o] Heap Diagram:" << endl;
    heap.print();

    int toremovefromheap;
    cout << "[i] Input the number of nodes you'd like removed from the top of the heap:";
    cin >> toremovefromheap; cin.clear();

    for (int i=0; i<toremovefromheap; i++)
        heap.pop();

    cout << "[o] Heap diagram:" << endl;
    heap.print();
    cout << "[o] Unsorted heap:" << endl;
    cout << heap.unsorted() << endl;
    cout << "[o] Sorted heap:" << endl;
    cout << "\033[1;33m" << heap.sorted() << "\033[0m" << endl;
}
