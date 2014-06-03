/*  Nick Sweeting 2014/05/08
    Graph Creator
    MIT License
*/

#include <stdlib.h>
#include <time.h>       // rand seed
#include <string>       // duh
#include <vector>       // vector of tokenized input
#include <queue>        // breadth-first search
#include <iostream>     // cout
#include <fstream>      // file()
#include <map>          // map
using namespace std;

typedef int(5) int;

const string alphabet[26] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};    // for generating random graph vertex labels

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

class Graph {
    private:
        typedef map<string, int*> list;             // grid row
        typedef map<string, list> matrix;           // grid columns
        typedef list::iterator listiter;            // grid row iterator (used to iterate over a map in a for loop) iter->first is key and iter->second is value
        typedef matrix::iterator matrixiter;        // grid column iterator (used to iterate over a map in a for loop)

        matrix adjTable;                            // init the adjacency table

        // get all verticies adjacent to a given vertex
        vector<string> getAdjacent(string label) {
            vector<string> adj;
            // iterate through that vertex's row in adjTable
            for (listiter iter = adjTable[label].begin(); iter != adjTable[label].end(); iter++) {
                // push any vertecies (that arent the current one) where edge = 1
                if (*(iter->second) == 1 && iter->first != label) {
                    adj.push_back(iter->first);
                }
            }
            //cout << label << ": "; for (int i=0; i<adj.size(); i++) cout << adj[i] << ","; cout << endl; // debug
            return adj;
        }

        void updateEdge(string vertex1, string vertex2, int status) {
            // check to make sure both verticies exist
            if (adjTable.find(vertex1) == adjTable.end()) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << vertex1 << "' does not exist.\033[0m" << endl;
                return;
            }
            else if (adjTable.find(vertex2) == adjTable.end()) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << vertex2 << "' does not exist.\033[0m" << endl;
                return;
            }
            else {
                // keep the adjTable consistent by updating both A-B and B-A
                *(adjTable[vertex1][vertex2]) = status;
                *(adjTable[vertex2][vertex1]) = status;
            }
        }

    public:
        Graph(int size=0) {
            // init a default graph with up to 26 verticies labeled A through Z
            for (int i=0; i < size && i < 27; i++)
                addVertex(alphabet[i]);
        }
        ~Graph() {
        }

        void addVertex(string label) {
            // see if it already exists
            if (adjTable.find(label) != adjTable.end()) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << label << "' already exists.\033[0m" << endl;
                return;
            }
            // make a new row for the new vertex
            list row;
            row[label] = new int(0);
            // seed the row with all the other vertecies
            for (matrixiter iterator = adjTable.begin(); iterator != adjTable.end(); iterator++)
                row[iterator->first] = new int(0);
            adjTable[label] = row;
            // add a new column to all the other rows
            for (matrixiter iterator = adjTable.begin(); iterator != adjTable.end(); iterator++)
                iterator->second[label] = new int(0);
        }

        void removeVertex(string label) {
            if (adjTable.find(label) == adjTable.end()) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << label << "' does not exist.\033[0m" << endl;
                return;
            }
            // remove the vertex's row
            adjTable.erase(label);
            // remove the column from all the other rows
            for (matrixiter iterator = adjTable.begin(); iterator != adjTable.end(); iterator++) {
                iterator->second.erase(label);
            }
        }

        void addEdge(string vertex1, string vertex2) {
            updateEdge(vertex1, vertex2, 1);
        }

        void removeEdge(string vertex1, string vertex2) {
            updateEdge(vertex1, vertex2, 0);
        }

        void breadthFirst(string vertex1, string vertex2) {
            if (adjTable.find(vertex1) == adjTable.end()) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << vertex1 << "' does not exist.\033[0m" << endl;
                return;
            }
            else if (adjTable.find(vertex2) == adjTable.end()) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << vertex2 << "' does not exist.\033[0m" << endl;
                return;
            }
            // if start and end are equal, no path finding is needed
            else if (vertex1 == vertex2) {
                cout << "\033[1;33m[√] Found path! " << vertex2 << "-" << vertex1 << "\033[0m";
                return;
            }
            // begin with vertex1, push all adjacent verticies to the queue, then repeat until a path is found
            // the path is recorded using the tofrom array, which simply records how we got to each vertex
            // to get the path, start at tofrom[end], then call tofrom[tofrom[end]] to see the one before it and so on
            queue<string> q;
            map<string, string> tofrom;
            tofrom[vertex1] = vertex1;  // set the beginning of the tofrom beadcrumb trail to something easy to check so that we dont have an infinite loop
            q.push(vertex1);            // start the queue at vertex1
            string root = vertex1;
            while (q.size() > 0) {
                root=q.front(); q.pop();
                vector<string> adj = getAdjacent(root);
                for (int i=0; i<adj.size(); i++) {
                    // if the next vertex is not the same as the last one (we want to keep moving forward), push it on the queue
                    if (adj[i] != tofrom[root]) {
                        tofrom[adj[i]] = root;
                        q.push(adj[i]);
                    }
                    // if the next vertex is the end vertex, stop searching
                    if (adj[i] == vertex2) {
                        string hopper = adj[i];
                        cout << "\033[1;33m[√] Found path! " << vertex2;
                        // follow the breadcrumbs back to the start to get the path
                        while (tofrom[hopper] != vertex1) {
                            cout << "-" << tofrom[hopper];
                            hopper = tofrom[hopper];
                        }
                        cout << "-" << vertex1 << "\033[0m" << endl;
                        return;
                    }
                }
            }
            cout << "\033[1;31m[X] Path not found.\033[0m" << endl;
        }

        void dijkstra(string vertex1, string vertex2) {
            // left as an excersize to the reader
            breadthFirst(vertex1, vertex2);
        }

        void printAdjTable() {
            cout << endl << " ";
            // print top labels
            for (matrixiter iterator = adjTable.begin(); iterator != adjTable.end(); iterator++)
                cout << "|" << iterator->first;
            cout << endl;
            // print each row
            for (matrixiter iterator = adjTable.begin(); iterator != adjTable.end(); iterator++) {
                cout << iterator->first << "|";
                list *second = &(iterator->second);
                for (listiter iterator2 = second->begin(); iterator2 != second->end(); iterator2++) {
                    if (*(iterator2->second))
                        cout << "\033[1;31m" << *(iterator2->second) << "\033[0m ";
                    else
                        cout << "\033[1;33m" << *(iterator2->second) << "\033[0m ";
                }
                cout << endl;
            }
            cout << endl;
        }

        int size() {
            return adjTable.size();
        }
};

