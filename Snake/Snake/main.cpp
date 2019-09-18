#include "snake.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SnakeGame  sg;
    sg.show();
    return a.exec();
}
