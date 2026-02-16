#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace std;

string path;

class Reader {
public:
    vector<string> lines;
    int cursorY = 0;
    int rowOffset = 0;
    int screenRows = 0;
    int screenCols = 0;
    termios orig_termios;

    Reader() {
        updateWindowSize();
        enableRawMode();
    }

    ~Reader() {
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
                }
            }
        } else if (c == 17) {
            disableRawMode();
            cout << "\033[2J\033[H";
            exit(0);
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (string(argv[1]) == "--help" || string(argv[1]) == "help") {
            cout << "MicroOS reader v0.1" << endl;
            cout << "Usage: reader [filename]" << endl;
            cout << "shortcuts: ctrl+q - exit" << endl;
            return 0;
        }
        path = argv[1];
        Reader reader;
        ifstream file(path);
        string line;
        if (file.is_open()) {
            while (getline(file, line)) {
                reader.lines.push_back(line);
            }
            file.close();
        } else {
            cerr << "reader: error couldn't open file" << endl;
            return 1;
        }
        while (true) {
            reader.refreshScreen();
            reader.processInput();
        }
        return 0;
    } else {
        cout << "Usage: reader [filename]" << endl;
        return 1;
    }
}