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
#define CONTAINS(obj, val) (obj.find(val) != obj.end())  // helper func because checking if the result of obj.find(val) = obj.end() is stupid and opaque

const string alphabet[36] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};    // for generating random graph vertex labels

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
                cout << endl << "\033[1;31m[X] Vertex with label '" << vertex1 << "' does not exist.\033[0m" << endl;
                return;
            }
            if (!CONTAINS(adjTable, vertex2)) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << vertex2 << "' does not exist.\033[0m" << endl;
                return;
            }
            // the adjTable is automatically consistent (edge is markes as both A-B and B-A)
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
            // see if it already exists
            if (CONTAINS(adjTable, label)) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << label << "' already exists.\033[0m" << endl;
                return;
            }
            if (label == "" || label == " ") return;
            // make a new row for the new vertex
            list new_row;
            new_row[label] = new int(0);               // vertexes are always disconnected to themselves
            
            // seed the row with edges to all the other vertecies
            for (auto& row: adjTable)
                new_row[row.LABEL] = new int(0);       // new vertex starts disconnected to all the others

            // add the new row to the adjTable
            adjTable[label] = new_row;

            // add a new column to all the other rows
            for (auto& row: adjTable)
                row.COLS[label] = new_row[row.LABEL];  // re-uses the pointers created above, so that both point to the same int
        }

        void removeVertex(string label) {
            if (!CONTAINS(adjTable, label)) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << label << "' does not exist.\033[0m" << endl;
                return;
            }
            // remove the vertex's row
            adjTable.erase(label);
            // remove the column from all the other rows
            for (auto& row: adjTable)
                row.COLS.erase(label);
        }

        void addEdge(string vertex1, string vertex2) {
            updateEdge(vertex1, vertex2, 1);
        }

        void removeEdge(string vertex1, string vertex2) {
            updateEdge(vertex1, vertex2, 0);
        }

        void breadthFirst(string start, string end) {
            if (!CONTAINS(adjTable, start)) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << start << "' does not exist.\033[0m" << endl;
                return;
            }
            if (!CONTAINS(adjTable, end)) {
                cout << endl << "\033[1;31m[X] Vertex with label '" << end << "' does not exist.\033[0m" << endl;
                return;
            }
            // if start and end are equal, no path finding is needed
            if (start == end) {
                cout << "\033[1;33m[√] Found path! " << end << "-" << start << "\033[0m";
                return;
            }
            if (getAdjacent(start).empty() || getAdjacent(end).empty()) {
                cout << "\033[1;31m[X] Start or end is orphaned. No path exists.\033[0m" << endl;
                return;
            }

            // begin with start, push all adjacent verticies to the queue, then repeat until a path is found
            // the path is recorded using the tofrom array, which simply records how we got to each vertex
            // to get the path, start at tofrom[end], then call tofrom[tofrom[end]] to see the one before it and so on
            queue<string> q;
            map<string, string> tofrom;
            tofrom[start] = start;    // set the beginning of the tofrom beadcrumb trail to something easy to check so that we dont have an infinite loop
            q.push(start);                   // start the queue at start
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
                        string hopper = next;
                        cout << endl << "\033[1;33m[√] Found path! " << end;
                        // follow the breadcrumbs back to the start to get the path
                        while (tofrom[hopper] != start) {
                            cout << "-" << tofrom[hopper];
                            hopper = tofrom[hopper];
                        }
                        cout << "-" << start << "\033[0m" << endl << endl;
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
            for (auto& row: adjTable)
                cout << "|" << row.LABEL;
            cout << endl;
            // print each row
            for (auto& row: adjTable) {
                cout << row.LABEL << "|";
                for (auto& col: row.COLS) {
                    if (*(col.VALUE))
                        cout << "\033[1;31m" << *(col.VALUE) << "\033[0m ";
                    else
                        cout << "\033[1;33m" << *(col.VALUE) << "\033[0m ";
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
            if (verticies < 37 && verticies > 0)
                *graph = Graph(verticies);
            else if (verticies > 37)
                cout << "\033[1;31m[X] Demo graph has a max of 36 vertices (because it uses the alphabet & numbers as labels)\033[0m" << endl;

            cout << "[i] Number of edges to randomly create\n(you need about 40 to reliably get a path from A-Z):";
            int edges;
            cin >> edges; cin.clear();
            verticies = graph->size();
            if (edges <= verticies * verticies) {
                for (int i=0; i<edges; i++)
                    graph->addEdge(alphabet[rand() % verticies], alphabet[rand() % verticies]);
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
                if (block[i] == '\n') break;
                intext += block[i];
            }

            // add verticies
            vector<string> tokens = tokenize(intext);
            if (tokens.empty())
                cout << "\033[1;31m[X] No properly formatted verticies found (A,B,C,...) in file\033[0m" << endl;
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
                cout << "\033[1;31m[X] No properly formatted edges found (A-B,A-C,C-B,...) in file\033[0m" << endl;
            else {
                for (string& link_str: tokens) {
                    vector<string> link = tokenize(link_str, "-");
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
