#include <cstddef> // needed to use NULL
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class ReversibleStack {
    private:
        struct Node {
            int data;
            Node* next;
        };

    public:
        Node* top;  // making this public is not ideal, but makes it much easier to Reverse if we can get the top pointer directly

        ReversibleStack() {
            top = NULL;
        }

        ~ReversibleStack(){
            Empty();
        }
        
        void Push(int newval) {
            Node *tmp = new Node;                   // create new node tmp

            tmp->data = newval;                     // set new node value to x
            tmp->next = top;                        // points to old top of the ReversibleStack
            top = tmp;                              // now the top
        }

        int Pop() {
            if (IsEmpty()) return NULL;             // if the list is empty, return NULL (this is very bad, we should return int*=NULL NOT int=NULL)
            int topVal = top->data;
            
            Node* oldTop = top;
            top = top->next;
            delete oldTop;
            
            return topVal;
        }
        
        int Top() {
            if (IsEmpty()) return NULL;
            return top->data;
        }

        void Reverse() {
            ReversibleStack* newStack = new ReversibleStack();  // have to use new to allocate new memory, otherwise it gets deleted when the function ends
            while (!IsEmpty()) newStack->Push(Pop());
            Node* oldTop = top;
            top = newStack->top;  // point top to the top of the new, reversed stack
            delete oldTop;        // delete the old top which is now pointing to an empty stack
        }

        void Empty() {
            while (!IsEmpty()) Pop();
        }
        
        int IsEmpty() {
            return (top == NULL);    // checks if ReversibleStack is empty
        }   
};

void Display(ReversibleStack& stack) {
    // Implement this function (use only Push, Pop and IsEmpty member functions of stack)
    // After the function completes, the stack should be unmodified (see assignment instructions)

    // we make a new stack, the loop through the existing ones, popping off elements and putting them on the new stack
    // then we go through the new stack and pop them all off and put them back on the original one
    ReversibleStack newStack;
    while (!stack.IsEmpty()) {
        int value = stack.Pop();
        cout << value << (stack.IsEmpty() ? "" :",");
        newStack.Push(value);
    }
    while(!newStack.IsEmpty()) {
        stack.Push(newStack.Pop());
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Missing required argument for input file." << endl;
        return 1;                                                       // return nonzero for failure
    }

    ifstream inFile;
    inFile.open(argv[1], ifstream::in);

    if (!inFile.is_open()) {
        cout << "Could not open: " << argv[1] << endl;
        return 2;
    }

    ReversibleStack rs;
    string line;

    // Process the command on each line
    while (!inFile.eof()) {
        getline(inFile, line);
        
        if (line == "") {
            // do nothing, it's an empty line
        }
        else if (line.compare(0, 6, "header") == 0) {
            cout << "Name: Eric Chen ID: 11381898";                     // Display the header line here, as the instructions describe
        }
        else if (line.compare(0, 4, "push") == 0) {
            int val = stoi(line.substr(5));
            rs.Push(val);
            cout << "pushed: " << val;
        }
        else if (line.compare(0, 3, "pop") == 0) {
            cout << "popped: " << rs.Pop();                             // pop and print the top item
        }
        else if (line.compare(0, 7, "isempty") == 0) {
            cout << "isempty: " << (rs.IsEmpty() ? "true": "false");    // print true if empty, otherwise false
        }
        else if (line.compare(0, 7, "reverse") == 0) {
            rs.Reverse();
            cout << "reversed";
        }
        else if (line.compare(0, 7, "display") == 0) {
            cout << "display: ";
            Display(rs);
        }
        else {
            cout << "Unknown command: " << line;
        }
        cout << endl;
    }

    inFile.close();
    cout << "Done" << endl;
    return 0;                                                           // return 0 for success
}
