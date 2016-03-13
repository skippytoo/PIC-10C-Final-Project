// @name Alice Qin
// @date 01/29/2016
// @file main.cpp
// @brief This is the default program. Sets up main window and executes it



#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
