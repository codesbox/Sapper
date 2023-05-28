#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Cell {
    bool hasMine;
    bool revealed;
    bool flagged;
};

typedef vector<vector<Cell>> Board;

const int BOARD_SIZE = 10;
const int EASY_MINES = 10;
const int MEDIUM_MINES = 20;
const int HARD_MINES = 30;

void initializeBoard(Board& board) {
    board.resize(BOARD_SIZE, vector<Cell>(BOARD_SIZE));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j].hasMine = false;
            board[i][j].revealed = false;
            board[i][j].flagged = false;
        }
    }
}

void plantMines(Board& board, int numMines) {
    int count = 0;
    while (count < numMines) {
        int row = rand() % BOARD_SIZE;
        int col = rand() % BOARD_SIZE;
        if (!board[row][col].hasMine) {
            board[row][col].hasMine = true;
            count++;
        }
    }
}

bool isValidCell(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

int countAdjacentMines(const Board& board, int row, int col) {
    static const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    static const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    int count = 0;
    for (int i = 0; i < 8; ++i) {
        int newRow = row + dx[i];
        int newCol = col + dy[i];
        if (isValidCell(newRow, newCol) && board[newRow][newCol].hasMine) {
            count++;
        }
    }
    return count;
}

void revealCell(Board& board, int row, int col) {
    if (isValidCell(row, col) && !board[row][col].revealed) {
        board[row][col].revealed = true;
        if (!board[row][col].hasMine) {
            int count = countAdjacentMines(board, row, col);
            if (count == 0) {
                static const int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
                static const int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

                for (int i = 0; i < 8; ++i) {
                    int newRow = row + dx[i];
                    int newCol = col + dy[i];
                    revealCell(board, newRow, newCol);
                }
            }
        }
    }
}

void flagCell(Board& board, int row, int col) {
    if (isValidCell(row, col)) {
        board[row][col].flagged = !board[row][col].flagged;
    }
}

void printBoard(const Board& board, bool revealMines = false) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].flagged) {
                cout << "F ";
            }
            else if (!board[i][j].revealed) {
                cout << "* ";
            }
            else if (board[i][j].hasMine) {
                if (revealMines) {
                    cout << "X ";
                }
                else {
                    cout << "* ";
                }
            }
            else {
                int count = countAdjacentMines(board, i, j);
                cout << count << " ";
            }
        }
        cout << endl;
    }
}

bool hasWon(const Board& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (!board[i][j].revealed && !board[i][j].hasMine) {
                return false;
            }
        }
    }
    return true;
}

int main() {
	setlocale(LC_ALL, "Russian");
    srand(time(0));

    Board board;
    initializeBoard(board);

    int level;
    cout << "Выберите уровень сложности:\n1. Легкий\n2. Средний\n3. Сложный\n";
    cin >> level;

    int numMines;
    switch (level) {
    case 1:
        numMines = EASY_MINES;
        break;
    case 2:
        numMines = MEDIUM_MINES;
        break;
    case 3:
        numMines = HARD_MINES;
        break;
    default:
        numMines = EASY_MINES;
    }

    plantMines(board, numMines);

    while (true) {
        printBoard(board);

        int row, col;
        cout << "Введите координаты клетки (строка столбец): ";
        cin >> row >> col;

        if (!isValidCell(row, col)) {
            cout << "Недопустимые координаты. Попробуйте снова." << endl;
            continue;
        }

        char action;
        cout << "Выберите действие (R - открыть клетку, F - установить флаг): ";
        cin >> action;

        if (action == 'R' || action == 'r') {
            if (board[row][col].hasMine) {
                cout << "Неудача" << endl;
                printBoard(board, true);
                break;
            }
            revealCell(board, row, col);
            if (hasWon(board)) {
                cout << "Успех" << endl;
                printBoard(board, true);
                break;
            }
        }
        else if (action == 'F' || action == 'f') {
            flagCell(board, row, col);
        }
        else {
            cout << "Недопустимое действие. Попробуйте снова." << endl;
        }
    }

    return 0;
}