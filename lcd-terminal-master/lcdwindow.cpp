#include <QCloseEvent>

#include "lcdwindow.h"


#include <QDebug>
#include "lcdinput.h"

LcdWindow::LcdWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    setFixedSize(800, 150);
    setWindowTitle("16x2 LCD");
    cursor = 0;
    displayRowPos = 0;

    display[0][0]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 16; ++col) {
            storedSymbols[row][col] = '\0';
        }
    }

    // form to test input
    //LcdInput *inp = new LcdInput(this);
    //inp->show();
}

LcdWindow::~LcdWindow() {
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 16; ++col) {
            delete display[row][col];
        }
    }
}

void LcdWindow::clearDisplay() {
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 16; ++col) {
            display[row][col]->clear();
        }
    }
}

void LcdWindow::shiftSymbols() {
    char tmp[16][16];
    memcpy(tmp, storedSymbols, sizeof(storedSymbols));

    int shift = 16 - (cursor % 16);

    for (int i = 0; i < shift; i++) {
        storedSymbols[(cursor+i) / 16][(cursor+i) % 16] = '\0';
    }

    if (cursor % 16 == 0) {
        bool check = 0;
        for (int i = 0; i < 16; i++) {
            if (storedSymbols[(cursor+i) / 16][(cursor+i) % 16] != '\0') {
                check = 1;
                break;
            }
        }
        if (!check) {
            storedSymbols[cursor / 16][cursor % 16] = '\n';
        }
    }

    // last row
    if(cursor / 16 == 15){
        updateDisplay(1);
        return;
    }

    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
    displayRowPos = 1;

    cursor += shift;

    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

    // line break
    for (int i = 0; i < shift; i++) {
        storedSymbols[(cursor+i) / 16][(cursor+i) % 16] = tmp[(cursor-shift+i) / 16][(cursor-shift+i) % 16];
    }
    // filling with '\0' the rest of the line
    for (int i = shift; i < 16; i++) {
        storedSymbols[(cursor+i) / 16][(cursor+i) % 16] = '\0';
    }

    shift = 16 - (cursor % 16);
    int vCursor = cursor + shift;

    while (vCursor < 255) {
        storedSymbols[vCursor / 16][vCursor % 16] = tmp[(vCursor - shift) / 16][(vCursor - shift) % 16];
        vCursor++;
    }

    if (cursor % 16 == 0) {
        bool check = 0;
        for (int i = 0; i < 16; i++) {
            if (storedSymbols[(cursor+i) / 16][(cursor+i) % 16] != '\0') {
                check = 1;
                break;
            }
        }
        if (!check) {
            storedSymbols[cursor / 16][cursor % 16] = '\n';
        }
    }


    updateDisplay(1);
}

// update information on the LCD
// 1 in bottom; 0 in top
void LcdWindow::updateDisplay(bool down){
    clearDisplay();

    int pos = 1;
    // update in bottom direction
    if (down) {
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 16; ++col) {
                if(storedSymbols[(cursor / 16) - pos][col] == '\n') {
                    continue;
                }
                display[row][col]->setText(QString(storedSymbols[(cursor / 16) - pos][col]));
            }
            pos = 0;
        }
    }
    // update in top direction
    else {
        pos = 0;
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 16; ++col) {
                if(storedSymbols[(cursor / 16) + pos][col] == '\n') {
                    continue;
                }
                display[row][col]->setText(QString(storedSymbols[(cursor / 16) + pos][col]));
            }
            pos = 1;
        }
    }
}

