/* 	Nick Sweeting 2013/09/09
	Tic Tac Toe in C++ (non OOP)
	MIT Liscense
*/

#include <stdlib.h>
#include <iostream>
using namespace std;

template <class Type> class LList {
private:
    struct node {
        Type value;
        node *next;
    };

    node *root;       // This won't change, or we would lose the list in memory
    node *conductor;  // This will point to each node as it traverses the list
    node *next_item;
    int size;

public:
    LList() {size = 0;}
    ~LList() {};

    Type& operator[](const int location) throw (const char *);

    void add(Type item) {
        if (size == 0) {
            root = new node;  // Sets it to actually point to something
            root->next = 0;
            root->value = item;
            size = 1;
            cout << "Added item " << item << " to list. size: " << size << endl;
        }
        else {
            conductor = root; // The conductor points to the first node
            if ( conductor != 0 ) {
                while ( conductor->next != 0) {
                    conductor = conductor->next;
                }
            }
            conductor->next = new node;  // Creates a node at the end of the list
            conductor = conductor->next; // Points to that node
            conductor->next = 0;         // Prevents it from going any further
            conductor->value = item;
            size++;
            cout << "Added item " << item << " to list. size: " << size << endl;
        }
    }

    // void add(int location, Type item) {
    //     int index = 0;
    //     conductor = root; // The conductor points to the first node
    //     if (conductor != 0 && location != 0) {
    //         while (conductor->next != 0) {
    //             if (index == location) {
    //                 // orig: {..., [item5 -> item6], [item6 -> item7], [item7 -> item8], ...}
    //                 // add+: {..., [item5 -> item112], [item6 -> item7], [item7 -> item8], ...[item 112 -> item6]}
    //                 next_item = conductor->next;
    //                 conductor->next = new node;  // Creates a node at the end of the list
    //                 conductor = conductor->next; // Points to that node
    //                 conductor->next = next_item;
    //                 size++;
    //                 conductor->value = item;
    //             }
    //             conductor = conductor->next;
    //             index++;
    //         }
    //     }
    //     next_item = conductor->next; // even if conductor->next is 0, this will work fine
    //     conductor->next = new node;  // Creates a node at the end of the list
    //     conductor = conductor->next; // Points to that node
    //     conductor->next = next_item;
    //     size++;
    //     conductor->value = item;
    //     cout << "Added item " << item << " to list. size: " << size << endl;
    // }

    Type find(Type item) {
        conductor = root; // The conductor points to the first node
        if ( conductor != 0 ) {
          while ( conductor->next != 0)
            if (conductor->value == item) return item;
            conductor = conductor->next;
        }
    }

    int size_of() {
        return size;
    }

};

template <class Type> Type& LList<Type>::operator[](const int location) throw (const char *) {
    int index = 0;
    if (location < 0 || location > size-1)
        throw "Invalid array access, index < 0 or index > array_size";
    else
        conductor = root; // The conductor points to the first node
        if (location == 0) return conductor->value;
        else if (conductor != 0) {
            while (conductor->next != 0) {
                conductor = conductor->next;
                index++;
                if (index == location) return conductor->value;
            }
        }
        throw "Invalid array access, index > array_size.";
}

int main() {
    try {
        LList<int> mylist;
        mylist.add(10);
        mylist.add(11);
        mylist.add(12);
        cout << "Value at mylist[0]:" << mylist[0] << endl;
        cout << "Value at mylist[1]:" << mylist[1] << endl;
        cout << "Value at mylist[2]:" << mylist[2] << endl;
    }
    catch (const char* e) {
        cout << e << endl;
    }
}
