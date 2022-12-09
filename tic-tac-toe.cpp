#include <bits/stdc++.h>
#include <vector>
using namespace std;

/*
A. AI engine
	- State representation: vector<vector<int>> (3x3)
		+ X is 1, O is -1, blank is 0
	- Check if the state is ended:
		+ Input: state
		+ Output: True/False
		+ Check if row, column, diagonal have 3 X/O together. if yes, return True
		+ Check if the whole board is filledd. if yes, return True
		+ If not return False
	- Check valid moves (with player as input):
		+ Input: player's new move
		+ Output: True/False
		+ Check current players:
			+ X always plays first
			+ If the number X on the chessboard > the number O on the chessboard --> turn O
			+ If number X on the chessboard = number of O on the chessboard --> turn X
		+ If current player == human player, continue
		+ Check if the move is valid (whether filled in the box already filled in), if not then alert and let human make another move
	- calculate state score
		+ Input: state + current player
		+ Output: state score (int)
		+ Check current players
		+ Score of the final state:
			+ 1 if X wins, 0 if draw, -1 if O wins
		+ Score of non-final state
			+ Consider the scores of the states after making the next move. The score of the current state will be the highest/lowest score of the next states. (minimax algorithm)
			+ Algorithm for taking all next chess positions
			+ Score all states taken from aforementioned algorithm
			+ Find out which position has the lowest score (with O) or the highest score (with X).
	- Find the best move:
		+ Input: state + current player
		+ Output: best next position
		+ Algorithm for taking all next chess positions
		+ Calculate the score of the next chess positions
		+ Take the position with the highest/lowest score depending on X/O

2.User interface
 	- asking x or o
 	- input move cordinate start from 0
 	- response if illegal move
	- response game result when end
	- print current game
	- print move
*/

//initiate 
//each State is represent with 0 (blank), 1 (X), or -1 (O)
//characters is used inside the "show state" function
char characters[] = "-XO";
typedef vector<vector<int> > State;
const int SIZE = 3;

//1. AI engine
//Game over/Decide the outcome of the game

/*
- render game
*/
State play(State s, int row, int col, int player)
{
    State newState = State(3, vector<int>(3, 0));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            newState[i][j] = s[i][j];
        }
    }
    newState[row][col] = player;
    return newState;
}
void renderGame(State state)
{
    cout << " -------------" << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << " | " << characters[(state[i][j] + 3) % 3];
        }
        cout << " |" << endl << " -------------" << endl;
    }
}


/*
	- Check if the state is ended:
		+ Input: state
		+ Output: True/False
		+ Check if row, column, diagonal have 3 X/O together. if yes, return True
		+ Check if the whole board is filledd. if yes, return True
		+ If not return False
*/
bool isFinalState(State s){
	//three in a row/col/diagonal
	for(int i = 0; i < SIZE; i++){
		if(abs(s[0][i] + s[1][i] + s[2][i]) == 3) return true; //check row
		if(abs(s[i][0] + s[i][1] + s[i][2]) == 3) return true; //check col
	}
	if(abs(s[0][0] + s[1][1] + s[2][2]) == 3) return true;
	if(abs(s[2][0] + s[1][1] + s[0][2]) == 3) return true; //check diagonals
	// check full
   	for (int i = 0; i < SIZE; i++){
   		for(int j = 0; j < SIZE; j++){
   			if(s[i][j] == 0) return false;	
		}	
	}
	return true;
}

/*
	- Check valid moves (with player as input):
		+ Input: player's new move
		+ Output: True/False
		+ Check current players
		+ If current player == human player, continue
		+ Check if the move is valid (whether filled in the box already filled in), if not then alert and let human make another move
*/

/*

	+ Check current players:
		+ X always plays first
		+ If the number X on the chessboard > the number O on the chessboard --> turn O
		+ If number X on the chessboard = number of O on the chessboard --> turn X
*/
int getNextPlayer(State s)
{
    int countX = 0, countO = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s[i][j] == 1) countX++;
            else if (s[i][j] == -1) countO++;
        }
    }
    return countX == countO ? 1 : -1;
}

/*
	- check valid moves
*/
bool checkLegalMove(State s, int i, int j){
	renderGame(s);
	if(i <= 0 or i > SIZE or j <= 0 or j > SIZE) return false; // out of bounds
	if(s[i - 1][j - 1] != 0) return false; // already filled
	return true; 
}

