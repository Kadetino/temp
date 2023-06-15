#ifndef LcdWindow_H
#define LcdWindow_H

#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>

class LcdWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LcdWindow(QWidget *parent = nullptr);
    ~LcdWindow();

public slots:
    void clearDisplay();
    void setSymbol(const char &symbol);
    void updateDisplay(bool down);

protected:
  void closeEvent(QCloseEvent *event);

private:
    void setupUi();
    void shiftSymbols();

    QLabel *display[2][16];
    int displayRowPos;

    char storedSymbols[16][16];
    int cursor;

signals:
       void closeLcd();
};

#endif
