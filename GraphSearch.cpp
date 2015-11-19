/*  Nick Sweeting 2014/05/08
    Graph Creator
    MIT License
    g++ GraphSearch.cpp -o ./main && ./main

    A graph and breadth-first-search implemenation that can read vertices and edges from a file and print the adjacency table.
*/

#include <stdlib.h>
#include <time.h>       // rand seed
#include <string>
#include <vector>       // vector of tokenized input
#include <queue>        // breadth-first search
#include <iostream>
#include <fstream>      // file()
#include <map>
using namespace std;

#define LABEL first      // we use a map to store vertex LABEL:VALUE, so for convenience I prefer to access them via vertex.LABEL & row.COLS instead of vertex.first and row.second
#define VALUE second
#define COLS second
#define CONTAINS(obj, val) (obj.find(val) != obj.end())  // helper func because c++ has no 'in' keword, e.g. `val in obj`

const string alphabet[36] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};    // for generating random graph vertex labels

const string RED = "\033[1;31m";
const string YELLOW = "\033[1;33m";
const string ENDCOLOR = "\033[0m";

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

        matrix adjTable;                            // init the adjacency table

        // get all verticies adjacent to a given vertex
        vector<string> getAdjacent(string label) {
            vector<string> adj;
            // iterate through that vertex's row in adjTable
            for (auto& vertex: adjTable[label]) {
                // push any vertecies (that arent the current one) where edge = 1
                if (*(vertex.VALUE) == 1 && vertex.LABEL != label)
                    adj.push_back(vertex.LABEL);
            }
            return adj;
        }

        void updateEdge(string vertex1, string vertex2, int status) {
            // check to make sure both verticies exist
            if (!CONTAINS(adjTable, vertex1)) {
                cout << endl << RED << "[X] Vertex with label '" << vertex1 << "' does not exist." << ENDCOLOR << endl;
                return;
            }
            if (!CONTAINS(adjTable, vertex2)) {
                cout << endl << RED << "[X] Vertex with label '" << vertex2 << "' does not exist." << ENDCOLOR << endl;
                return;
            }
            // the adjTable is automatically consistent (A-B and B-A)
            *(adjTable[vertex1][vertex2]) = status;
            // *(adjTable[vertex2][vertex1]) = status;  <- this is unecessary because both edges are pointers to the same int
        }

    public:
        Graph(int size=0) {
            // init a default graph with up to 36 verticies labeled A through Z
            for (int i=0; i < size && i < 37; i++)
                addVertex(alphabet[i]);
        }
        ~Graph() {}

        void addVertex(string label) {
            if (CONTAINS(adjTable, label)) {
                cout << endl << RED << "[X] Vertex with label '" << label << "' already exists." << ENDCOLOR << endl;
                return;
            }
            if (label == "" || label == " ") return;
            
            list new_adj_row;
            new_adj_row[label] = new int(0);                    // vertexes are always disconnected to themselves
           
            for (auto& vertex: adjTable)                        // seed the new adjacency table row with edges to all the other vertecies
                new_adj_row[vertex.LABEL] = new int(0);         // new vertex starts disconnected to all the others
            
            adjTable[label] = new_adj_row;                      // add the new row to the adjTable
            
            for (auto& row: adjTable)                           // add a new column to all the other rows
                row.COLS[label] = new_adj_row[row.LABEL];       // re-use the pointers created above, so that both point to the same int
        }

        void removeVertex(string vertex) {
            if (!CONTAINS(adjTable, vertex)) {
                cout << endl << RED << "[X] Vertex with label '" << vertex << "' does not exist." << ENDCOLOR << endl;
                return;
            }
            adjTable.erase(vertex);                             // remove the vertex's row
            for (auto& row: adjTable)                           // remove the column from all the other rows
                row.COLS.erase(vertex);
        }

        void addEdge(string vertex1, string vertex2) {
            updateEdge(vertex1, vertex2, 1);
        }

        void removeEdge(string vertex1, string vertex2) {
            updateEdge(vertex1, vertex2, 0);
        }

        void breadthFirst(string start, string end) {
            if (!CONTAINS(adjTable, start)) {
                cout << endl << RED << "[X] Vertex with label '" << start << "' does not exist." << ENDCOLOR << endl;
                return;
            }
            if (!CONTAINS(adjTable, end)) {
                cout << endl << RED << "[X] Vertex with label '" << end << "' does not exist." << ENDCOLOR << endl;
                return;
            }
            if (start == end) {
                cout << YELLOW << "[√] Start and End are the same vertex! " << end << "-" << start << "" << ENDCOLOR << endl;
                return;
            }
            if (getAdjacent(start).empty() || getAdjacent(end).empty()) {
                cout << RED << "[X] Start or End is orphaned. No path exists." << ENDCOLOR << endl;
                return;
            }

            // begin with start, push all adjacent verticies to the queue, pop top of queue and repeat until end is found
            // the path is recorded using the tofrom array, which simply records how we got to each vertex
            // to get the path, start at tofrom[end], then call tofrom[tofrom[end]] to see the one before it and so on
            queue<string> q;
            map<string, string> tofrom;
            tofrom[start] = start;          // set the beginning of the tofrom beadcrumb trail to the start vertex
            q.push(start);
            string root;
            while (q.size() > 0) {
                root = q.front(); q.pop();
                for (string& next: getAdjacent(root)) {
                    // if the next vertex has not already been visited, add it to the search queue
                    if (!CONTAINS(tofrom, next)) {
                        tofrom[next] = root;
                        q.push(next);
                    }
                    // if the next vertex is the end vertex, stop searching
                    if (next == end) {
                        cout << endl << YELLOW << "[√] Found path! " << end;
                        // follow the breadcrumbs back to the start to get the path
                        string last = end;
                        while (tofrom[last] != start) {
                            last = tofrom[last];
                            cout << "-" << last;
                        }
                        cout << "-" << start << ENDCOLOR << endl << endl;
                        return;
                    }
                }
            }
            cout << RED << "[X] Path not found." << ENDCOLOR << endl;
        }

        void dijkstra(string vertex1, string vertex2) {
            // left as an excersize to the reader
            breadthFirst(vertex1, vertex2);
        }

        void printAdjTable() {
            cout << endl << " ";
            // print top labels
            for (auto& row: adjTable)
                cout << "|" << row.LABEL;
            cout << endl;
            // print each row
            for (auto& row: adjTable) {
                cout << row.LABEL << "|";
                for (auto& col: row.COLS)
                    cout << (*(col.VALUE) ? RED : YELLOW) << *(col.VALUE) << " " << ENDCOLOR;
                cout << endl;
            }
            cout << endl;
        }

        int size() {
            return adjTable.size();
        }
};

