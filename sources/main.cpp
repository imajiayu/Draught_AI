#include "AI_draught.h"
#include <QtWidgets/QApplication>
#include "AI_engine.h"
#include <iostream>
#include <cstring>
using namespace std;


extern char INPUT[50];
extern struct Command OUTPUT;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    AI_draught w;
    w.show();
    w.gameLoop();
    return a.exec();
}
