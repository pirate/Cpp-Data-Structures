/* 	Nick Sweeting 2013/09/09
	Tic Tac Toe in C++ (non OOP)
	MIT Liscense
*/

#include <stdlib.h>
#include <iostream>

using namespace std;

enum square {EMPTY=' ', O='O', X='X'};											// define a new enum "square" with 3 possible values (used for array board[])

square board[9] = {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY};		// initialize the board with all empty squares
//square board[9] = {X,O,O,O,X,O,X,O,X};										// initialize the board with X winning		(debug only)
//square board[9] = {O,O,X,O,O,X,X,X,O};										// initialize the board with O winning		(debug only)
//square board[9] = {X,O,X,O,O,X,X,X,O};										// initialize the board with a tie			(debug only)

int solutions[8][3] = {{0, 3, 6}, // vertical
                       {1, 4, 7},
                       {2, 5, 8},
                       {0, 1, 2}, // horizontal
                       {3, 4, 5},
                       {6, 7, 8},
                       {0, 4, 8}, // diagonal
                       {2, 4, 6}};

// print user help board showing square numbering
void print_help() {
    cout << 1 << '|' << 2 << '|' << 3 << endl;         //  X|O|X
    cout << 4 << '|' << 5 << '|' << 6 << endl;         //  O|X|O
    cout << 7 << '|' << 8 << '|' << 9 << endl;         //  X|O|X
}

// print current board highlighting last human move and last computer move (sorry for line noise, ansi is ugly)
void print_board(square board[9], int recent_human_move, int recent_computer_move) {
	string tmp_board[9];
    int i;
    for (i=0; i<9; i++) {
    	if (i == recent_human_move) tmp_board[i] = "\033[1;31m" + string(1,board[i]) + "\033[0m";			// red for human's last move (using ASCII escape codes)
    	else if (i == recent_computer_move) tmp_board[i] = "\033[1;33m" + string(1,board[i]) + "\033[0m";	// yellow for computer's move
    	else tmp_board[i] = string(1, board[i]);
    }
    cout << tmp_board[0] << '|' << tmp_board[1] << '|' << tmp_board[2] << endl;			//  X|O|X
    cout << tmp_board[3] << '|' << tmp_board[4] << '|' << tmp_board[5] << endl;			//	O|X|O
    cout << tmp_board[6] << '|' << tmp_board[7] << '|' << tmp_board[8] << endl;			//	X|O|X
}

// check to see if the game is over, return an int status
int check_for_winner(square board[9], int solutions[8][3]) {			// returns int winner X=0, O=1, tie=2, unfinished=-1
	// check for win by a player
	int i, index1, index2, index3;
    for (i=0; i<8; i++){
        index1 = solutions[i][0];
        index2 = solutions[i][1];
        index3 = solutions[i][2];
        if (board[index1] == board[index2] && board[index2] == board[index3] && board[index1] != EMPTY) {
        	if (board[index1] == X) return 0;
        	else if (board[index1] == O) return 1;
        }
    }

	// check for a tie
	int b;
    for (b=0; b<9; b++) if (board[b] == EMPTY) return -1;	// return -1 if any squares are empty (game unfinished)

    // if no winner is found, and squares are not empty, return tie
    return 2;
}

// check to see if move is allowed by checking to see if spot is empty or taken, returns bool (valid/invalid = true/false)
bool check_move_validity(square board[9], int move, square symbol) {
	if (symbol == EMPTY) return false;					// placing an EMPTY is always invalid
	else if (board[move] != EMPTY) return false;		// placing your move where one already exists is always invalid
	else return true;
}

// checks move validity, and places the move if valid then returns the int move, if invalid returns -1
int place_move(square board[9], int move, square symbol) {
	if (check_move_validity(board, move, symbol)) {
		board[move] = symbol;
		return move;
	}
	else return -1;
}

// ask the human player for input, check for validity. DOES NOT PLACE THE MOVE, just returns int move if its valid
int get_player_move() {
    int input_square = -1;
    while (input_square != -1) {
        cout << "Which square do you pick: ";
        cin >> input_square;
        cin.clear();                                                    // will cause infinite loop unless input buffer is cleared between inputs
        cin.ignore(numeric_limits<streamsize>::max(), '\n');            // must also ignore newlines from previous input

        if (input_square < 1 || input_square > 9) {
            cout << "\nYou idiot, type numbers between 1-9 only.\n";
            usleep(1000*1000);
            cout<< endl;
        }
        else return input_square-1;
    }
}

// asks the human for input by calling get_player_move, then places the move if its valid
int player_move(square board[9], square human_symbol) {					// returns the move made or -1 if invalid
	int move;
	move = get_player_move();
	int result;
	result = place_move(board, move, human_symbol);
	if (result == -1) {
		cout << "\nPlacing an " << string(1, human_symbol) << " at " << move+1 << " is invalid.\n";
		return player_move(board, human_symbol);
	}
	else return result;
}

// AI Functions  (alpha-beta pruning algorithm (minimax), big O worst case: O(|E|+|V|) -- depth-first)

// dumb AI for gameplay demonstration and debugging when minimax_ai is broken
int fallback_ai(square board[9], square computer_symbol) {
    int sorted_board[9] = {4,0,2,6,8,1,3,5,7};                     // play center, then corners, then everything else
    int i;
    for (i=0;i<9;i++) {
        if (board[sorted_board[i]] == EMPTY) {
            place_move(board, sorted_board[i], computer_symbol);
            return sorted_board[i];
        }
    }
}

