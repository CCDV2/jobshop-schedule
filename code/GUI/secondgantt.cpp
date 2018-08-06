#include "secondgantt.h"
#include "src/datastructure.h"
#include"src/topograph.h"
#include<QTimer>
#include<string.h>
#include<QString>
#include<QBrush>

// initialize the window
SecondGantt::SecondGantt(QWidget *parent):
	QWidget(parent)
{
	fst = true;
}

// calculate color of each FPS
void SecondGantt::calculateColor()
{
	int _cl = paintCount / FPS;
	for(int i = 0; i < machineNumber; ++i)
	{
		for(int j = _cl; j <= currentT; ++j)
		{
			paintNumber[i][j] = -1;
		}
	}
    for(int i = 0; i < machineNumber; i++)
    {
        MachineSequence *p = &machineSequence[i];
        for(int j = 1; j <= p->length; j++)
        {
            for(int k = jobVertex[p->no[j]].current_time;
                k <= jobVertex[p->no[j]].current_time + jobEdge[p->no[j]].duration;
                k++)
			{
				if(k == jobVertex[p->no[j]].current_time + jobEdge[p->no[j]].duration
					&& paintNumber[i][k] == 0) continue;
                paintNumber[i][k] = jobVertex[p->no[j]].belong_job;
            }
        }
        if(repairList[i].startTime)
        {
			for(int j = repairList[i].startTime; j < repairList[i].endTime; j++)
            {
                paintNumber[i][j] = 0;
            }
        }
    }
}

// start to paint
void SecondGantt::startPaint()
{
	fst = true;
	memset(repairList, 0, sizeof(repairList));
	memset(paintNumber, -1, sizeof(paintNumber));

	getEarliest();
	for(int i = 1; i <= jobVertexCount; ++i)
		jobVertex[i].current_time = topoVertex[i].earliestTime;
	currentT = topoVertex[jobVertexCount + 1].earliestTime;

	repaint();
	prePix = this->grab();
	fst = false;
	xRange = FPS * (50 + currentT + 200);
	yRange = FPS * (40 * machineNumber + 10);
    calculateColor();
	paintCount = 0;
    repaint();
}

// the animation of second gantt
void SecondGantt::loopPaint(QTimer *timer, QPushButton *start_repair, QPushButton *confirm)
{
	prePix = this->grab();
	paintCount += TIME_MULTIPLE;
    repaint();
	if(paintCount > (currentT) * FPS)
	{
		paintCount -= FPS;
		timer->stop();
		start_repair->setEnabled(true);
		confirm->setEnabled(true);
	}
}

void SecondGantt::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	if(fst)
	{
		painter.setWindow(0, 0, xRange, yRange);
		painter.fillRect(0, 0, xRange, yRange, Qt::white);
	}
	else
	{
		paintGantt();
	}

}

// draw gantt chart
void SecondGantt::paintGantt()
{
	QPainter painter(this);

	painter.setWindow(0, 0, xRange, yRange);
	painter.fillRect(0, 0, xRange, yRange, Qt::white);

	painter.setPen(QPen(Qt::black, FPS));

	painter.drawPixmap(0, 0, xRange, yRange, prePix);
	painter.drawLine(20 * FPS, 2 * FPS, 20 * FPS, yRange - 8 * FPS);
	painter.drawLine(20 * FPS, yRange - 8 * FPS, xRange, yRange - 8 * FPS);


	painter.translate(22 * FPS, 10 * FPS);
	for(int i = 0; i < machineNumber; i++)
	{
        int no = paintNumber[i][paintCount / FPS];
        if(no != -1)
        {
            QBrush brush;
            if(no != 0)
            {
                brush.setStyle(Qt::SolidPattern);
                brush.setColor(QColor(color[no].r, color[no].g, color[no].b));
                painter.setPen(QPen(QColor(color[no].r, color[no].g, color[no].b),1));
                painter.fillRect(QRect(paintCount, 40 * FPS * i, TIME_MULTIPLE, 20 * FPS),
                                 brush);
            }
            else
            {
				brush.setStyle(Qt::Dense3Pattern);
				brush.setColor(QColor(color[no].r, color[no].g, color[no].b));
				painter.setPen(QPen(QColor(color[no].r, color[no].g, color[no].b),1));
				/*painter.fillRect(QRect(paintCount, 40 * FPS * i, TIME_MULTIPLE, 20 * FPS),
                                 brush);
				*/painter.fillRect(QRect(repairList[i].startTime * FPS,
									   40 * FPS * i,
									   (paintCount - repairList[i].startTime * FPS),
									   20 * FPS),
								 brush);
            }
		}
	}
}