// check and set symbol into lcd
// some kind of text editor
void LcdWindow::setSymbol(const char &symbol) {
    qDebug() << "Symbol: " << (int)symbol << "\tCursor: " << cursor << "\tCurrent Symbol: " << (int)storedSymbols[cursor / 16][cursor % 16];

    // Enter
    if (symbol == 0x0d) {
        if (cursor == 256) {
            return;
        }
        shiftSymbols();
    }

    // Up Arrow
    else if (symbol == 0x18) {
        if (cursor / 16 == 0) {
            return;
        }
        int shift = cursor % 16;
        for (int i = cursor-16; i >= cursor-shift-16; i--) {
            if (storedSymbols[i / 16][i % 16] == '\0') {
                continue;
            }

            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
            cursor = i;

            if (cursor % 16 != 15 && storedSymbols[(cursor+1) / 16][(cursor+1) % 16] == '\0' && storedSymbols[cursor / 16][cursor % 16] != '\n') {
                cursor++;
            }

            displayRowPos = 0;
            updateDisplay(0);
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

            return;
        }

    }

    // Down Arrow
    else if (symbol == 0x19) {
        if (cursor / 16 == 15) {
            return;
        }
        int shift = cursor % 16;
        for (int i = cursor+16; i >= cursor-shift+16; i--) {
            if (storedSymbols[i / 16][i % 16] == '\0') {
                continue;
            }

            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
            cursor = i;

            if (cursor % 16 != 15 && storedSymbols[(cursor+1) / 16][(cursor+1) % 16] == '\0' && storedSymbols[cursor / 16][cursor % 16] != '\n') {
                cursor++;
            }

            displayRowPos = 1;
            updateDisplay(1);
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

            return;
        }
    }

    // Right Arrow
    else if (symbol == 0x1a) {
        if (cursor == 256) {
            return;
        }
        else if (cursor == 255 && storedSymbols[cursor / 16][cursor % 16] != '\0') {
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
            cursor++;
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-right: 4px solid red");
            return;
        }
        else if (cursor % 16 == 15 && storedSymbols[(cursor+1) / 16][(cursor+1) % 16] != '\0') {
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
            cursor++;
            displayRowPos = 1;
            updateDisplay(1);
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");
        }
        else if (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\n' && cursor < 240) {
            for (int i = cursor+1; i < 256; i++) {
                if (storedSymbols[i / 16][i % 16] != '\0') {
                    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
                    cursor = i;
                    displayRowPos = 1;
                    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

                    updateDisplay(1);
                    break;
                }
            }

        }
        else if (storedSymbols[cursor / 16][cursor % 16] == '\0') {
            for (int i = cursor+1; i < 256; i++) {
                if (storedSymbols[i / 16][i % 16] != '\0') {
                    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
                    cursor = i;

                    displayRowPos = 1;
                    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

                    updateDisplay(1);
                    break;
                }
            }
        }
        else if (storedSymbols[cursor / 16][cursor % 16] != '\0') {
            // check \n
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
            cursor++;
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");
        }
    }

    // Left Arrow
    else if (symbol == 0x1b) {
        if (cursor == 0) {
            return;
        }
        else if (cursor == 256) {
            cursor--;
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");
        }
        else if (cursor % 16 == 0) {
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
            do {
                cursor--;
                if (cursor == 0 || (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\0')) {
                    break;
                }
            } while(storedSymbols[cursor / 16][cursor % 16] == '\0');

            if (cursor % 16 != 15 && storedSymbols[cursor / 16][cursor % 16] != '\n') {
                cursor++;
            }

            displayRowPos = 0;
            updateDisplay(0);
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");
        }
        else {
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold");
            cursor--;
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");
        }
    }

    // Delete
    else if (symbol == 0x7f) {
        if (cursor >= 255) {
            return;
        }
        if (cursor % 16 != 15 && storedSymbols[cursor / 16][cursor % 16] != '\0') {
            if (storedSymbols[cursor / 16][cursor % 16] == '\n') {
                char tmp[16][16];
                memcpy(tmp, storedSymbols, sizeof(storedSymbols));
                int vCursor = cursor + 16;
                while (vCursor < 255) {
                    storedSymbols[(vCursor-16) / 16][(vCursor-16) % 16] = tmp[vCursor / 16][vCursor % 16];
                    vCursor++;
                }
                // need to update display
                if (displayRowPos == 0) {
                    for (int i = cursor; i < cursor + 16; i++) {
                        if (storedSymbols[i / 16][i % 16] != '\n') {
                            display[0][i % 16]->setText(QString(storedSymbols[i / 16][i % 16]));
                        }
                        else {
                            display[0][i % 16]->setText(QString('\0'));
                        }
                        display[1][(i+16) % 16]->setText(QString(storedSymbols[(i+16) / 16][(i+16) % 16]));
                    }
                }
                else {
                    for (int i = cursor; i < cursor + 16; i++) {
                        if (storedSymbols[i / 16][i % 16] != '\n') {
                            display[1][(i+16) % 16]->setText(QString(storedSymbols[i / 16][i % 16]));
                        }
                        else {
                            display[1][(i+16) % 16]->setText(QString('\0'));
                        }
                    }
                }
                return;
            }

            int shift = 16 - (cursor % 16);
            for (int i = 0; i < shift; i++) {
                storedSymbols[(cursor+i) / 16][(cursor+i) % 16] = storedSymbols[(cursor+i+1) / 16][(cursor+i+1) % 16];
                display[displayRowPos][(cursor+i) % 16]->setText(QString(storedSymbols[(cursor+i+1) / 16][(cursor+i+1) % 16]));
            }
            storedSymbols[(cursor+shift-1) / 16][(cursor+shift-1) % 16] = '\0';
            display[displayRowPos][(cursor+shift-1) % 16]->setText(QString('\0'));
        }
        else if (storedSymbols[(cursor+1) / 16][(cursor+1) % 16] == '\0') {
            int shift = 16 - (cursor % 16);
            if ((cursor + shift) >= 255) {
                return;
            }
            if (storedSymbols[(cursor+shift) / 16][(cursor+shift) % 16] == '\n') {
                char tmp[16][16];
                memcpy(tmp, storedSymbols, sizeof(storedSymbols));
                int vCursor = cursor + 16 + shift;

                if (vCursor >= 255) {
                    return;
                }

                while (vCursor < 255) {
                    storedSymbols[(vCursor-16) / 16][(vCursor-16) % 16] = tmp[vCursor / 16][vCursor % 16];
                    vCursor++;
                }
                // need to update display
                if (displayRowPos == 0) {
                    for (int i = cursor + shift; i < cursor + 16 + shift; i++) {
                        display[1][i % 16]->setText(QString(storedSymbols[i / 16][i % 16]));
                    }
                }
            }
        }

    }
    // Backspace
    else if (symbol == 0x08) {
        if (cursor == 0) {
            return;
        }
        if (cursor == 256) {
            cursor--;
            storedSymbols[cursor / 16][cursor % 16] = '\0';
            display[displayRowPos][cursor % 16]->setText(QString('\0'));
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");
            return;
        }

        if (cursor % 16 != 0) {
            int shift = 16 - (cursor % 16);
            for (int i = cursor-1; i < cursor+shift-1; i++) {
                storedSymbols[i / 16][i % 16] = storedSymbols[(i+1) / 16][(i+1) % 16];
                display[displayRowPos][i % 16]->setText(QString(storedSymbols[i / 16][i % 16]));
            }
            storedSymbols[(cursor+shift-1) / 16][(cursor+shift-1) % 16] = '\0';
            display[displayRowPos][(cursor+shift-1) % 16]->setText(QString('\0'));

            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold;");
            cursor--;

            //if (cursor % 16 == 0 && storedSymbols[(cursor+1) / 16][(cursor+1) % 16] == '\0') {
            //    storedSymbols[cursor / 16][cursor % 16] = '\n';
            //}

            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

        }
        else {
            if (cursor / 16 == 0) {
                return;
            }
            for (int i = cursor-1; i>=cursor-16; i--) {
                if (storedSymbols[i / 16][i % 16] == '\0') {
                    continue;
                }
                if (storedSymbols[i / 16][i % 16] == '\n') {
                    char tmp[16][16];
                    int vCursor = i;

                    memcpy(tmp, storedSymbols, sizeof(storedSymbols));

                    if (vCursor >= 255 || vCursor < 0) {
                        return;
                    }

                    while (vCursor+16 < 255) {
                        storedSymbols[vCursor / 16][vCursor % 16] = tmp[(vCursor+16) / 16][(vCursor+16) % 16];
                        vCursor++;
                    }

                    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold;");
                    cursor = i;
                    displayRowPos = 0;
                    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

                    // need to update display
                    updateDisplay(0);

                    return;
                }
                else {
                    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold;");
                    cursor = i;
                    displayRowPos = 0;
                    display[displayRowPos][cursor % 16]->setText(QString('\0'));
                    display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

                    storedSymbols[cursor / 16][cursor % 16] = '\0';

                    if (cursor % 16 == 0) {
                        storedSymbols[cursor / 16][cursor % 16] = '\n';
                    }

                    // need to update display
                    updateDisplay(0);

                    return;
                }
            }
        }
    }
    // Other allowed symbols
    else if (symbol >= 0x20 && symbol <= 0x7e) {
        // qDebug() << "Symbol: " << symbol << "; Hex: " << hex << (int)symbol;

        // check forbidden cursor pos
        if (cursor == 256){
            return;
        }
        // check last cursor pos
        if (cursor == 255) {
            storedSymbols[cursor / 16][cursor % 16] = symbol;

            // update display
            display[displayRowPos][cursor % 16]->setText(QString(symbol));
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-right: 4px solid red");

            // update cursor
            cursor++;
            return;
        }
        // check for last position
        if (cursor % 16 == 15){
            // update all stored chars
            storedSymbols[cursor / 16][cursor % 16] = symbol;

            // update display
            display[displayRowPos][cursor % 16]->setText(QString(symbol));
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold;");

            // update cursors
            cursor++;
            displayRowPos = 1;

            if (cursor < 256 && storedSymbols[cursor / 16][cursor % 16] == '\0') {
                storedSymbols[cursor / 16][cursor % 16] = '\n';
            }

            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");

            updateDisplay(1);
        }

        else {
            storedSymbols[cursor / 16][cursor % 16] = symbol;

            display[displayRowPos][cursor % 16]->setText(QString(symbol));
            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold;");

            cursor++;

            display[displayRowPos][cursor % 16]->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold; border-left: 4px solid red");
        }


    }
}

void LcdWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QGridLayout *gridLayout = new QGridLayout(centralWidget);

    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 16; ++col) {
            QLabel *cell = new QLabel;
            cell->setAutoFillBackground(true);
            cell->setStyleSheet("background-color: black; color: green; font-family: Courier New; font-size: 40px; font-weight: bold;");
            cell->setAlignment(Qt::AlignCenter);

            display[row][col] = cell;

            gridLayout->addWidget(cell, row, col);
        }
    }

    centralWidget->setLayout(gridLayout);
}

void LcdWindow::closeEvent(QCloseEvent *event){
    emit closeLcd();
}
