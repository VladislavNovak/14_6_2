#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::string;

size_t BOARD_SIZE = 3;
enum Player { TIC = 'X', TAC = 'O' };
enum GameStatus { IN_PROCESS, TIC_IS_WIN, TAC_IS_WIN, DRAW };

// Графическое представление доски
void displayBoard(const vector<vector<char>> &board) {
    cout << "\n\tPLAYER - 1 [X]\t PLAYER - 2 [O]\n" << endl;

    for (int line = 0; line < 3; ++line) {
        cout << "\t\t     |     |     \n\t\t  ";
        for (int cell = 0; cell < 3; ++cell) cout << board[line][cell] << (cell < 2 ? "  |  " : " \n");
        cout << (line < 2 ? "\t\t_____|_____|_____\n" : "\t\t     |     |     \n");
    }
    cout << endl;
}

// Пользовательский ввод после минимальной валидации меняет ячейку на доске
void changeBoard(vector<vector<char>> &board, const char currentPlayer) {
    while(true) {
        int choice[2] = { -1, -1 };

        for (int axis = 0; axis < 2; ++axis) {
            string axisName = (axis == 0 ? "line" : "column");
            while(true) {
                int place;
                cout << "Player " << currentPlayer << ": enter number (1, 2, 3) to select " << axisName << ": ";
                std::cin >> place;
                if (place == 1 || place == 2 || place == 3) {
                    choice[axis] = (place - 1);
                    break;
                } else {
                    cout << "Wrong. Try again!" << endl;
                }
            }
        }

        // если ячейка ещё не выбрана - тогда вносим изменения в board
        if (board[choice[0]][choice[1]] == ' ') {
            board[choice[0]][choice[1]] = currentPlayer;
            break;
        }

        cout << "Specified cell is already selected. Try again" << endl;
    }
}

// Анализирует доску по строкам, колонкам и диагоналям. Возвращает один из статусов игры
GameStatus getCurrentGameStatus(const vector<vector<char>> &board) {
    // 0,1,2 - для строк, 3,4,5 - для колонок, 6,7 - для диагоналей
    vector<int> lines = {0, 0, 0, 0, 0, 0, 0, 0};
    const int LEFT_DIAGONAL = 6;
    const int RIGHT_DIAGONAL = 7;
    // Подсчитываем количество пустых ячеек. Должна быть хотя бы одна
    int emptyCells = 0;

    for (int i = 0, rowReversed = (int) BOARD_SIZE - 1; i < BOARD_SIZE; ++i, --rowReversed) {
        vector<char> row = board[i];

        // сохраняем результат по строкам
        for (char cell : row) {
            lines[i] += cell;

            if (cell == ' ') ++emptyCells;
        }

        for (int j = 0, start = 3, columnReversed = (int) BOARD_SIZE - 1; j < BOARD_SIZE; ++j, ++start, --columnReversed) {
            char cell = row[j];
            // сохраняем результат по столбцам
            lines[start] += cell;
            // результат первой диагонали
            if (i == j) lines[LEFT_DIAGONAL] += cell;
            // результат второй диагонали
            if (rowReversed == columnReversed) lines[RIGHT_DIAGONAL] += row[columnReversed];
        }
    }

    GameStatus gameStatus = GameStatus::IN_PROCESS;

    for (int line : lines) {
        if (line == (Player::TIC * 3)) {
            gameStatus = GameStatus::TIC_IS_WIN;
            break;
        } else if (line == (Player::TAC * 3)) {
            gameStatus = GameStatus::TAC_IS_WIN;
            break;
        }
    }

    if (emptyCells == 0) gameStatus = GameStatus::DRAW;

    return gameStatus;
}

int main() {
    bool gameStatus = GameStatus::IN_PROCESS;
    char currentPlayer = Player::TIC;
    vector<vector<char>> board(BOARD_SIZE);

    for (auto &line : board) line.resize(BOARD_SIZE, ' ');

    cout << "GAME HAS BEGUN" << endl;

    while(gameStatus == GameStatus::IN_PROCESS) {

        changeBoard(board, currentPlayer);

        displayBoard(board);

        gameStatus = getCurrentGameStatus(board);

        if (gameStatus == GameStatus::IN_PROCESS) {
            if (currentPlayer == Player::TIC) {
                currentPlayer = Player::TAC;
            } else {
                currentPlayer = Player::TIC;
            }
        }
    }

    cout << "************" << endl;
    cout << "status: " << gameStatus << endl;
    cout << "GAME IS OVER" << endl;
    cout << "************" << endl;
}