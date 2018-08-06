#include "mainwidget.h"

#include"src/topograph.h"
#include<QVBoxLayout>
#include<QStackedLayout>
#include<QGridLayout>
#include<QMenuBar>
#include<QAction>
#include<QTextEdit>
#include<QFileDialog>
#include<QIODevice>
#include<QMessageBox>
#include<QTextStream>
#include<QLabel>
#include<QPushButton>
#include<QDebug>
#include<QTime>
#include<QCoreApplication>
#include<QDialog>
#include<string>

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{


//	building menu
	auto menu = new QMenuBar(this);

	openAction = new QAction(QIcon(":/image/doc-open"), tr("打开文件"), menu);
	openAction->setShortcuts(QKeySequence::Open);

	saveAction = new QAction(QIcon(":/image/doc-save"), tr("保存初始文件"), menu);
	saveAction->setShortcut(QKeySequence::Save);

	saveAction2 = new QAction(QIcon(":/image/doc-save"), tr("保存检修后文件"), menu);

	saveGantt = new QAction(QIcon(":/image/doc-save"), tr("保存初始甘特图"), menu);
	saveGantt2 = new QAction(QIcon(":/image/doc-save"), tr("保存检修后甘特图"), menu);

	auto menu_file = menu->addMenu(tr("文件(&F)"));
	menu_file->addAction(openAction);
	menu_file->addAction(saveAction);
	menu_file->addAction(saveAction2);
	connect(openAction, &QAction::triggered, this, &MainWidget::openFile);
	connect(saveAction, &QAction::triggered, this, &MainWidget::saveFile);
	connect(saveAction2, &QAction::triggered, this, &MainWidget::saveFile2);


	auto menu_file2 = menu->addMenu(tr("图片(&P)"));
	menu_file2->addAction(saveGantt);
	menu_file2->addAction(saveGantt2);
	connect(saveGantt, &QAction::triggered, this, &MainWidget::_saveGantt);
	connect(saveGantt2, &QAction::triggered, this, &MainWidget::_saveGantt2);

//	builiding the input text
	inputText = new QTextEdit(this);
	confirm_btn = new QPushButton(tr("提交任务"), this);
	confirm_btn->setFixedWidth(120);
	connect(confirm_btn, &QPushButton::clicked, this, &MainWidget::runSolution);


//	building the first gantt chart
	firstGantt = new FirstGantt(this);
	max1 = new FirstGantt();
	currentTime = new QLabel(tr("End Time: "), this);

	qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
	for(int i=1;i<=34;i++)
	{
        color[i]=Color(qrand()%200 + 28,qrand()%200 + 28,qrand()%200 + 28);
	}
	color[0] = Color(0, 0, 0);
	QPushButton *maxFirst = new QPushButton(tr("最大化"), this);
	maxFirst->setFixedWidth(120);
	connect(maxFirst, &QPushButton::clicked, this, &MainWidget::maximizeFirstGantt);


//	building the second gantt chart
	timer = new QTimer(this);
	secondGantt = new SecondGantt(this);
	start_repair = new QPushButton(tr("启动模拟"), this);
	connect(start_repair, &QPushButton::clicked, this, &beginTime);
	connect(timer, &QTimer::timeout, this, &loopTime);
	start_repair->setFixedWidth(120);
	submit_repair = new QPushButton(tr("提交检修"), this);
	connect(submit_repair, &QPushButton::clicked, this, &readjustTime);
	currentTime2 = new QLabel(tr("当前时间 :"), this);


//	building the output text
	outputText = new QTextEdit(this);


//	combine layout
	QVBoxLayout *v = new QVBoxLayout(this);
	QHBoxLayout *h1 = new QHBoxLayout(this);
	QVBoxLayout *h1_1_v = new QVBoxLayout(this);
	QHBoxLayout *h1_1_v1_h = new QHBoxLayout(this);
	QVBoxLayout *h1_2_v = new QVBoxLayout(this);
	QHBoxLayout *h2 = new QHBoxLayout(this);
	QVBoxLayout *h2_1_v = new QVBoxLayout(this);
	QHBoxLayout *h2_1_v1_h = new QHBoxLayout(this);
	QVBoxLayout *h2_2_v = new QVBoxLayout(this);
	QHBoxLayout *h2_2_v2_h = new QHBoxLayout(this);

	h1_1_v1_h->addWidget(currentTime, 0, Qt::AlignLeft);
	h1_1_v1_h->addWidget(maxFirst, 0, Qt::AlignRight);

	h1_1_v->addLayout(h1_1_v1_h);
	h1_1_v->addWidget(firstGantt, 1);

	h1->addLayout(h1_1_v, 15);
	h1->setMargin(20);
	h1->addStretch(1);
	h1_2_v->addWidget(inputText);
	h1_2_v->addWidget(confirm_btn, 0, Qt::AlignCenter);
	h1->addLayout(h1_2_v, 10);

	h2_1_v1_h->addWidget(currentTime2, 0, Qt::AlignLeft);
	h2_1_v->addLayout(h2_1_v1_h);
	h2_1_v->addWidget(secondGantt, 1);
	h2->addLayout(h2_1_v, 15);
	h2->setMargin(20);
	h2_2_v->addWidget(outputText, 1);

	h2_2_v2_h->addWidget(start_repair, 0, Qt::AlignCenter);
	h2_2_v2_h->addWidget(submit_repair, 0, Qt::AlignCenter);
	h2_2_v->addLayout(h2_2_v2_h);

	h2->addStretch(1);
	h2->addLayout(h2_2_v, 10);

	v->setMenuBar(menu);
	v->addLayout(h1, 1);
	v->addLayout(h2, 1);
	setLayout(v);

	setWindowTitle(tr("车间调度"));
	setWindowIcon(QIcon(":/image/icon"));
	resize(1600, 900);
}

