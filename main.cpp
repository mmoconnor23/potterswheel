#include "mainwindow.h"
#include <QApplication>

/* By: Melissa O'Connor & Tony Farias
 *Date: February 20th, 2013
 *Description:
 *This library creates different shape objects that
 *extend from a drawable class. These shapes can be constructed,
 *drawn, copied and moved using a GUI.
 *
 */


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