// returns +1, 0, or -1 for scoring of the move to determine whether it helps the computer, human, or neither
int judge_move(int winner, square current_player, square computer_symbol) {
	int player;
    if (winner == 2) return 0;      // handle ties straight off the bat

    if (current_player == X) player = 0;
    else if (current_player == O) player = 1;

    int score;
	if (winner == player) score = 1;
	else if (winner != player) score = -1;
    // flip score if computer is not player who placed move
    if (computer_symbol != current_player) {
        if (score == 1) return -1;
        else if (score == -1) return 1;
    }
    else return score;
}

// calculates which moves are viable by seeing which squares are EMPTY, returns array of bools viable_squares[]
bool* calc_viable(square board[9]) {
	bool *viable_squares;
    viable_squares = new bool [9];

	int i, a = 0;
	for (i=0;i<9;i++) {
		if (board[i] == EMPTY) viable_squares[i] = true;
        else viable_squares[i] = false;
	}
	return viable_squares;
}

// the recursor function that plays out every possible game and calulates scores for each, places scores in the score[] int array
void minimax_recursor(square *tmp_board, int solutions[8][3], int score[9], bool viable_squares[9], square current_player, square computer_symbol) {
	int m;
	for (m=0;m<9;m++) {
		if (viable_squares[m] == true) {
            tmp_board[m] = current_player;  // place actual move on temporary board
            int winner;
            winner = check_for_winner(tmp_board, solutions);
            if (winner != -1) {
                //cout << char(current_player) << m+1 << " ends the game with " << winner << " as winner." << endl;
                score[m] += judge_move(winner, current_player, computer_symbol);
                cout << "Score[" << m+1 << "] += " << judge_move(winner, current_player, computer_symbol) << endl;
                //print_board(tmp_board, -1, m);
                tmp_board[m] = EMPTY;      // undo move so simulation can continue
                if (current_player == X) current_player = O; else current_player = X;

            }
            else {
                // flip flop the players
                if (current_player == X) current_player = O; else current_player = X;

                //cout << "Begin recurse for " << m+1 << endl;
                bool *viable_squares = calc_viable(tmp_board);
                minimax_recursor(tmp_board, solutions, score, viable_squares, current_player, computer_symbol);

            }
            tmp_board[m] = EMPTY;      // undo move so simulation can continue
            // flip flop the players
            if (current_player == X) current_player = O; else current_player = X;
            *viable_squares = calc_viable(tmp_board);
        }
    }
}

// starts the minimax_recursor with a starting set of viable_moves, chooses the move with the best score and returns int move_made
int ai_move(square board[9], square computer_symbol) {
	// copy board into tmp_board to prevent direct modification during ranking
	square tmp_board[] = {EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY};
	int a;
	for (a=0;a<9;a++) tmp_board[a] = board[a];

    cout << "starting AI" << endl;
    print_board(tmp_board, -1, -1);

	// score all the possible moves recursively
	int score[9] = {0,0,0,0,0,0,0,0,0};
    bool *viable_squares = calc_viable(tmp_board);
	minimax_recursor(tmp_board, solutions, score, viable_squares, computer_symbol, computer_symbol);

    int best_score[2] = {-1,100000000};  // position, score
	int y;
	for (y=0;y<9;y++) {
		cout << "Position: " << y+1 << ". Score: " << score[y] << endl;
		if (score[y] < best_score[1]) {
            best_score[0] = y;
            best_score[1] = score[y];
        }
	}
    if (best_score[0] == -1) {
        cout << "Computer found no ideal move." << endl;
        return place_move(board, fallback_ai(board, computer_symbol), computer_symbol);
    }
    else {
        cout << "Computer chose: " << best_score[0] << endl;
        int result;
        result = place_move(board, best_score[0], computer_symbol);
        if (result == -1) {
            cout << "\nPlacing an " << string(1, computer_symbol) << " at " << best_score[0]+1 << " is invalid.\n";
            return place_move(board, fallback_ai(board, computer_symbol), computer_symbol);
        }
        else return result;
    }



}

int main () {
    // ask user to pick which symbol they want to use
	cout << "Choose your weapon (X/O): ";
	char tmp_human_symbol;
	cin >> tmp_human_symbol;
	square human_symbol, computer_symbol;
	if (tmp_human_symbol == 'x' || tmp_human_symbol == 'X') {
		human_symbol = X;
		computer_symbol = O;
	}
	else {
		human_symbol = O;
		computer_symbol = X;
	}

	int recent_human_move, recent_computer_move = -1;
	print_help();

	// Game loop
	while (check_for_winner(board, solutions) == -1) {
        recent_human_move = player_move(board, human_symbol);
        if (check_for_winner(board, solutions) != -1) break;
        recent_computer_move = ai_move(board, computer_symbol);                        // change minimax_ai to fallback_ai to test game without minimax
        if (check_for_winner(board, solutions) != -1) break;
        print_board(board, recent_human_move, recent_computer_move);
	}

	cout << endl;

    // Endgame Text
	print_board(board, -1, -1);                                                            // print ending state of the board with no recent moves highlighted
	string winner;
	int end_result;
	end_result = check_for_winner(board, solutions);                                       // its a tad innefficient to keep calling this every time, who cares, its only an extra 15ms
	if (human_symbol == X && end_result == 0) winner = "You won. (X)";
	else if (human_symbol == O && end_result == 1) winner = "You won. (O)";
	else if (computer_symbol == X && end_result == 0) winner = "Computer won. (X)";
	else if (computer_symbol == O && end_result == 1) winner = "Computer won. (O)";
    else winner = "It was a tie.";

	cout << "\nGame is over. " << winner << endl;

	return 0;
}
