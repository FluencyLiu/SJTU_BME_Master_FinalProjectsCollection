#include "tabboard.h"
#include "QIntValidator"

tabBoard::tabBoard(QWidget *parent) : QTabWidget(parent)
{
	lineEdit->setValidator(new QIntValidator(0, 255, this));
	slider->setMinimum(0);
	slider->setMaximum(255);
	slider->setValue(50);
	connect(this->slider, SIGNAL(valueChanged(int)), this, SLOT(setLineEditValue(int)));
	connect(this->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(setSiderValue(QString)));
	this->widgetLayout->addWidget(lineEdit);
	this->widgetLayout->addWidget(slider);
	this->container->setLayout(this->widgetLayout);
	this->pageLayout1->addWidget(this->container);
	this->pageLayout1->addWidget(histogramButton);
	this->pageLayout1->addWidget(thresholdButton1);
	this->pageLayout1->addWidget(thresholdButton2);
	this->pageLayout1->addWidget(thresholdButton3);
	this->page1->setLayout(this->pageLayout1);

	this->pageLayout2->addWidget(reloadButton);
	this->pageLayout2->addWidget(edgeButton);
	this->pageLayout2->addWidget(denoiseButton);
	this->page2->setLayout(this->pageLayout2);

	this->pageLayout3->addWidget(erodeButton);
	this->pageLayout3->addWidget(dilateButton);
	this->pageLayout3->addWidget(openButton);
	this->pageLayout3->addWidget(closeButton);
	this->page3->setLayout(this->pageLayout3);

	this->pageLayout4->addWidget(binaryDTButton);
	this->pageLayout4->addWidget(skeletonButton);
	this->pageLayout4->addWidget(restorteButton);
	this->pageLayout4->addWidget(morEdgeButton);
	this->pageLayout4->addWidget(morGradientButton);
	this->pageLayout4->addWidget(condDilateButton);
	this->pageLayout4->addWidget(grayRestorateButton);
	this->page4->setLayout(this->pageLayout4);

    this->addTab(page1, "Page1");
    this->addTab(page2, "Page2");
	this->addTab(page3, "Page3");
	this->addTab(page4, "Page4");

    this->show();
}

void tabBoard::setLineEditValue(int value)
{
	QString str = QString("%1").arg(value);
	this->lineEdit->setText(str);
}

void tabBoard::setSiderValue(QString text)
{
	int value = text.toInt();
	this->slider->setValue(value);
}