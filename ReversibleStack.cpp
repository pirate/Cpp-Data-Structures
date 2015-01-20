#include <cstddef> // needed to use NULL
 
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