int main() {
    // init random number seed from time
    srand(time(NULL));
    // init graph
    Graph *graph = new Graph();
    cout << "Will generate and map a graph. Points can be labeled and referenced using strings.\n[1] add verticies\n[2] remove verticies\n[3] add edges\n[4] remove edges\n[5] breadth-first shortest path\n[6] dijkstra's shortest path\n---\n[7] generate demo graph\n[8] read graph from file\n[0] quit\n\n";
    int n;
    while (1) {
        graph->printAdjTable();
        cout << "[#]:";
        cin >> n;
        if (n == 1) {
            cout << "[i] Verticies to add\n e.g A,B,C,D,E :";
            string intext;
            cin >> intext; cin.clear();
            // tokenizing input allows them to enter multiple verticies on a single line
            vector<string> tokens = tokenize(intext);
            for (int i=0; i<tokens.size(); i++)
                graph->addVertex(tokens[i]);
        }
        else if (n == 2) {
            cout << "[i] Verticies to remove\n e.g. A,C,D :";
            string intext;
            cin >> intext; cin.clear();
            vector<string> tokens = tokenize(intext);
            for (int i=0; i<tokens.size(); i++)
                graph->removeVertex(tokens[i]);
        }
        else if (n == 3) {
            cout << "[i] Verticies to link\n e.g. A-E :";
            string intext;
            cin >> intext; cin.clear();
            vector<string> tokens = tokenize(intext);
            if (tokens.size() == 2)
                graph->addEdge(tokens[0], tokens[1]);
            else
                cout << "\033[1;31m[X] Only two verticies can be linked.\033[0m" << endl;
        }
        else if (n == 4) {
            cout << "[i] Verticies to unlink\n e.g A-B :";
            string intext;
            cin >> intext; cin.clear();
            vector<string> tokens = tokenize(intext);
            if (tokens.size() == 2)
                graph->removeEdge(tokens[0], tokens[1]);
            else
                cout << "\033[1;31m[X] Only two verticies can be unlinked.\033[0m" << endl;
        }
        else if (n == 5) {
            // breadth first path finding search
            cout << "[i] Path starting ending verticies\n e.g. A-Z :";
            string intext;
            cin >> intext; cin.clear();
            vector<string> tokens = tokenize(intext);
            if (tokens.size() == 2)
                graph->breadthFirst(tokens[0], tokens[1]);
            else
                cout << "\033[1;31m[X] You can only find the path between two verticies.\033[0m" << endl;
        }
        else if (n == 6) {
            // fake dijkstra's seach (it just uses breadth-first)
            cout << "[i] Path starting ending verticies\n e.g. A-Z :";
            string intext;
            cin >> intext; cin.clear();
            vector<string> tokens = tokenize(intext);
            if (tokens.size() == 2)
                graph->dijkstra(tokens[0], tokens[1]);
            else
                cout << "\033[1;31m[X] You can only find the path between two verticies.\033[0m" << endl;
        }
        else if (n == 7) {
            // generate a random demo board
            cout << "[i] Number of verticies to generate\n(enter 0 to leave graph unchanged):";
            int verticies;
            cin >> verticies; cin.clear();
            if (verticies < 27 && verticies > 0)
                *graph = Graph(verticies);
            else if (verticies > 27)
                cout << "\033[1;31m[X] Demo graph has a max of 26 vertices (because it uses the alphabet as labels)\033[0m" << endl;
            else
                continue; // do not remove, bad things will happen

            cout << "[i] Number of edges to randomly create\n(you need about 40 to reliably get a path from A-Z):";
            int edges;
            cin >> edges; cin.clear();
            if (edges <= (graph->size())*(graph->size())) {
                for (int i=0; i<edges; i++) {
                    graph->addEdge(alphabet[rand()%(graph->size())], alphabet[rand()%(graph->size())]);
                }
            }
            else
                cout << "\033[1;31m[X] Too many edges to fit on this graph.\033[0m" << endl;
        }
        else if (n == 8) {
            // load board from a file
            string infile;
            cout << "[*] File must have a list of verticies separated by commas, a newline, then a list of comma separated vertex1-vertex2 edges." << endl;
            cout << "[i] Filename:";
            cin >> infile;

            // set up file reading
            std::ifstream file(infile, std::ios::binary);
            std::streambuf* raw_buffer = file.rdbuf();
            char* block = new char[65536]; // max file size
            raw_buffer->sgetn(block, 65536);
            string intext = "";

            // read in verticies "A,B,C,D,E,F,G"
            int i=0;
            for (; i<65536; i++) {
                if (block[i] == '\n')
                    break;
                intext += block[i];
            }

            // add verticies
            vector<string> tokens = tokenize(intext);
            if (tokens.empty())
                cout << "\033[1;31m[X] No properly formatted verticies found (A,B,C,...)\033[0m" << endl;
            else {
                for (int i=0; i<tokens.size(); i++)
                    graph->addVertex(tokens[i]);
            }

            // read in edges "A-B,B-C,C-D,D-F,F-E,C-G"
            intext = ""; i++;  // re-using i so that we start where we left off (skipping the newline of course)
            for (; i<65536; i++) {
                if (block[i] == '\n')
                    break;
                intext += block[i];
            }

            // add edges
            vector<string> links = tokenize(intext, ",");
            if (links.empty())
                cout << "\033[1;31m[X] No properly formatted edges found (A-B,A-C,C-B,...)\033[0m" << endl;
            else {
                for (int i=0; i<links.size(); i++) {
                    vector<string> link = tokenize(links[i], "-");
                    if (link.size() != 2)
                        cout << "\033[1;31m[X] Incomplete link found: " << link[0] << "-" << endl;
                    else
                        graph->addEdge(link[0], link[1]);
                }
            }

            delete[] block;
        }
        else if (n == 0)
            break; // quit
        else
            cout << "\033[1;31m[X] Enter the number of the command you're trying to run.\033[0m" << endl;
    }
}
