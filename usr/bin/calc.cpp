#include <iostream>
#include <string>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <algorithm>
#include <sstream>
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
        display = text;
        if (display.length() > 13) display = display.substr(0, 13);
    }

    void render() {
        clearScreen();
        cout << "  ┌─────────────┐ " << endl;
        cout << "  │ " << display;
        for (int i = 0; i < 11 - display.length(); ++i) cout << " ";
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
        setRawMode(true);
        char ch;
        while (true) {
            render();
            ch = getchar();
            if (ch == 'q') exit(0);
            
            lastPressed = ch;
            onKeyPress(ch);

            render();
            usleep(50000); 
            lastPressed = ' ';
        }
        setRawMode(false);
        clearScreen();
    }
};

int main(int argc, char* argv[]) {
    if (argc == 2) {
        string arg = argv[1];
        if (arg == "--help" || arg == "help") {
            cout << "MicroOS calc v0.1 beta" << endl;
            cout << "Use 0-9, +, -, *, /, =, c keys to calculate" << endl;
            cout << "q to quit" << endl;
            return 0;
        }
    }
    CalculatorTUI calc;
    calc.run();
    return 0;
}