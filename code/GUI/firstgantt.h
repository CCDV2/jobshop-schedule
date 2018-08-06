#ifndef FIRSTGANTT_H
#define FIRSTGANTT_H

#include <QWidget>
#include<QPainter>
#include<QPixmap>

class FirstGantt : public QWidget
{
	Q_OBJECT
public:
	FirstGantt(QWidget *parent = 0);
	void paintGantt();
	void _startRepair();
	bool startRepair;
	QPixmap prePix;

private:
	int xRange = 100, yRange = 100;

	bool firstDraw = true;
	QPainter *painter;

protected:
	void paintEvent(QPaintEvent *);
};

#endif // FIRSTGANTT_H
