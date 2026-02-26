#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

string path;

class TextEngine {
public:
    vector<string> lines;
    int cursorX = 0;
    int cursorY = 0;
    int rowOffset = 0;
    int screenRows = 0;
    int screenCols = 0;
    termios orig_termios;

    TextEngine() {
        updateWindowSize();
        enableRawMode();
    }

    ~TextEngine() {
        disableRawMode();
    }

    void updateWindowSize() {
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_row == 0) {
            screenRows = 24;
            screenCols = 80;
        } else {
            screenRows = ws.ws_row;
            screenCols = ws.ws_col;
        }
    }

    void enableRawMode() {
        tcgetattr(STDIN_FILENO, &orig_termios);
        termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
        raw.c_iflag &= ~(IXON | ICRNL);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    void disableRawMode() {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }

    void scroll() {
        if (cursorY < rowOffset) {
            rowOffset = cursorY;
        }
        if (cursorY >= rowOffset + screenRows) {
            rowOffset = cursorY - screenRows + 1;
        }
    }

    void insertChar(char c) {
        if (lines.empty()) lines.push_back("");
        lines[cursorY].insert(cursorX, 1, c);
        cursorX++;
    }

    void insertNewline() {
        if (lines.empty()) lines.push_back("");
        string currentLine = lines[cursorY];
        lines[cursorY] = currentLine.substr(0, cursorX);
        lines.insert(lines.begin() + cursorY + 1, currentLine.substr(cursorX));
        cursorY++;
        cursorX = 0;
    }

    void backspace() {
        if (lines.empty()) return;
        if (cursorX > 0) {
            lines[cursorY].erase(cursorX - 1, 1);
            cursorX--;
        } else if (cursorY > 0) {
            cursorX = lines[cursorY - 1].length();
            lines[cursorY - 1] += lines[cursorY];
            lines.erase(lines.begin() + cursorY);
            cursorY--;
        }
    }

    void refreshScreen() {
        scroll();
        
        string buffer = "\x1b[?25l";
        buffer += "\x1b[H";

        for (int i = 0; i < screenRows; i++) {
            int fileRow = i + rowOffset;
            if (fileRow < lines.size()) {
                string line = lines[fileRow];
                if (line.length() > screenCols) line = line.substr(0, screenCols);
                buffer += line;
            }
            buffer += "\x1b[K";
            if (i < screenRows - 1) buffer += "\r\n";
        }

        char cursorGoto[32];
        snprintf(cursorGoto, sizeof(cursorGoto), "\x1b[%d;%dH", (cursorY - rowOffset) + 1, cursorX + 1);
        buffer += cursorGoto;
        
        buffer += "\x1b[?25h";
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
    }

    void processInput() {
        char c;
        if (read(STDIN_FILENO, &c, 1) <= 0) return;

        if (c == 27) {
            char seq[3];
            if (read(STDIN_FILENO, &seq[0], 1) == 0) return;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) return;

            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A': if (cursorY > 0) cursorY--; break;
                    case 'B': if (cursorY < (int)lines.size() - 1) cursorY++; break;
                    case 'C': if (cursorX < (int)lines[cursorY].length()) cursorX++; break;
                    case 'D': if (cursorX > 0) cursorX--; break;
                }
            }
        } else if (c == 13) {
            insertNewline();
        } else if (c == 127 || c == 8) {
            backspace();
        } else if (c == 17) {
            disableRawMode();
            cout << "\033[2J\033[H";
            exit(0);
        } else if (c == 19) {
            ofstream file(path);
            if (file.is_open()) {
                for (string line : lines) {
                    file << line << endl;
                }
                file.close();
            } else {
                disableRawMode();
                cerr << "edit: error while saving file" << endl;
                exit(1);
            }
        } else if (!iscntrl(c) || c == '\t') {
            insertChar(c);
        }

        if (!lines.empty() && cursorX > (int)lines[cursorY].length()) {
            cursorX = lines[cursorY].length();
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (string(argv[1]) == "--help" || string(argv[1]) == "help") {
            cout << "MicroOS edit v0.1" << endl;
            cout << "Usage: edit [filename]" << endl;
            cout << "shortcuts: ctrl+q - exit" << endl;
            cout << "           ctrl+s - save" << endl;
            return 0;
        }
        path = argv[1];
        TextEngine editor;
        if (fs::exists(path)) {
            ifstream file(path);
            string line;
            if (file.is_open()) {
                while (getline(file, line)) {
                    editor.lines.push_back(line);
                }
                file.close();
            } else {
                cerr << "edit: error couldn't open file" << endl;
                return 1;
            }
        }
        while (true) {
            editor.refreshScreen();
            editor.processInput();
        }
        return 0;
    } else {
        cout << "Usage: edit [filename]" << endl;
        return 1;
    }
}