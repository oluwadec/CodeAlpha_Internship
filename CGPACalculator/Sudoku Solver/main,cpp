#include <iostream>
using namespace std;

const int SIZE = 9;

// Prints the Sudoku board in a clean format
void printBoard(int board[SIZE][SIZE])
{
    cout << endl;
    cout << "Sudoku Board:" << endl;

    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            cout << board[row][col] << " ";

            // Add a space after every 3 columns
            if ((col + 1) % 3 == 0 && col != SIZE - 1)
            {
                cout << " ";
            }
        }

        cout << endl;

        // Add a space after every 3 rows
        if ((row + 1) % 3 == 0 && row != SIZE - 1)
        {
            cout << endl;
        }
    }
}

// Checks if a number already exists in the same row
bool isInRow(int board[SIZE][SIZE], int row, int number)
{
    for (int col = 0; col < SIZE; col++)
    {
        if (board[row][col] == number)
        {
            return true;
        }
    }

    return false;
}

// Checks if a number already exists in the same column
bool isInColumn(int board[SIZE][SIZE], int col, int number)
{
    for (int row = 0; row < SIZE; row++)
    {
        if (board[row][col] == number)
        {
            return true;
        }
    }

    return false;
}

// Checks the 3x3 box containing the empty cell
bool isInBox(int board[SIZE][SIZE], int startRow, int startCol, int number)
{
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (board[startRow + row][startCol + col] == number)
            {
                return true;
            }
        }
    }

    return false;
}

// Checks all three Sudoku rules before placing a number
bool isSafe(int board[SIZE][SIZE], int row, int col, int number)
{
    // Find the top-left corner of the 3x3 box
    int boxRow = row - row % 3;
    int boxCol = col - col % 3;

    if (isInRow(board, row, number))
    {
        return false;
    }

    if (isInColumn(board, col, number))
    {
        return false;
    }

    if (isInBox(board, boxRow, boxCol, number))
    {
        return false;
    }

    return true;
}

// Uses backtracking to solve the puzzle
bool solveSudoku(int board[SIZE][SIZE])
{
    // Look for an empty cell
    for (int row = 0; row < SIZE; row++)
    {
        for (int col = 0; col < SIZE; col++)
        {
            if (board[row][col] == 0)
            {
                // Try every number from 1 to 9
                for (int number = 1; number <= 9; number++)
                {
                    if (isSafe(board, row, col, number))
                    {
                        // Put the number in the empty cell
                        board[row][col] = number;

                        // Try to solve the rest of the puzzle
                        if (solveSudoku(board))
                        {
                            return true;
                        }

                        // The number didn't work, so undo it
                        board[row][col] = 0;
                    }
                }

                // None of the numbers worked in this cell
                return false;
            }
        }
    }

    // There are no empty cells, so the puzzle is solved
    return true;
}

int main()
{
    // 0 represents an empty cell
    int board[SIZE][SIZE] =
    {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},

        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},

        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    cout << "Sudoku Solver" << endl;
    cout << endl;

    cout << "Original puzzle:";
    printBoard(board);

    // Try to solve the puzzle
    if (solveSudoku(board))
    {
        cout << endl;
        cout << "Solution found:";
        printBoard(board);
    }
    else
    {
        cout << endl;
        cout << "This Sudoku puzzle has no solution." << endl;
    }

    return 0;
}