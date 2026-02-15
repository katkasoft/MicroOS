#include <iostream>
#include <string>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <algorithm>
#include <sstream>
#include <poll.h>
#include <chrono>
#include "../../lib/calc.hpp"

using namespace std;

class CalculatorTUI {
private:
    string display = "0";
    string buffer;
    char lastPressed = ' ';
    vector<char> printed_keys = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '-', '*', '/', '.'};

    void setRawMode(bool enable) {
        static struct termios oldt, newt;
        if (enable) {
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            newt.c_cc[VMIN] = 0;
            newt.c_cc[VTIME] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        } else {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        }
    }

    void clearScreen() {
        cout << "\033[2J\033[H";
    }

    void drawButton(char label) {
        if (lastPressed == label) {
            cout << "\033[7m " << label << " \033[0m";
        } else {
            cout << "[" << label << "]";
        }
    }

public:
    void updateDisplay(string text) {
        if (text.length() > 11) { 
            display = text.substr(0, 11);
        } else {
            display = text;
        }
    }

    void render() {
        clearScreen();
        cout << "  ┌─────────────┐ " << endl;
        cout << "  │ " << display;
        int spaces = 11 - (int)display.length();
        if (spaces > 0) {
            for (int i = 0; i < spaces; ++i) cout << " ";
        }
        cout << " │ " << endl;
        cout << "  └─────────────┘ " << endl;
        
        cout << "  "; drawButton('7'); cout << " "; drawButton('8'); cout << " "; drawButton('9'); cout << " "; drawButton('/'); cout << endl;
        cout << "  "; drawButton('4'); cout << " "; drawButton('5'); cout << " "; drawButton('6'); cout << " "; drawButton('*'); cout << endl;
        cout << "  "; drawButton('1'); cout << " "; drawButton('2'); cout << " "; drawButton('3'); cout << " "; drawButton('-'); cout << endl;
        cout << "  "; drawButton('0'); cout << " "; drawButton('.'); cout << " "; drawButton('='); cout << " "; drawButton('+'); cout << endl;
        cout << "  "; drawButton('C'); cout << endl;
    }

    void onKeyPress(char key) {
        if (key == 'C' || key == 'c') {
            buffer = "";
            updateDisplay("0");
        } else if (key == '=') {
            try {
                double val = calc(buffer);
                ostringstream oss;
                oss << val;
                string res = oss.str();
                updateDisplay(res);
                buffer = res;
            } catch (...) {
                updateDisplay("error");
                buffer = "";
            }
        } else if (find(printed_keys.begin(), printed_keys.end(), key) != printed_keys.end()) {
            if (buffer == "0") buffer = ""; 
            buffer += key;
            updateDisplay(buffer);
        }
    }

    void run() {
        atexit([]() {
            struct termios t;
            tcgetattr(STDIN_FILENO, &t);
            t.c_lflag |= (ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &t);
            cout << "\033[?25h";
            cout << "\033[2J\033[H";
            cout.flush(); 
        });
        setRawMode(true);
        cout << "\033[?25l";
        auto lastPressTime = chrono::steady_clock::now();
        bool needsRedraw = true;
        while (true) {
            if (needsRedraw) {
                render();
                needsRedraw = false;
            }
            auto now = chrono::steady_clock::now();
            if (lastPressed != ' ' && chrono::duration_cast<chrono::milliseconds>(now - lastPressTime).count() > 150) {
                lastPressed = ' ';
                needsRedraw = true;
            }
            struct pollfd pfd = { STDIN_FILENO, POLLIN, 0 };
            if (poll(&pfd, 1, 10) > 0) {
                char ch;
                if (read(STDIN_FILENO, &ch, 1) > 0) {
                    if (ch == 'q') break;
                    lastPressed = ch;
                    lastPressTime = chrono::steady_clock::now();
                    onKeyPress(ch);
                    needsRedraw = true;
                }
            }
        }
        setRawMode(false);
    }
};

int main(int argc, char* argv[]) {
    if (argc == 2) {
        string arg = argv[1];
        if (arg == "--help" || arg == "help") {
            cout << "MicroOS calc v0.2" << endl;
            cout << "Usage:" << endl;
            cout << "1. calc (without args) - enter TUI calculator interface" << endl;
            cout << "- Use 0-9, +, -, *, /, =, ., c keys to calculate" << endl;
            cout << "- q to quit" << endl;
            cout << "2. calc [-c/--cli/cli] - enter calculator command line" << endl;
            cout << "- q to quit" << endl;
            cout << "2. calc [expression] - calculate expression" << endl;
        } else if (arg == "-c" || arg == "--cli" || arg == "cli") {
            cout << "MicroOS calc v0.2" << endl;
            cout << "q to quit" << endl;
            while (true) {
                cout << "calc>";
                string expr;
                cin >> expr;
                if (expr == "q") return 1;
                try {
                    cout << calc(expr) << endl;
                } catch (const runtime_error& e) {
                    cerr << "calculation exited with error: " << e.what() << endl;
                }
            }
        } else {
            try {
                cout << calc(arg) << endl;
            } catch (const runtime_error& e) {
                cerr << "calculation exited with error: " << e.what() << endl;
            }
        }
    } else {
        CalculatorTUI calc;
        calc.run();
    }
    return 0;
}