// open file to input process sequences
void MainWidget::openFile()
{
	QString path = QFileDialog::getOpenFileName(this,
												tr("打开文件"),
												".",
												tr("文本文件(*.txt);;所有文件(*.*)")
												);
	if(!path.isEmpty())
	{
		QFile file(path);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(this, tr("错误"),
								 tr("无法打开该文件:\n%1").arg(path));
			return;
		}
		QTextStream in(&file);
		inputText->setText(in.readAll());
		file.close();
	}
}

// save the solution
void MainWidget::saveFile()
{
	QString path = QFileDialog::getSaveFileName(this,
												tr("保存初始文件"),
												".",
												tr("文本文件(*.txt)"));
	if(!path.isEmpty())
	{
		std::string temp = path.toStdString();
		const char *temp2 = temp.c_str();
		freopen(temp2, "w", stdout);
		stdOutput(elapsedTime);
		fflush(stdout);
		fclose(stdout);
	}
}
// save the file after repairing
void MainWidget::saveFile2()
{
	QString path = QFileDialog::getSaveFileName(this,
												tr("保存检修后文件"),
												".",
												tr("文本文件(*.txt)"));
	if(!path.isEmpty())
	{
		std::string temp = path.toStdString();
		const char *temp2 = temp.c_str();
		freopen(temp2, "w", stdout);
		stdOutput2();
		fflush(stdout);
		fclose(stdout);
	}
}

// run to caculate solution
void MainWidget::runSolution()
{
	firstGantt->startRepair = false;
	max1->startRepair = false;
	confirm_btn->setEnabled(false);
	inputText->setEnabled(false);
	start_repair->setEnabled(false);
	QString path = "tempSave.txt";
	QFile file(path);
	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);
	out << inputText->toPlainText();
	file.flush();
	file.close();

	QTime _start;
	_start.start();
	freopen("tempSave.txt", "r", stdin);
	inputData();
	fclose(stdin);
	findOneSolution();
	qDebug() << "first is " << currentT;
	redraw();
	solve(this);
	qDebug() << "Last is" << currentT;
	redraw();
	elapsedTime = _start.elapsed();
	firstGantt->prePix = firstGantt->grab();
	max1->prePix = firstGantt->prePix;
	confirm_btn->setEnabled(true);
	inputText->setEnabled(true);
	start_repair->setEnabled(true);
}

// repaint the first gantt
void MainWidget::redraw()
{
	firstGantt->repaint();
	max1->repaint();
	QString temp = "End Time: " + QString::number(currentT);
	currentTime->setText(temp);
	QTime t;
	t.start();
	while(t.elapsed() < 400)
		QCoreApplication::processEvents();
}

// begin to time
void MainWidget::beginTime()
{
	firstGantt->startRepair = true;
	max1->startRepair = true;
	start_repair->setEnabled(false);
	confirm_btn->setEnabled(false);
	timer->start(TIME_INTERVAL);

	secondGantt->startPaint();

}


// the animation of second gantt
void MainWidget::loopTime()
{
	secondGantt->loopPaint(timer, start_repair, confirm_btn);
	int temp = paintCount / FPS;
	QString modify = tr("当前时间 :") + QString::number(temp);
	currentTime2->setText(modify);
}

// adjust process sequences after joining overhaul time
void MainWidget::readjustTime()
{
	timer->stop();

	QString path = "tempSave2.txt";
	QFile file(path);
	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);
	out << outputText->toPlainText();
	file.flush();
	file.close();
	outputText->clear();

	freopen("tempSave2.txt", "r", stdin);
	readjustEarliestTime(paintCount / FPS);
	fclose(stdin);
	secondGantt->calculateColor();
	timer->start(TIME_INTERVAL);
	secondGantt->repaint();
}

// maximize first gantt
void MainWidget::maximizeFirstGantt()
{
	QDialog *temp = new QDialog(this);
	QVBoxLayout *tempLayout = new QVBoxLayout(temp);
//	temp->setAttribute(Qt::WA_DeleteOnClose);
	temp->resize(1600, 900);
	tempLayout->addWidget(max1);
	temp->setLayout(tempLayout);
	temp->show();

}

// save the first gantt
void MainWidget::_saveGantt()
{
	QString path = QFileDialog::getSaveFileName(this,
												tr("保存初始甘特图"),
												".",
												tr("图像文件(*.png)"));
	if(!path.isEmpty())
	{
		QPixmap temp = firstGantt->grab();
		temp.save(path);
	}
}

// save the second gantt
void MainWidget::_saveGantt2()
{
	QString path = QFileDialog::getSaveFileName(this,
												tr("保存检修后甘特图"),
												".",
												tr("图像文件(*.png)"));
	if(!path.isEmpty())
	{
		QPixmap temp = secondGantt->grab();
		temp.save(path);
	}
}
