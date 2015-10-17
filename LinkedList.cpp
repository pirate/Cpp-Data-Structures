/*  Nick Sweeting 2013/10/08
    Student List (OOP)
    MIT Licence

    A Linked List implemenation that is used to store a list of students.
*/

#include <stdlib.h>
#include <iostream>
#include <iomanip>			// for setprecision()
#include <cmath>
using namespace std;

// Linked List Implementation
template <class Type> class LList {
private:
    struct node {
        Type value;
        node *next;
        node *prev;
    };

    node *root;		  // Fixed start of list
    node *tail;		  // Fixed end of list
    node *conductor;  // points to each node while looping over the whole list
    node *next_item;  // points to the node after conductor (used when adding/removing)
    node *prev_item;  // points to the node before conductor (used when adding/removing)
    int list_size;	  // number of nodes in the list, always kept up-to-date

public:
    LList() {list_size = 0;}
    ~LList() {};

    Type& operator[](const int location) throw (const char *) {
        // negative indecies work like they do in python
        // [  0,  1,  2,  3,  4,  5,  6,  7,  8,  9] ==
        // [-10, -9, -8, -7, -6, -5, -4, -3, -2, -1]
        if (location > list_size-1 || location < 0-list_size) throw "Invalid array access, index > array_size";

        // SPECIAL CASE: fetch root
        if (location == 0 || location == -1-list_size) return root->value;
        // SPECIAL CASE: fetch tail
        else if (location == -1 || location == list_size-1) return tail->value;
        // fetch using positive index
        else if (location > 0) {
        	conductor = root;
        	int index = 0;
            while (conductor->next != 0) {
                conductor = conductor->next;
                index++;
                if (index == location) return conductor->value;
            }
        }
        // fetch using negative index
        else if (location < 0) {
        	conductor = tail;
        	int index = -1;
        	while (conductor->prev != 0) {
                conductor = conductor->prev;
                index--;
                if (index == location) return conductor->value;
            }
        }
        throw "Error in code: node contains invalid value (happens when node == 0).";
    }

    void add(int location, Type item) throw (const char *) {
        // SPECIAL CASE: change of root
        if (location == 0) {
            if (list_size == 0) {
            	root = new node;
            	tail = root;
            }
            else {
            	root->prev = new node;
            	root->prev->next = root;
            	root = root->prev;
            }
            root->value = item;
            root->prev = 0;
            list_size++;
        }
        // SPECIAL CASE: change of tail
        else if (location == -1 || location == list_size) {
        	if (list_size == 0) {
            	root = new node;
            	tail = root;
            }
            else {
            	tail->next = new node;
            	tail->next->prev = tail;
            	tail = tail->next;
            }
        	tail->value = item;
        	tail->next = 0;
        	list_size++;
        }
        // POSITIVE INDEX
        else if (location > 0 && location < list_size) {
        	// ins x at idx 2
        	// [ 0,  1,  2]   -->   [ 0,  1,  2,  3]
        	// [ a,  b,  c]   -->   [ a,  b,  x,  c]
        	int index = 0;
            conductor = root;
            // stops one before the target index (in order to fetch mylist[inx]->prev)
            while (index < list_size+1) {
            	if (index+1 == location) {
            		next_item = conductor->next;
            		prev_item = conductor;

            		conductor->next = new node;
            		conductor = conductor->next;
            		conductor->next = next_item;
            		conductor->prev = prev_item;
            		conductor->prev->next = conductor;
            		conductor->next->prev = conductor;
            		conductor->value = item;
            		list_size++;
                    break;
            	}
                conductor = conductor->next;
                index++;
            }
        }
        // NEGATIVE INDEX
        else if (location < 0 && location > -1-list_size) {
        	int index = -1;
        	conductor = tail;
        	// ins x at idx -2
        	// [ 0,  1,  2]   -->   [ 0,  1,  2,  3]
        	// [-3, -2, -1]   -->   [-4, -3, -2, -1]
        	// [ a,  b,  c]   -->   [ a,  b,  x,  c]
        	// stops one before the target index (in order to fetch mylist[inx]->next)
            while (index > -2-list_size) {
            	if (index-1 == location) {
            		prev_item = conductor->prev;
            		next_item = conductor;

            		conductor->prev = new node;
            		conductor = conductor->prev;
            		conductor->prev = prev_item;
            		conductor->next = next_item;
            		conductor->prev->next = conductor;
            		conductor->next->prev = conductor;
            		conductor->value = item;
            		list_size++;

            		break;
            	}
                conductor = conductor->prev;
                index--;
            }
        }
        else throw "Invalid access, index > array_size.";
    }

    void add(Type item) {
        // add appends to the end of the list by default
        // to add to the head, do add(0, item)
        add(-1, item);
    }

    void remove(int location) throw (const char *) {
    	// SPECIAL CASE: change of root
        if (location == 0 || location == -1-list_size) {
	    	if (list_size > 0) {
			    node *nexttmp = root->next;
				delete root;
                root = nexttmp;
	    	    list_size--;
                if (list_size > 0) root->prev = 0;
            }
	    	if (list_size < 2) {
                tail = root;
            }
        }
        // SPECIAL CASE: change of tail
        else if (location == -1 || location == list_size-1) {
			node *prevtmp = tail->prev;
			delete tail;
        	tail = prevtmp;
	    	list_size--;
	    	if (list_size > 0) tail->next = 0;
	    	else if (list_size <= 1) root = tail;
        }
        // remove item at given positive index
        else if (location > 0 && location < list_size) {
        	int index = 0;
            conductor = root; // The conductor starts at head and loops forward following node->next
            while (index < list_size) {
            	if (index+1 == location) {
            		if (index+2 < list_size) conductor->next->next->prev = conductor;
					node *tmpnext = conductor->next->next;
					delete conductor->next;
            		conductor->next = tmpnext;
            		list_size--;
            		if (index == list_size-1) tail = conductor;
            		break;
            	}
                conductor = conductor->next;
                index++;
            }
        }
        // remove item at given negative index
        else if (location < 0 && location > -1-list_size) {
        	int index = -1;
        	conductor = tail; // The conductor starts at tail and loops back following node->prev
            while (index > -1-list_size) {
            	if (index-1 == location) {
            		if (index-1 > 0-list_size) conductor->prev->prev->next = conductor;
					node *tmpprev = conductor->prev->prev;
					delete conductor->prev;
            		conductor->prev = tmpprev;
            		list_size--;
            		if (index == 0-list_size) root = conductor;
            		break;
            	}
                conductor = conductor->prev;
                index--;
            }
        }
        else throw "Invalid array access, index > array_size";
    }

    int find(Type item) {
    	int index = 0;
        conductor = root; // start at head and loop through node->next towards tail
        while (conductor != 0) {
        	if (conductor->value == item) return index;
            conductor = conductor->next;
            index++;
        }
        return -1;
    }

    int reverse_find(Type item) {
    	int index = -1;
        conductor = tail; // start at tail and loop through node->prev towards head
        while (conductor != 0) {
        	if (conductor->value == item) return index;
            conductor = conductor->prev;
            index--;
        }
        return 1;
    }

    int size() {return list_size;}
};

