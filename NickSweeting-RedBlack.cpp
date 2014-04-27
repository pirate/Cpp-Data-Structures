/*  Nick Sweeting 2014/03/20
    RedBlack
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

const string delims = ",; ";            // for reading csv
const int maxwidth = 240;               // max terminal width for centering output

#define RED     1
#define BLACK   2

struct node {
    int key;
    struct node *left, *right, *p;      // p = parent
    int color;                          // RED || BLACK
};

typedef struct node *nodep;             // nodepointer
struct node nil;
nodep nilp = &nil;                      // instead of using NULL for comparisons we use a nil pointer, which is a pointer with a recursively defined left & right that point to itself

// this is my first venture into functional-style c++ programming
// i'm goind to try to implement redblack without using state
// no size counter is kept, everything is a function that takes a tree as input and either returns a response or is called for a side effect (like printing to stdout)

// x,y are like temp1, temp2; user input is stored in Z

void sorted(nodep x) {
    if (x != nilp) {
        sorted(x->left);
        cout << x->key << ", ";
        sorted(x->right);
    }
}

nodep search(nodep root, int k) {
    if (root == nilp || root->key == k)
        return root;
    if (k < root->key)
        return search(root->left, k);
    else
        return search(root->right, k);
}

nodep min(nodep root) {
    while (root->left != nilp)
        root = root->left;
    return root;
}

nodep max(nodep root) {
    while (root->right != nilp)
        root = root->right;
    return root;
}

nodep after(nodep root, int x) {
    nodep temp = search(root, x);
    if (temp == nilp) {
        cout << "[X]" << x << " not in tree" << endl;
        return NULL;
    }
    if (temp->right != nilp)
        return min(temp->right);
    nodep y = temp->p;
    while (y != nilp && temp == y->right) {
        temp = y;
        y = y->p;
    }
    return y;
}

nodep before(nodep root, int x) {
    nodep temp = search(root, x);
    if (temp == nilp) {
        cout << "[X]" << x << " not in tree" << endl;
        return NULL;
    }
    if (temp->left != nilp)
        return max(temp->left);
    nodep y = temp->p;
    while (y != nilp && temp == y->left) {
        temp = y;
        y = y->p;
    }
    return y;
}

void rotateleft(nodep *treeroot, nodep x) {
    nodep y = x->right;
    x->right = y->left;
    if (y->left != nilp)
        y->left->p = x;
    y->p = x->p;
    if (x->p == nilp)
        *treeroot = y;
    else if (x->p->left == x)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
}

void rotateright(nodep *treeroot, nodep y) {
    nodep x = y->left;
    y->left = x->right;
    if (x->right != nilp)
        x->right->p = y;
    x->p = y->p;
    if (y->p == nilp)
        *treeroot = x;
    else if (y->p->left == y)
        y->p->left = x;
    else
        y->p->right = x;
    x->right = y;
    y->p = x;
}

void treeinsertfix(nodep *treeroot, nodep z) {
    while (z->p->color == RED) {
        if (z->p == z->p->p->left) {                        // parent parent
            nodep y = z->p->p->right;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else {
                if (z == z->p->right) {
                    z = z->p;
                    rotateleft(treeroot,z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                rotateright(treeroot,z->p->p);
            }
        }
        else {
            nodep y = z->p->p->left;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else {
                if (z == z->p->left) {
                    z = z->p;
                    rotateright(treeroot,z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                rotateleft(treeroot,z->p->p);
            }
        }
    }
    (*treeroot)->color = BLACK;
}

void treeinsert(nodep *treeroot, int z) {
    nodep Z = (nodep) malloc(sizeof(struct node));          // wohoho, I've discovered some new dangerous toys from C-land to play with
    Z->key = z;
    nodep y = nilp;
    nodep x = *treeroot;
    while (x != nilp) {
        y = x;
        if (Z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    Z->p = y;
    if (y == nilp)
        *treeroot = Z;
    else if (Z->key < y->key)
        y->left = Z;
    else
        y->right = Z;
    Z->left = nilp;
    Z->right = nilp;
    Z->color = RED;
    treeinsertfix(treeroot,Z);
}

void treetransplant(nodep *treeroot, nodep u, nodep v) {
    if (u->p == nilp)
        *treeroot = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}

void treedeletefix(nodep *treeroot, nodep x) {
    while (x != *treeroot && x->color == BLACK) {
        if (x == x->p->left) {
            nodep w = x->p->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                rotateleft(treeroot,x->p);
                w = x->p->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->p;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateright(treeroot,w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                rotateleft(treeroot,x->p);
                x = *treeroot;
            }
        }
        else {
            nodep w = x->p->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                rotateright(treeroot,x->p);
                w = x->p->left;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->p;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateleft(treeroot,w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                rotateright(treeroot,x->p);
                x = *treeroot;
            }
        }
    }
    x->color = BLACK;
}

void treedelete(nodep *treeroot, int z) {
    nodep Z = search(*treeroot, z);
    if (Z == nilp) {
        cout << "[X] Node to be deleted not found" << endl;
        return;
    }
    nodep y = Z;
    int yoc = y->color;
    nodep x;
    if (Z->left == nilp) {
        x = Z->right;
        treetransplant(treeroot,Z,Z->right);
    }
    else if (Z->right == nilp) {
        x = Z->left;
        treetransplant(treeroot,Z,Z->left);
    }
    else {
        y = min(Z->right);
        yoc = y->color;
        x = y->right;
        if (y->p == Z)
            x->p = y;
        else {
            treetransplant(treeroot,y,y->right);
            y->right = Z->right;
            y->right->p = y;
        }
        treetransplant(treeroot,Z,y);
        y->left = Z->left;
        y->left->p = y;
        y->color = Z->color;
    }
    if (yoc == BLACK)
        treedeletefix(treeroot,x);
}

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
        //tokens.push_back(stoint(input.substr(pos, 1)));                   // this adds the delimiter as a token as well, useful for catching operators as tokens (e.g. +-()*/^)
        prev = pos+1;
    }
    // catch trailing token after the last delimiter
    if (prev < input.length())
        tokens.push_back(stoint(input.substr(prev, string::npos)));

    return tokens;
}

