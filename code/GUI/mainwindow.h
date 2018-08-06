#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"firstgantt.h"
#include <QMainWindow>
#include<QTextEdit>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void openFile();
	void SaveFile();

	QAction *openAction, *saveFile, *saveGantt, *saveGantt2;
	QTextEdit *textEdit, *t2;
	FirstGantt *firstGantt;

};

#endif // MAINWINDOW_H
