/*  Nick Sweeting 2014/02/03
    Infix Translator
    MIT License
*/

// #include ".//.//.//.//infix.cpp"                                     // uncomment this, I dare you
// #include "infix.cpp"
#include <stdlib.h>
#include <ctype.h>      // isdigit
#include <string>
#include <stack>        // stack of tokens
#include <vector>       // vector of tokenized input
#include <iostream>     // cout
#include <sstream>      // istringstream
#include <math.h>       // pow
using namespace std;

const string ops = "+-/*()^";
const string nums = "0123456789";

// binary tree node to hold tokens and their arguments
struct node {
    string value;
    node *left;
    node *right;

    node(string val) {
        value = val;
        left = 0;
        right = 0;
    }
};

// separate string "2+45(3-45*(2+3))" into tokens ["2", "+", "45", "*", "(", ...] and remove extra ()'s
vector<string> tokenize(string input) {
    // input = (2+45-34)+156-(2+5)
    // returns [(] [2] [+] [45] [-] [34] [)] [+] [156] [-] [(] [2] [+] [5] [)]
    vector<string> tokens;

    int opens = 0;
    int closs = 0;
    for (int i=0; i<input.length(); i++) {
        if (input[i] == '(') opens++;
        else if (input[i] == ')') closs++;
    }
    if (opens != closs) {
        cout << "Error: invalid expression, parentheses mismatch" << endl;
        return tokens;
    }

    tokens.push_back("(");
    size_t prev = 0, pos;
    // find next occurence of delimiter after prev
    while ((pos = input.find_first_of(ops, prev)) != string::npos) {
        if (pos > prev)
            tokens.push_back(input.substr(prev, pos-prev));
        tokens.push_back(input.substr(pos, 1));
        prev = pos+1;
    }
    // if theres trailing info after the last delimiter
    if (prev < input.length())
        tokens.push_back(input.substr(prev, string::npos));

    tokens.push_back(")");
    return tokens;
}

// look for y chars in x, if found return true
bool is(string x, string y) {
    if (x.find_first_of(y) != string::npos)
        return true;
    else
        return false;
}

string prefix(node *token) {
    if (token->right && token->left)
        return "(" + token->value + " " + prefix(token->left) + " " + prefix(token->right) + ")";
    else
        return token->value;
}

string postfix(node *token) {
    if (token->right && token->left)
        return "(" + postfix(token->left) + " " + postfix(token->right) + " " + token->value + ")";
    else
        return token->value;
}

int stoint(string input) {
    int i = 0;
    istringstream(string(input)) >> i;
    return i;
}

int evaluate(node *token) {
    if (token->right && token->left) {
        if (token->value == "+")
            return (evaluate(token->left))+(evaluate(token->right));
        else if (token->value == "-")
            return (evaluate(token->left))-(evaluate(token->right));
        else if (token->value == "*")
            return (evaluate(token->left))*(evaluate(token->right));
        else if (token->value == "/")
            return (evaluate(token->left))/(evaluate(token->right));
        else if (token->value == "^")
            return (evaluate(token->left))*(evaluate(token->right));
    }
    return stoint(token->value);
}

int main () {
    string infix, prepost;
    cout << "[i] Supported operators:   " << ops << "    each operator may have 2 and ONLY 2 arguments. Do not add extra ()s." << endl;
    cout << "[1] Input an expression in infix notation: " << endl;
    cin >> infix;
    cin.clear();

    vector<string> tokens = tokenize(infix);
    if (!(tokens.size()))
        return 1;

    stack<node *> stk;

    for (int i=0; i<tokens.size(); i++) {
        // Make current token into a node.
        node *token = new node(tokens[i]);

        // If token is “(“ push node
        if (token->value == "(")
            stk.push(token);
        else if (token->value == ")"){
            node *cur = stk.top(); stk.pop();
            node *top = stk.top(); stk.pop();
            while (top->value != "(") {
                top->right = cur;
                cur = top;
                top = stk.top(); stk.pop();
                stk.push(cur);
            }
        }
        else if (is(token->value, nums)) {
            if (is(stk.top()->value, ops)) {
                node *op = stk.top(); stk.pop();
                op->right = token;
                stk.push(op);
            }
            stk.push(token);
        }
        else if (!is(token->value, "()") && is(token->value, ops)) {
        // If token is an operator
        //   top=pop, make top left child of node, push node
            node *left = stk.top(); stk.pop();
            token->left = left;
            stk.push(token);
        }
        else {
            cout << "ERROR, weird token value: " << token->value << endl;
        }
    }

    cout << "[2] What format do you want your output? (pre/post):";
    cin >> prepost;
    cin.clear();

    if (prepost == "pre" || prepost == "p") {
        node *r = stk.top();
        cout << prefix(stk.top()) << " = " << evaluate(stk.top()) << endl;
    }
    else if (prepost == "post" || prepost == "rpn") {
        node *r = stk.top();
        cout << postfix(stk.top()) << " = " << evaluate(stk.top()) << endl;
    }
    else {
        cout << "[X] Invalid choice, please specify 'pre' or 'post' or 'rpn'." << endl;
        return 1;
    }

    return 0;
}



// INFIX -> POSTFIX

// For each token {
//     If (token is a number) {
//         Add number to the output queue
//     }
//
//     If (token is an operator eg +,-,*...) {
//         While (stk not empty AND stk top element is an operator) {
//             If ((token = left associative AND precedence <= stk top element) OR (token = right associative AND precedence < stk top element)) {
//                  Pop stk onto the output queue.
//                  Exit while loop.
//             }
//         }
//         Push token onto stk
//     }
//
//     If (token is left bracket '(') {
//         Push token on to stk
//     }
//
//     If (token is right bracket ')') {
//         While (stk not empty AND stk top element not a left bracket) {
//             Pop the stk onto output queue
//         }
//         Pop the stk
//     }
// }
//
// While (stk not empty) {
//     Pop stk onto output queue
// }

// EVAL RPN
// For each token  {
//     If (token is a number) {
//         Push value onto stk
//     }

//     If (token is an operator) {
//         Pop 2 top values from the stk
//         Evaluate operator using popped values as args
//         Push result onto stk
//     }
// }
