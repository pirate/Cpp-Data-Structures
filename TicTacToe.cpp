/*  Nick Sweeting 2013/09/09
    Tic Tac Toe in C++ (non OOP)
    MIT Liscense
    g++ TicTacToe.cpp -o main && ./main

    The TicTacToe game.  Human vs (buggy) minimax-based AI.
*/

#include <stdlib.h>
#include <iostream>
using namespace std;

enum square {EMPTY=' ', O='O', X='X'};                                          // define a new enum "square" with 3 possible values (used for array board[])

square board[9] = {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY};      // initialize the board with all empty squares

bool allow_skip = true;                             // allow the human to skip their turn and have the computer play itself (debug only)

// print user help board showing square numbering
void print_help() {
    cout << 1 << '|' << 2 << '|' << 3 << endl;         //  1|2|3
    cout << 4 << '|' << 5 << '|' << 6 << endl;         //  4|5|6
    cout << 7 << '|' << 8 << '|' << 9 << endl;         //  7|8|9
}

// print current board highlighting last human move and last computer move (sorry for line noise, ansi escapes are ugly)
void print_board(square board[9], int recent_human_move, int recent_computer_move) {
    string tmp_board[9];
    for (int i=0; i<9; i++) {
        if (i == recent_human_move) tmp_board[i] = "\033[1;31m" + string(1,board[i]) + "\033[0m";           // red for human's last move (using ASCII escape codes)
        else if (i == recent_computer_move) tmp_board[i] = "\033[1;33m" + string(1,board[i]) + "\033[0m";   // yellow for computer's move
        else tmp_board[i] = string(1, board[i]);
    }
    cout << tmp_board[0] << '|' << tmp_board[1] << '|' << tmp_board[2] << endl;         //  X|O|X
    cout << tmp_board[3] << '|' << tmp_board[4] << '|' << tmp_board[5] << endl;         //  O|X|O
    cout << tmp_board[6] << '|' << tmp_board[7] << '|' << tmp_board[8] << endl;         //  X|O|X
}

// check to see if the game is over, return an int status
int check_for_winner(square board[9]) {         // returns int winner X=0, O=1, tie=2, unfinished=-1

    int solutions[8][3] = {{0, 3, 6}, // vertical solutions
                           {1, 4, 7},
                           {2, 5, 8},
                           {0, 1, 2}, // horizontal solutions
                           {3, 4, 5},
                           {6, 7, 8},
                           {0, 4, 8}, // diagonal solutions
                           {2, 4, 6}};

    // check for win by a player
    int index1, index2, index3;
    for (int i=0; i<8; i++){
        index1 = solutions[i][0];
        index2 = solutions[i][1];
        index3 = solutions[i][2];
        if (board[index1] == board[index2] &&
            board[index2] == board[index3] &&
            board[index1] != EMPTY) {
            return board[index1] == X ? 0 : 1;
        }
    }

    // check for a tie
    for (int b=0; b<9; b++)
        if (board[b] == EMPTY)
            return -1;  // return -1 if any squares are empty (game unfinished)

    // if no winner is found, and squares are not empty, return tie
    return 2;
}

// checks move validity, and places the move if valid then returns the int move, if invalid returns -1
int place_move(square board[9], int move, square player_symbol) {
    if (player_symbol == EMPTY) return -1;                              // placing an EMPTY is always invalid
    else if (board[move] != EMPTY) return -1;                           // placing your move where one already exists is always invalid
    else {
        board[move] = player_symbol;
        return move;
    }
}

// asks the human for input, then places the move if its valid
int human_move(square board[9], square human_symbol) {                  // returns the move made or -1 if invalid
    int move = -1;
    int input_square = -1;
    while (input_square == -1) {
        cout << "Which square do you pick: ";
        cin >> input_square;
        cin.clear();                                                    // will cause infinite loop unless input buffer is cleared between inputs

        if (input_square == 0 && allow_skip)
            return 0;

        if (input_square < 1 || input_square > 9) {
            cout << "\nYou idiot, type numbers between 1-9 only.\n";
            input_square = -1;
        }
        else
            move = input_square-1;
    }

    int final_move = -1;
    final_move = place_move(board, move, human_symbol);
    if (final_move == -1) {
        cout << "\nPlacing an " << string(1, human_symbol) << " at " << move+1 << " is invalid.\n";       // if spot is taken
        return human_move(board, human_symbol);
    }
    else
        return final_move;
}

/* AI Functions:
    Alpha-Beta Pruning algorithm (Minimax)
    Big O worst case: O(|E|+|V|) -- type: depth-first
*/

// dumb AI for gameplay demonstration and debugging when minimax_ai is broken
int fallback_ai(square board[9]) {
    int sorted_board[9] = {4,0,2,6,8,1,3,5,7};                     // play center, then corners, then everything else
    for (int i=0;i<9;i++)
        if (board[sorted_board[i]] == EMPTY)
            return sorted_board[i];
    return 4;
}

