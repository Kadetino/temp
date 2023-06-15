#ifndef LCDINPUT_H
#define LCDINPUT_H

#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

class LcdInput : public QMainWindow
{
    Q_OBJECT

public:
    LcdInput(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        // Create the main widget and layout
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        // Create the text input field
        QLineEdit *inputField = new QLineEdit(this);
        layout->addWidget(inputField);

        // Create the submit button
        QPushButton *submitButton = new QPushButton("Submit", this);
        layout->addWidget(submitButton);

        // Connect the button's clicked signal to a slot
        connect(submitButton, &QPushButton::clicked, this, [inputField, parent = parent]() {
            // Retrieve the entered text
            QString inputText = inputField->text();

            // Convert the text to an integer
            bool ok;
            int inputValue = inputText.toInt(&ok);

            if (ok) {
                // Display the entered integer
                //qDebug() << "Entered integer: " << inputValue;
                QMetaObject::invokeMethod(parent, "setSymbol", Q_ARG(const char&, inputValue));
            } else {
                // Display an error message
                //qDebug() << "Invalid input!";
            }
        });

        // Set the central widget
        setCentralWidget(centralWidget);
    }
};


#endif // LCDINPUT_H
