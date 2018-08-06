#include "firstgantt.h"
#include<QPainter>
#include "src/datastructure.h"
#include<QTime>
#include<QString>

// initialize the window
FirstGantt::FirstGantt(QWidget *parent) :
	QWidget(parent)
{
    setWindowTitle(tr("gantt"));

	firstDraw = true;
	startRepair = false;
}

// set the size of window
void FirstGantt::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
    if(currentT != 0)
    {
		if(startRepair)
		{
			painter.drawPixmap(0, 0, this->width(), this->height(),
							   prePix, 0, 0, prePix.width(), prePix.height());
			return;
		}
		if(firstDraw)
		{
			firstDraw = false;
			xRange = currentT + 50; yRange = 40*machineNumber+10;
		}
		paintGantt();
    }
    else
	{
		painter.setWindow(0, 0, xRange, yRange);
		painter.fillRect(0, 0, xRange, yRange, Qt::white);
    }
}

// draw gantt chart
void FirstGantt::paintGantt()
{
	QPainter painter(this);
	painter.setWindow(0, 0, xRange, yRange);
	painter.fillRect(0, 0, xRange, yRange, Qt::white);
//	painter.setPen(QPen(Qt::black,3));
	painter.drawLine(20,2,20,yRange - 8);
	painter.drawLine(20,yRange - 8,xRange,yRange - 8);
//	painter.drawLine(xRange - 30, 2, xRange - 30, yRange - 8);
    QBrush brush(QColor(color[0].r,color[0].g,color[0].b),Qt::SolidPattern);
//    painter.setPen(QPen(Qt::black,1));
	painter.setBrush(brush);
	painter.translate(22, 10);
    for(int i = 0; i < machineNumber; i++)
    {
        MachineSequence *p = &machineSequence[i];
        int belongJob = 0;
		QString jobnum = QString::number(belongJob, 10);
        for(int j = 1; j <= p->length; j++)
        {
            belongJob = jobVertex[p->no[j]].belong_job;
			QString jobnum = QString::number(belongJob - 1, 10);
            brush.setColor(QColor(color[belongJob].r,color[belongJob].g,color[belongJob].b));
			painter.fillRect(QRect(jobVertex[p->no[j]].current_time,
                             40*i,
                             jobEdge[p->no[j]].duration,
							 20),brush);
            painter.drawText(jobVertex[p->no[j]].current_time + jobEdge[p->no[j]].duration / 2 - 2,
                             40 * i + 15,
							 jobnum);
        }
    }
}

void FirstGantt::_startRepair()
{
	prePix = this->grab();
	startRepair = true;
}