void center(string centerstr) {
    int len = centerstr.length();
    if (len > maxwidth) {
        centerstr = "...";
        len = 3;
    }
    for (int i=0; i<floor((maxwidth-len)/2); i++)
        cout << " ";
    cout << centerstr << endl;
}

// version with color (breaks centering due to invisible characters)
// string ntostring(nodep n) {
//     string nodestr;
//     if (n == nilp)
//         nodestr = "\033[1;33mNIL|B\033[0m";
//     else if (n->color == RED)
//         nodestr = "\033[1;31m" + itostring(n->key)+"|R" + "\033[0m";
//     else
//         nodestr = "\033[1;33m" + itostring(n->key)+"|B" + "\033[0m";
//     return nodestr;
// }

string ntostring(nodep n) {
    string nodestr;
    if (n == nilp)
        nodestr = "NIL|B";
    else if (n->color == RED)
        nodestr = itostring(n->key)+"|R";
    else
        nodestr = itostring(n->key)+"|B";
    return nodestr;
}

void visualize(nodep root) {
    queue<nodep> q;

    q.push(root);
    q.push(0);

    string out = "";

    while (q.size() > 1) { // one nullptr will always remain in the list. So test for size>1
        root=q.front();
        q.pop();
        if (!root) {center(out); out = ""; q.push(0); continue; }

        out += ntostring(root) + " ";

        if (root != nilp) {
            q.push(root->left);
            q.push(root->right);
        }
    }
}

int main () {
    nil.left = nil.right = nil.p = nilp;  // magic
    nil.color = BLACK;  // black magic
    nodep tree = nilp; // oh no! where did it go?
    // ^ inits the helpful nilp so that we can use it for comparisons
    int n;  // sorcery!
    cout << "Will generate and balance a RedBlack tree, it can handle numbers between 0 and the max (int) size on your system\n[1] add\n[2] remove\n[3] find\n[4] sorted\n[5] min\n[6] max\n---\n[7] read file\n[8] visualize\n[9] link to python script\n[0] quit\n\n";
    while (1) {
        cout << "[#]:";
        cin >> n;
        if (n == 1) {
            cout << "[i] Numbers to add:";
            string intext;
            cin >> intext; cin.clear();
            vector<int> tokens = tokenize(intext);
            for (int i=0; i<tokens.size(); i++)
                treeinsert(&tree, tokens[i]);
        }
        else if (n == 2) {
            cout << "[i] Numbers to remove:";
            string intext;
            cin >> intext; cin.clear();
            vector<int> tokens = tokenize(intext);
            for (int i=0; i<tokens.size(); i++)
                treedelete(&tree, tokens[i]);
        }
        else if (n == 3) {
            cout << "[i] Number you want to search for:";
            int num;
            cin >> num; cin.clear();
            if (search(tree, num) == nilp)
                cout << "[X] " << num << " not found" << endl;
            else
                cout << "[√] " << num << " found" << endl;
        }
        else if (n == 4) {
            cout << "[<] ";
            sorted(tree);
            cout << endl;
        }
        else if (n == 5) {
            cout << "[-] " << min(tree)->key << endl;
        }
        else if (n == 6) {
            cout << "[+] " << max(tree)->key << endl;
        }
        else if (n == 7) {
            string infile;
            cout << "[i] Filename:";
            cin >> infile;
            std::ifstream file(infile, std::ios::binary);
            std::streambuf* raw_buffer = file.rdbuf();
            char* block = new char[65536]; // max file size
            raw_buffer->sgetn(block, 65536);
            string intext = "";
            for (int i=0; i<65536; i++)
                intext += block[i];
            vector<int> tokens = tokenize(intext);
            if (tokens.size() == 1)
                cout << "[X] No CSV formatted input found (numbers delimited by , and/or ;)" << endl;
            else {
                for (int i=0; i<tokens.size(); i++)
                    treeinsert(&tree, tokens[i]);
            }
            delete[] block;
        }
        else if (n == 8) {
            cout << "[*] RedBlack tree visualization" << endl;
            visualize(tree);
            cout << endl;
        }
        else if (n == 9) {
            cout << endl << "[*] Redblack.py" << endl << "[√] Ready..." << endl;
            // {{ % hook % }}
        }
        else if (n == 0) {
            break; // quit
        }
        else if (n == 11) {                                         // everybody likes easter eggs
            cout << "[i] Find the number after this:";
            int num;
            cin >> num; cin.clear();
            nodep t = after(tree, num);
            if (t != NULL)
                cout << "[√] " << t->key << endl;
        }
        else if (n == 12) {
            cout << "[i] Find the number before this:";
            int num;
            cin >> num; cin.clear();
            nodep t = before(tree, num);
            if (t != NULL)
                cout << "[√]" << t->key << endl;
        }
        else
            cout << "[X] Enter the number of the command you're trying to run." << endl;
    }
    return 0;
}
