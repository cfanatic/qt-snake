#include "snake.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("res/snake.icns"));

    Snake w;
    w.show();

    return a.exec();
}