int main() {
    auto* graph = new Graph();
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
            for (string& vertex: tokens)
                graph->addVertex(vertex);
        }
        else if (n == 2) {
            cout << "[i] Verticies to remove\n e.g. A,C,D :";
            string intext;
            cin >> intext; cin.clear();
            vector<string> tokens = tokenize(intext);
            for (string& vertex: tokens)
                graph->removeVertex(vertex);
        }
        else if (n == 3) {
            cout << "[i] Verticies to link\n e.g. A-E :";
            string intext;
            cin >> intext; cin.clear();
            vector<string> tokens = tokenize(intext);
            if (tokens.size() == 2)
                graph->addEdge(tokens[0], tokens[1]);
            else
                cout << RED << "[X] Only two verticies can be linked." << ENDCOLOR << endl;
        }
        else if (n == 4) {
            cout << "[i] Verticies to unlink\n e.g A-B :";
            string intext;
            cin >> intext; cin.clear();
            vector<string> tokens = tokenize(intext);
            if (tokens.size() == 2)
                graph->removeEdge(tokens[0], tokens[1]);
            else
                cout << RED << "[X] Only two verticies can be unlinked." << ENDCOLOR << endl;
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
                cout << RED << "[X] You can only find the path between two verticies." << ENDCOLOR << endl;
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
                cout << RED << "[X] You can only find the path between two verticies." << ENDCOLOR << endl;
        }
        else if (n == 7) {
            // generate a random demo board
            cout << "[i] Number of verticies to generate\n(enter 0 to leave graph unchanged):";
            int verticies;
            cin >> verticies; cin.clear();
            if (verticies < 37 && verticies > 0)
                *graph = Graph(verticies);
            else if (verticies > 37)
                cout << RED << "[X] Demo graph has a max of 36 vertices (because it uses the alphabet & numbers as labels)" << ENDCOLOR << endl;

            cout << "[i] Number of edges to randomly create\n(you need about 40 to reliably get a path from A-Z):";
            int edges;
            cin >> edges; cin.clear();
            verticies = graph->size();
            srand(time(NULL));      // init random number seed from time
            if (edges <= verticies * verticies) {
                for (int i=0; i<edges; i++)
                    graph->addEdge(alphabet[rand() % verticies], alphabet[rand() % verticies]);
            }
            else
                cout << RED << "[X] Too many edges to fit on this graph." << ENDCOLOR << endl;
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
                if (block[i] == '\n') break;
                intext += block[i];
            }

            // add verticies
            vector<string> tokens = tokenize(intext);
            if (tokens.empty())
                cout << RED << "[X] No properly formatted verticies found (A,B,C,...) in file" << ENDCOLOR << endl;
            else {
                for (string& vertex: tokens)
                    graph->addVertex(vertex);
            }

            // read in edges "A-B,B-C,C-D,D-F,F-E,C-G"
            intext = ""; i++;  // re-using i so that we start where we left off (skipping the newline of course)
            for (; i<65536; i++) {
                if (block[i] == '\n') break;
                intext += block[i];
            }

            // add edges
            vector<string> links = tokenize(intext, ",");
            if (links.empty())
                cout << RED << "[X] No properly formatted edges found (A-B,A-C,C-B,...) in file" << ENDCOLOR << endl;
            else {
                for (string& link_str: tokens) {
                    vector<string> link = tokenize(link_str, "-");
                    if (link.size() != 2)
                        cout << RED << "[X] Incomplete link found: " << link[0] << "-" << endl;
                    else
                        graph->addEdge(link[0], link[1]);
                }
            }

            delete[] block;
        }
        else if (n == 0) return 0;
        else cout << RED << "[X] Enter the number of the command you're trying to run." << ENDCOLOR << endl;
    }
}
