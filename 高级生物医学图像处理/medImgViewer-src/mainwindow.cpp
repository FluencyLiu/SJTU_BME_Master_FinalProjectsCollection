#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>
#include <QWidget>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setMinimumSize(800, 500);

    this->mainWidget = new QWidget(this);
    this->setCentralWidget(mainWidget);
    this->mainWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout* layout = new QHBoxLayout(this);
    splitter = new QSplitter(Qt::Horizontal, this->mainWidget);
    splitter->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    splitter->show();
    layout->addWidget(splitter);

    this->leftWidget = new QWidget(splitter);
    this->rightWidget = new QWidget(splitter);
    this->leftWidget->setGeometry(0,0,300,500);
    this->rightWidget->setGeometry(300,0,500,500);
    this->splitter->childrenCollapsible();
    //this->leftWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //this->rightWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    mainWidget->setLayout(layout);
    this->update();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showHistogram(ImageType::Pointer itkImg)
{
	histogramDialog *hisDialog = new histogramDialog(itkImg);
	hisDialog->show();
}