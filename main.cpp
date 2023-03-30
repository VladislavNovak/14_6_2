#include <iostream>
#include <vector>
#include <numeric> // accumulate

using std::cout;
using std::endl;
using std::vector;
using std::string;

enum Player { TIC = 'X', TAC = 'O' };
enum GameStatus { IN_PROCESS, TIC_IS_WIN, TAC_IS_WIN };

void displayBoard(const vector<vector<char>> &board) {
    cout << "\n\tPLAYER - 1 [X]\t PLAYER - 2 [O]\n" << endl;

    for (int line = 0; line < 3; ++line) {
        cout << "\t\t     |     |     \n\t\t  ";
        for (int cell = 0; cell < 3; ++cell) cout << board[line][cell] << (cell < 2 ? "  |  " : " \n");
        cout << (line < 2 ? "\t\t_____|_____|_____\n" : "\t\t     |     |     \n");
    }
}

// возвращает вектор с суммарными данными по строкам, колонкам и диагоналям
GameStatus getCurrentGameStatus(const vector<vector<char>> &board) {
    const int LEFT_DIAGONAL = 6;
    const int RIGHT_DIAGONAL = 7;
    // 0,1,2 - для строк, 3,4,5 - для колонок, 6,7 - для диагоналей
    vector<int> lines = {0, 0, 0, 0, 0, 0, 0, 0};
    size_t gridSize = board.size();

    for (int i = 0, rowReversed = (int) gridSize - 1; i < gridSize; ++i, --rowReversed) {
        vector<char> row = board[i];
        size_t rowSize = row.size();

        // сохраняем результат по строкам
        lines[i] = accumulate(row.begin(), row.end(), 0);

        for (int j = 0, start = 3, columnReversed = (int) rowSize - 1; j < rowSize; ++j, ++start, --columnReversed) {
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
        cout << "log (line):" << line << endl;

        if (line == (Player::TIC * 3)) {
            gameStatus = GameStatus::TIC_IS_WIN;
            break;
        } else if (line == (Player::TAC * 3)) {
            gameStatus = GameStatus::TAC_IS_WIN;
            break;
        } else {
            cout << "Game is process" << endl;
        }
    }

    return gameStatus;
}

int main() {
    // условные обозначения:
    // let - создаем переменную
    // f() - вынести в отдельную функцию

    // let char board[3][3] - это матрица 3 х 3, отображающая текущее положение на доске
    // let enum Player { TIC = 'X', TAC = '0' } - игроки
    // let char CurrentPlayer - текущий игрок

    size_t BOARD_SIZE = 3;
    bool gameStatus = GameStatus::IN_PROCESS;
    char currentPlayer = Player::TIC;
    vector<vector<char>> board(BOARD_SIZE);

    for (auto &line : board) line.resize(BOARD_SIZE, ' ');

    while(gameStatus == GameStatus::IN_PROCESS) {
        displayBoard(board);

        // ШАГ 1:
        // делаем ход. Если ход сделал TIC, то ходит TAC. И наоборот.
        // соответственно сохраняем текущего игрока

        // ШАГ 2:
        // для хода, текущий пользователь должен ввести значение по линии и колонке.
        // значит - нужна проверка f().
        // ввел:
        // - a) не прошел валидацию: входим в цикл
        // - б) все ок: переходим к следующему шагу

        while(true) {
            int choice[2] = { -1, -1 };

            for (int axis = 0; axis < 2; ++axis) {
                string axisName = (axis == 0 ? "line" : "column");
                while(true) {
                    int place;
                    cout << "Enter " << axisName << " number (1, 2, 3):";
                    std::cin >> place;
                    if (place == 1 || place == 2 || place == 3) {
                        choice[axis] = (place - 1);
                        break;
                    } else {
                        cout << "Wrong. try again!" << endl;
                    }
                }
            }

            // учитываем вариант, когда данная ячейка уже выбрана
            if (board[choice[0]][choice[1]] == ' ') {
                // ШАГ 3:
                // вносим изменения в board
                board[choice[0]][choice[1]] = currentPlayer;
                break;
            }

            cout << "Specified cell is already selected" << endl;
            // позже можем показывать доску
            // displayBoard(board);
        }

        displayBoard(board);

        // ШАГ 4:
        // проверяем каждую линию и количество знаков.
        // Возвращаем результат. Это может быть сигнал:
        // - а) сообщение о победе текущего игрока
        // - б) сообщение о продолжении. Тогда переходим к ШАГ 1.

        gameStatus = getCurrentGameStatus(board);

        if (currentPlayer == Player::TIC) {
            currentPlayer = Player::TAC;
        } else {
            currentPlayer = Player::TIC;
        }
    }
}