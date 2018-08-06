#include "mainwindow.h"

#include<QVBoxLayout>
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

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{


//	building menu
	openAction = new QAction(QIcon(":/image/doc-open"), tr("打开文件"), this);
	openAction->setShortcuts(QKeySequence::Open);

	saveFile = new QAction(QIcon(":/image/doc-save"), tr("保存输出文件"), this);
	saveFile->setShortcut(QKeySequence::Save);

//	QMenu *menu = menuBar()->addMenu(tr("文件(&F)"));
//	menu->addAction(openAction);
//	menu->addAction(saveFile);

	connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
//	connect(saveFile, &QAction::triggered, this, &MainWindow::saveFile);

//	textEdit = new QTextEdit();
//	t2 = new QTextEdit(this);
//	firstGantt = new FirstGantt(this);


//	add layout

	QVBoxLayout *layout = new QVBoxLayout();

	QPushButton *t1 = new QPushButton("wtf", this);
	QPushButton *t2 = new QPushButton("hehe", this);

//	layout->addWidget(menu);
	layout->addWidget(t1);
	layout->addWidget(t2);
//	layout->addWidget(textEdit);
	setLayout(layout);

	setWindowTitle(tr("job shop"));
	resize(1600, 900);
}

MainWindow::~MainWindow()
{

}


void MainWindow::openFile()
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
		textEdit->setText(in.readAll());
		file.close();
	}
}
