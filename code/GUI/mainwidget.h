#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include"firstgantt.h"
#include"secondgantt.h"
#include"src/input.h"
#include"src/giffler.h"
#include"src/output.h"

#include <QWidget>
#include<QTextEdit>
#include<QLCDNumber>
#include<QPushButton>
#include<QLabel>
#include<QTimer>
#include<QTime>


extern void inputData();
extern void findOneSolution();

extern int currentT;
extern JobEdge jobEdge[];



class MainWidget : public QWidget
{
	Q_OBJECT
public:
	MainWidget(QWidget *parent = 0);
	void redraw();	
private slots:
	void openFile();
	void saveFile();
	void saveFile2();
	void runSolution();
	void _saveGantt();
	void _saveGantt2();

private:
	void beginTime();
	void loopTime();
	void readjustTime();
	void maximizeFirstGantt();

	QAction *openAction, *saveAction, *saveAction2, *saveGantt, *saveGantt2;
	QTextEdit *inputText, *outputText;
	FirstGantt *firstGantt, *max1;
	SecondGantt *secondGantt, *max2;
	QPushButton *confirm_btn, *start_repair, *submit_repair;
	QLabel *currentTime, *currentTime2;
	QTimer *timer;
	int elapsedTime;
};

#include"src/tabusearch.h"
extern void solve(MainWidget *from);

#endif // MAINWIDGET_H