/*
	- calculate state score
		+ Input: state + current player
		+ Output: state score (int)
		+ Check current players
		+ Score of the final state
		+ Score of non-final state
			+ Consider the scores of the states after making the next move. The score of the current state will be the highest/lowest score of the next states. (minimax algorithm)
			+ Algorithm for taking all next chess positions
			+ Score all states taken from aforementioned algorithm
			+ Find out which position has the lowest score (with O) or the highest score (with X).
*/


/*
	+ Score of the final state:
		+ 1 if X wins, 0 if draw, -1 if O wins
*/

int getScoreFinalState(State s)
{
    // check rows & cols
    for (int i = 0; i < SIZE; i++){
    	if(abs(s[i][0] + s[i][1] + s[i][2]) == 3) return s[i][0];
    	if(abs(s[0][i] + s[1][i] + s[2][i]) == 3) return s[0][i];
	}
    // check diagonals
    if(abs(s[0][0] + s[1][1] + s[2][2]) == 3) return s[0][0];
    if(abs(s[2][0] + s[1][1] + s[0][2]) == 3) return s[2][0];
    return 0;
}


/*
	+ Score of non-final state
		+ Consider the scores of the states after making the next move. The score of the current state will be the highest/lowest score of the next states. (minimax algorithm)
		+ Algorithm for taking all next states
		+ Score all states taken from aforementioned algorithm
		+ Find out which position has the lowest score (with O) or the highest score (with X).
*/

/*
	+ Algorithm for taking all next states
*/
vector<State> getNextStates(State s, int player)
{
    vector<State> states;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s[i][j] == 0) {
                State nextState = play(s, i, j, player);
                states.push_back(nextState);
			}
        }
    }
    return states;
}

pair<int, State> getScore(State s)
{
    if (isFinalState(s)) {
        return make_pair(getScoreFinalState(s), s);
    }
    int player = getNextPlayer(s);
    vector<State> states = getNextStates(s, player);
    int bestScore = -10;
    State bestState;
    for (State ns: states) {
        pair<int, State> p = getScore(ns);
        int score = p.first;
        if (player == 1){
        	if (bestScore < score  || bestScore == -10){
	            bestScore = score;
	            bestState = ns;
	        }
		} else {
			if (bestScore > score  || bestScore == -10){
	            bestScore = score;
	            bestState = ns;
	        }
		}
        //cout << score << "\n";
        //renderGame(ns);
    }
    
    return make_pair(bestScore, bestState);
}

pair<int, int> getComputerPlay(State s)
{
    pair<int, State> p = getScore(s);
    int score = p.first;
    State newState = p.second;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (newState[i][j] != s[i][j]) return make_pair(i, j);
        }
    }
    return make_pair(-1, -1);
}

//2. user interface
//showState
//input: board with 0, 1, -1
//output: show board with X and O
void showState(State s){
	cout << "____________________________\n";
	for(int i = 0; i < SIZE; i++){
		for(int j = 0; j < SIZE; j++){
			cout << characters[s[i][j]] << " ";
		}
		cout << "\n";
	}
	cout << "____________________________\n";
}


int main()
{
    State s = State(3, vector<int>(3, 0));
    int humanPlayer = 1, currentPlayer = 1;
    char player;
    recin:
    cout << "Choose your player (X or O): ";
    cin >> player;
    if(player == 'O') humanPlayer = -1;
    else if(player != 'X'){
    	cout << "Invalid player. Try again";
    	goto recin;
	} 
    cout << "You are " << humanPlayer << " player" << endl;

    renderGame(s);
    while (true) {
        int i = -1, j = -1; 
        if (humanPlayer == currentPlayer) {
        	moveAgain:
            cout << "Enter your play: ";
            cin >> i >> j;
            // check legal move
            if( !checkLegalMove(s, i, j) ){
            	cout << "Illegal move! ";
            	goto moveAgain;
			}
            cout << "You play (" << i << ", " << j << ")"<< endl;
        } else {
            pair<int, int> move = getComputerPlay(s);
            i = move.first + 1;
            j = move.second + 1;
            cout << "I play (" << i << ", " << j << ")" << endl;
        }
        s = play(s, i - 1, j - 1, currentPlayer);
        renderGame(s);
        
        currentPlayer *= -1;
        if (isFinalState(s)) {
            int score = getScoreFinalState(s) * player;
            if (score > 0) {
                cout << "You won" << endl; 
            } else if (score < 0) {
                cout << "I won" << endl; 
            } else {
                cout << "We draw" << endl; 
            }
            break;
        }
    }
    return 0;
}