struct Student {
    string firstName;
    string lastName;
    int studentID;
    double GPA;
};

void printStudents(LList<Student> students) {
    for (int i=0;i<students.size();i++) {
        cout << students[i].firstName << " " << students[i].lastName << ", " << students[i].studentID << ", " << setprecision(3) << students[i].GPA << endl;
    }
};

void addStudent(LList<Student> *students) {
    Student newStudent;

    cout << "First Name: ";
    cin >> newStudent.firstName;
    cout << "Last Name: ";
    cin >> newStudent.lastName;
	
	bool IDIsValid = false;
	while (!IDIsValid) {
		cout << "ID: ";
		cin >> newStudent.studentID;
		IDIsValid = true;
	
		for (int a=0; a < students->size(); a++) {
			if ((*students)[a].studentID == newStudent.studentID) {
				cout << "Student ID must be unique." << endl;
				IDIsValid = false;
			}
		}
	}
	
    cout << "GPA: ";
    cin >> newStudent.GPA;

	bool added = false;
	if (students->size() > 0) {
		for (int a=0; a<(students->size()); a++) {
			if (newStudent.studentID < (*students)[a].studentID) {
				students->add(a, newStudent);  // inserts it at that position, shifting old value right (to [a+1])
                added = true;
				break;
			} 
		}
	}
    if (!added) students->add(newStudent);
}

void delStudent(LList<Student> *students) {
    int studentIDtoDel;
    cout << "ID of student to delete: ";
    cin >> studentIDtoDel;

    for (int a=0; a < students->size(); a++) {
        if ((*students)[a].studentID == studentIDtoDel) {
            students->remove(a);
			break;
        }
    }
}

int main() {
    LList<Student> students;
    string input;
	cout << "Commands: [add, delete, print, average, quit]" << endl;

    while (true) {
        cout << ">";
        cin >> input;

        if (input == "ADD" || input == "a" || input == "add") addStudent(&students);
        else if (input == "PRINT" || input == "p" || input == "print") printStudents(students);
        else if (input == "DELETE" || input == "d" || input == "delete") delStudent(&students);
		else if (input == "AVERAGE" || input == "avg" || input == "average") {
            double average = 0;
			for (int a=0; a < students.size(); a++) average += students[a].GPA;
			average = average/students.size();
			cout << "Average GPA of all students: " << setprecision(3) << average << endl;
        }
        else if (input == "QUIT" || input == "q" || input == "quit") return 0;
    }
}