// returns +1, 0, or -1 for scoring of the move to determine whether it helps the computer, human, or neither
int judge_move(int winner, square current_player, square computer_symbol) {
    if (winner == 2) return 0;      // handle ties straight off the bat

    // convert player symbol to int for easier processing X=0, O=1
    int player = current_player == X ? 0 : 1;
    int score = winner == player ? 1 : -1;

    // flip score if computer is not player who placed move
    if (computer_symbol != current_player)
        score = 0 - score;
    return score;
}

// calculates which moves are viable by seeing which squares are EMPTY, returns array of bools viable_squares[]
bool* calc_viable(square board[9]) {
    bool *viable_squares;
    viable_squares = new bool [9];

    for (int i=0;i<9;i++)
        viable_squares[i] = board[i] == EMPTY;

    return viable_squares;
}

// the recursor function that plays out every possible game and calulates scores for each, places scores in the score[] int array
void minimax_recursor(square *tmp_board, int score[9], square current_player, square computer_symbol) {
    bool *viable_squares = calc_viable(tmp_board);
    for (int m=0;m<9;m++) {
        if (viable_squares[m] == true) {
            tmp_board[m] = current_player;  // place actual move on temporary board
            int winner = check_for_winner(tmp_board);
            if (winner == -1) {
                //cout << "Begin recurse for " << m+1 << endl;

                // flip flop the players
                current_player = current_player == X ? O : X;
                minimax_recursor(tmp_board, score, current_player, computer_symbol);
                tmp_board[m] = EMPTY;      // undo move so simulation can continue
                current_player = current_player == X ? O : X;
            }
            else {
                //cout << "Score[" << m+1 << "] += " << judge_move(winner, current_player, computer_symbol) << endl;
                //print_board(tmp_board, -1, m);
                //score[m] += judge_move(winner, current_player, computer_symbol);
                int square_score = judge_move(winner, current_player, computer_symbol);
                if (score[m] >= 0 && square_score > 0) {
                    score[m] += square_score;
                }
                else if (score[m] <= 0 && square_score < 0) {
                    score[m] -= square_score;
                }
                tmp_board[m] = EMPTY;      // undo move so simulation can continue
            }
        }
    }
}

// starts the minimax_recursor with a starting set of viable_moves, chooses the move with the best score and returns int move_made
int ai_move(square board[9], square computer_symbol) {
    // copy board into tmp_board to prevent direct modification during ranking
    square tmp_board[] = {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY};
    for (int a=0;a<9;a++) tmp_board[a] = board[a];

    cout << "starting AI" << endl;
    print_board(tmp_board, -1, -1);

    // score all the possible moves recursively
    int score[9] = {0,0,0,0,0,0,0,0,0};
    minimax_recursor(tmp_board, score, computer_symbol, computer_symbol);

    int best_score[2] = {-1,0};  // position, score
    for (int y=0;y<9;y++) {
        cout << "Position: " << y+1 << ". Score: " << score[y] << endl;
        if (score[y] < best_score[1]) {
            best_score[0] = y;
            best_score[1] = score[y];
        }
    }
    if (best_score[1] == 0) {
        for (int y=0;y<9;y++) {
            if (score[y] > best_score[1]) {
                best_score[0] = y;
                best_score[1] = score[y];
            }
        }
    }

    int result = place_move(board, best_score[0], computer_symbol);
    if (result == -1) {
        cout << "\nMimixax Error: " << string(1, computer_symbol) << " at " << best_score[0]+1 << " is invalid.\n";
        result = place_move(board, fallback_ai(board), computer_symbol);
    }
    cout << "Computer chose: " << result+1 << endl;
    return result;
}

int main () {
    // ask user to pick which symbol they want to use
    cout << "Choose your weapon (X/O): ";
    char tmp_human_symbol;
    cin >> tmp_human_symbol;
    cin.clear();                                                    // will cause infinite loop unless input buffer is cleared between inputs

    square human_symbol, computer_symbol;
    if (tmp_human_symbol == 'x' || tmp_human_symbol == 'X') {
        human_symbol = X;
        computer_symbol = O;
    }
    else {
        human_symbol = O;
        computer_symbol = X;
    }

    print_help();

    int recent_human_move, recent_computer_move = -1;

    // Game loop
    while (check_for_winner(board) == -1) {
        recent_human_move = human_move(board, human_symbol);
        if (check_for_winner(board) != -1) break;
        recent_computer_move = ai_move(board, computer_symbol);
        if (check_for_winner(board) != -1) break;
        print_board(board, recent_human_move, recent_computer_move);
    }

    // Endgame Text
    cout << endl;
    print_board(board, -1, -1);                                                     // print ending state of the board with no recent moves highlighted
    string winner;
    int end_result = check_for_winner(board);                                       // its a tad innefficient to keep calling this every time, who cares, its only an extra 15ms
    if (human_symbol == X && end_result == 0) winner = "You won. (X)";
    else if (human_symbol == O && end_result == 1) winner = "You won. (O)";
    else if (computer_symbol == X && end_result == 0) winner = "Computer won. (X)";
    else if (computer_symbol == O && end_result == 1) winner = "Computer won. (O)";
    else winner = "It was a tie.";

    cout << "\nGame is over. " << winner << endl;

    return 0;
}
