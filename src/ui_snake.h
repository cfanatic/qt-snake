/********************************************************************************
** Form generated from reading UI file 'snake.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNAKE_H
#define UI_SNAKE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Snake
{
public:
    QLabel *labelCounter;
    QLabel *labelTime;

    void setupUi(QWidget *Snake)
    {
        if (Snake->objectName().isEmpty())
            Snake->setObjectName(QStringLiteral("Snake"));
        Snake->resize(400, 420);
        labelCounter = new QLabel(Snake);
        labelCounter->setObjectName(QStringLiteral("labelCounter"));
        labelCounter->setGeometry(QRect(0, 398, 400, 16));
        labelCounter->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setBold(true);
        labelCounter->setFont(font);
        labelTime = new QLabel(Snake);
        labelTime->setObjectName(QStringLiteral("labelTime"));
        labelTime->setGeometry(QRect(0, 150, 400, 90));
        font.setPointSize(32);
        font.setBold(true);
        font.setWeight(75);
        labelTime->setFont(font);
        labelTime->setAlignment(Qt::AlignCenter);

        retranslateUi(Snake);

        QMetaObject::connectSlotsByName(Snake);
    } // setupUi

    void retranslateUi(QWidget *Snake)
    {
        Snake->setWindowTitle(QApplication::translate("Snake", "Snake", 0));
        labelCounter->setText(QApplication::translate("Snake", "0", 0));
        labelTime->setText(QApplication::translate("Snake", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class Snake: public Ui_Snake {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNAKE_H
