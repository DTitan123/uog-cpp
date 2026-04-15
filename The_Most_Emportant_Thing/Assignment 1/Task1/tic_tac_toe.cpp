#include <iostream> 
#include <cstdlib>  
#include <ctime>    
using namespace std; 

// Prints the current state of the board.
void drawBoard(char board[3][3]) { //char → each element is a character ('X', 'O', or ' ').
    
    system("clear");

    cout << "-------------\n";
    for (int i = 0; i < 3; i++) { //int i = 0; → start at 0
                                //i < 3; → keep going as long as i is less than 3
                                //i++ → after each loop, add 1 to i 
        cout << "| ";
        for (int j = 0; j < 3; j++) { //nested loop to go through each cell in the row.
            cout << board[i][j] << " | "; //prints the current cell and a vertical line.
        }
        cout << "\n-------------\n"; //prints a line after each row.
    }
}

// Reads and validates the player's move.
void playerMove(char board[3][3]) { 
    while (true) {
        int move;
        cout << "Select your move (numeric keypad 1-9): ";
        cin >> move;

        // Converts keypad input into board row and column indices.
        int row = 2 - (move - 1) / 3;
        int col = (move - 1) % 3;

        if (move < 1 || move > 9) {
            cout << "Invalid number; please select again.\n";
          } else if (board[row][col] != ' ') {
            cout << "Invalid move; please select again.\n";
          } else {
            board[row][col] = 'x';
            break;
          }
    }
}
// Generates a random valid move for the computer.
void computerMove(char board[3][3]) {
    while (true) {
        int move = rand() % 9 + 1;  // random number 1–9

        int row = 2 - (move - 1) / 3;
        int col = (move - 1) % 3;

        if (board[row][col] == ' ') {
            board[row][col] = 'o';
            break;
        }
    }
}

// Checks rows, columns, and diagonals for a winner.
char checkWin(char board[3][3]) {

    // Check rows
    for (int row = 0; row < 3; row++) {
        if (board[row][0] != ' ' &&
            board[row][0] == board[row][1] &&
            board[row][1] == board[row][2]) {
            return board[row][0];  // 'x' or 'o'
        }
    }

    // Check columns
    for (int col = 0; col < 3; col++) {
        if (board[0][col] != ' ' &&
            board[0][col] == board[1][col] &&
            board[1][col] == board[2][col]) {
            return board[0][col];
        }
    }

    // Check diagonal (top-left to bottom-right)
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        return board[0][0];
    }

    // Check diagonal (top-right to bottom-left)
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return board[0][2];
    }

    return 'n';  // n = no one has won yet
}


int main() { //int → main returns an integer (a number) to the operating system.
    // Seeds the random number generator so the computer moves differ each game.
    srand(time(0)); 

    // THIS is where the board belongs.
    char board[3][3] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
    };

    int moves = 0;
    char winner = 'n';

    while (true) {

        drawBoard(board);

        // Player move
        playerMove(board);
        moves++;

        winner = checkWin(board);
        if (winner == 'x') {
            drawBoard(board);
            cout << "You win!\n";
            break;
        }

        if (moves == 9) {
            drawBoard(board);
            cout << "It's a tie!\n";
            break;
        }

        // Computer move
        computerMove(board);
        moves++;

        winner = checkWin(board);
        if (winner == 'o') {
            drawBoard(board);
            cout << "Computer wins!\n";
            break;
        }

        if (moves == 9) {
            drawBoard(board);
            cout << "It's a tie!\n";
            break;
        }
    }
    return 0; //return 0 means the program finished successfully.
}