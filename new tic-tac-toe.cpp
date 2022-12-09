#include <bits/stdc++.h>
#include <vector>
using namespace std;

/*
1. inside gameplay
	- Game over/Decide the outcome of the game
	- Point of state
	- Best move
	- Which player is playing
	- Which player is user
2. user interface
	- Show state
	
1.Ai engine
	- state: vector<vector<int>> (SIZExSIZE)
		+ SIZE (=3)
		+ new typedef
		+ array of characters to convert in rendering
	- decide outcome of the game
		+ game over or not
	- is the move valid 
	- calculate best move
		+ calculate state score
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

//1. inside gameplay
//Game over/Decide the outcome of the game

/*Decide if game is over or not
	over fuction: over : 
	a. three in a row/col/diagonal
		3X: sum of row/col/diagonal = 3
		3O: sum of row/col/diagonal = -3
		--> |sum of row/col/diagonal| = 3 means over
	b. fullboard:
		no blank in board = no 0 in State
*/
bool over(State s){
	//three in a row/col/diagonal
	for(int i = 0; i < SIZE; i++){
		if(abs(s[0][i] + s[1][i] + s[2][i]) == 3) return true; //check row
		if(abs(s[i][0] + s[i][1] + s[i][2]) == 3) return true; //check col
	}
	if(abs(s[0][0] + s[1][1] + s[2][2]) == 3) return true;
	if(abs(s[2][0] + s[1][1] + s[0][2]) == 3) return true; //check diagonals
}

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

bool isFinalState(State s)
{
    // check rows & cols
    for (int i = 0; i < SIZE; i++)
    	if(abs(s[i][0] + s[i][1] + s[i][2]) == 3) return true;
    	if(abs(s[0][i] + s[1][i] + s[2][i]) == 3) return true;
    // check diagonals
    if(abs(s[0][0] + s[1][1] + s[2][2]) == 3) return true;
   	if(abs(s[2][0] + s[1][1] + s[0][2]) == 3) return true;
   	// check full
   	for (int i = 0; i < SIZE; i++){
   		for(int j = 0; j < SIZE; j++){
   			if(s[i][j] == 0) return false;	
		}	
	}
    return true;
}

int getScoreFinalState(State s)
{
    // check rows & cols
    for (int i = 0; i < SIZE; i++)
    	if(abs(s[i][0] + s[i][1] + s[i][2]) == 3) return s[i][0];
    	if(abs(s[0][i] + s[1][i] + s[2][i]) == 3) return s[0][i];
    // check diagonals
    if(abs(s[0][0] + s[1][1] + s[2][2]) == 3) return s[0][0];
    if(abs(s[2][0] + s[1][1] + s[0][2]) == 3) return s[2][0];
    return 0;
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
        if (getNextPlayer == 1){
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

bool checkLegalMove(State s, int i, int j){
	renderGame(s);
	if(i <= 0 or i > SIZE or j <= 0 or j > SIZE) return false; // out of bounds
	if(s[i - 1][j - 1] != 0) return false; // already filled
	return true; 
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
