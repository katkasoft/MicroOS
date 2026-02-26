#include <iostream>
#include <vector>
#include <cctype>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono_literals;

class TicTacToeGame {
private:
    std::vector<char> map{'1', '2', '3', '4', '5', '6', '7', '8', '9'}; 
    bool isFirstTurn = true;

    bool checkFree(int i) {
        if (map[i] != 'X' && map[i] != 'O') return true;
        else return false;
    }

    void clearScreen() {
        cout << "\033[2J\033[H";
    }

    void render() {
        cout << "  Tic-Tac-Toe" << endl;
        cout << " -------------" << endl;
        for (int i = 0; i < 9; i++) {
            cout << " | " << map[i];
            if ((i + 1) % 3 == 0) {
                cout << " |" << endl;
                cout << " -------------" << endl;
            }
        }
    }

    void botTurn() {
        vector<int> freeSpaces;
        for (int i = 0; i < 9; i++) {
            if (checkFree(i)) freeSpaces.push_back(i);
        }

        if (freeSpaces.empty()) return;

        int randomIndex = std::rand() % freeSpaces.size();
        map[freeSpaces[randomIndex]] = 'O';
    }

    bool checkWin(char player) {
        const int winPatterns[8][3] = {
            {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // horisontally
            {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // vertically
            {0, 4, 8}, {2, 4, 6}             // diagonally
        };

        for (int i = 0; i < 8; i++) {
            if (map[winPatterns[i][0]] == player &&
                map[winPatterns[i][1]] == player &&
                map[winPatterns[i][2]] == player) {
                return true;
            }
        }
        return false;
}
public:
    TicTacToeGame() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }
    void run() {
        while (true) {
            clearScreen();
            render();
            
            if (isFirstTurn) {
                for (int j = 0; j < 9; j++) {
                    map[j] = ' ';
                }
                isFirstTurn = false;
            }
            
            if (checkWin('X')) {
                cout << "You win!" << endl;
                break;
            } else if (checkWin('O')) {
                cout << "Opponent win!" << endl;
                break;
            }

            char turn;
            cout << "Turn (1-9): ";
            cin >> turn;
            
            if (turn == 'q') break;

            if (isdigit(turn)) {
                int i = turn - '0';
                if (i >= 1 && i <= 9) {
                    i--;
                    if (checkFree(i)) {
                        map[i] = 'X';
                        botTurn();
                    } else {
                        cerr << map[i] << " already on this position" << endl;
                        this_thread::sleep_for(5s);
                    }
                } else {
                    cerr << "Turn must be from 1 to 9" << endl;
                    this_thread::sleep_for(5s);
                }
            } else {
                cerr << "Turn must be digit 1-9" << endl;
                this_thread::sleep_for(5s); 
            }
        }
    }
};

int main() {
    TicTacToeGame game;
    game.run();
    return 0;
}