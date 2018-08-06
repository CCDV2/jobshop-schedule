//#include "mainwindow.h"
#include"mainwidget.h"
#include <QApplication>
#include<ctime>
#include<cstdlib>

int main(int argc, char *argv[])
{
	srand(time(NULL));
	QApplication a(argc, argv);
	MainWidget w;
	w.show();

	return a.exec();
}
