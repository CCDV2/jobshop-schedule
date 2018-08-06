#ifndef SECONDGANTT_H
#define SECONDGANTT_H

#include <QWidget>
#include<QPainter>
#include"src/datastructure.h"
#include<QPicture>
#include<QImage>
#include<QPushButton>

class SecondGantt:
	public QWidget
{
public:
    SecondGantt(QWidget *parent = 0);

    void calculateColor();
    void startPaint();
	void loopPaint(QTimer *timer, QPushButton *start_repair, QPushButton *confirm);


protected:
	void paintEvent(QPaintEvent *);
private:
	void paintGantt();

	bool fst;
	int xRange = 100, yRange = 100;
	QPixmap prePix;
	QImage *preImg;
};



#endif // SECONDGANTT_H
