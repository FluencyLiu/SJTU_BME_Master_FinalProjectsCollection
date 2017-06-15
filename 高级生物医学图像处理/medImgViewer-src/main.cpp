#include "mainwindow.h"
#include "viewer.h"
#include "tabboard.h"
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QSplitter>
#include <QImage>
#include <QLabel>

#include <QTextEdit>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFont font("helianthus_lu", 12);
    app.setFont(font);
    MainWindow w;
    w.setWindowTitle("Medical Image Viewer");

    //Specify widgets of the left part.
    tabBoard *tabPanel = new tabBoard();
    tabPanel->setFixedWidth(300);
    tabPanel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    w.lLayout = new QVBoxLayout;
    w.lLayout->addWidget(tabPanel);
    w.leftWidget->setLayout(w.lLayout);

    //Specify layout of the right parrts.
    QWidget *subWidget1 = new QWidget(w.rightWidget);
    QHBoxLayout *subLayout1 = new QHBoxLayout;
    QWidget *subWidget2 = new QWidget(w.rightWidget);
    QHBoxLayout *subLayout2 = new QHBoxLayout;
    w.rLayout = new QVBoxLayout;
    w.rLayout->addWidget(subWidget1);
    w.rLayout->addWidget(subWidget2);
    w.rightWidget->setLayout(w.rLayout);

    //Specify 4 subwindows of this app.
	QImage sourceImg("E:\\Code\\medImgViewer\\medImgViewer-qt5\\origin.jpg");
	QImage image = sourceImg.convertToFormat(QImage::Format::Format_Grayscale8);
    viewer* originViewer = new viewer(subWidget1);
    originViewer->setAlignment(Qt::AlignCenter);
    //originViewer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    originViewer->setFrameShape(QFrame::Box);
    originViewer->setAcceptDrops(true);
    originViewer->loadImg(&image);

    viewer* viewer1 = new viewer(subWidget1);
    viewer1->setAlignment(Qt::AlignCenter);
    //viewer1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    viewer1->setFrameShape(QFrame::Box);
    viewer1->loadImg(&image);
	viewer1->setProcessType(0);

    viewer* viewer2 = new viewer(subWidget2);
    viewer2->setAlignment(Qt::AlignCenter);
    //viewer2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    viewer2->setFrameShape(QFrame::Box);
    viewer2->loadImg(&image);
	viewer2->setProcessType(1);

    viewer* viewer3 = new viewer(subWidget2);
    viewer3->setAlignment(Qt::AlignCenter);
    //viewer3->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    viewer3->setFrameShape(QFrame::Box);
    viewer3->loadImg(&image);
	viewer3->setProcessType(2);

    subLayout1->addWidget(originViewer);
    subLayout1->addWidget(viewer1);
    subLayout2->addWidget(viewer2);
    subLayout2->addWidget(viewer3);
    subWidget1->setLayout(subLayout1);
    subWidget2->setLayout(subLayout2);

    //splitter seeting.
    w.splitter->setStretchFactor(0, 0);
    w.splitter->setStretchFactor(1, 1);
    w.splitter->setOpaqueResize(false);
    w.splitter->update();
    w.splitter->show();

    //specify signal and slot connection.
	//load new images
    QObject::connect(originViewer, SIGNAL(imgChange(QImage*)), viewer1,
		SLOT(reloadImg(QImage*)));
	QObject::connect(originViewer, SIGNAL(imgChange(QImage*)), viewer2,
		SLOT(reloadImg(QImage*)));
	QObject::connect(originViewer, SIGNAL(imgChange(QImage*)), viewer3,
		SLOT(reloadImg(QImage*)));
	//show histogram
	QObject::connect(tabPanel->histogramButton, SIGNAL(clicked()), originViewer,
		SLOT(sendItk()));
	QObject::connect(originViewer, SIGNAL(itkInfo(ImageType::Pointer)), &w,
		SLOT(showHistogram(ImageType::Pointer)));
	//thresholding
	QObject::connect(tabPanel->slider, SIGNAL(valueChanged(int)), viewer1,
		SLOT(thresholding(int)));
	QObject::connect(tabPanel->thresholdButton1, SIGNAL(clicked()), viewer1,
		SLOT(thresholding()));
	QObject::connect(tabPanel->thresholdButton2, SIGNAL(clicked()), viewer2,
		SLOT(thresholding()));
	QObject::connect(tabPanel->thresholdButton3, SIGNAL(clicked()), viewer3,
		SLOT(thresholding()));
	//reload image
	QObject::connect(tabPanel->reloadButton, SIGNAL(clicked()), originViewer,
		SLOT(sendReload()));
	QObject::connect(originViewer, SIGNAL(reloadInfo(QImage*)), viewer1,
		SLOT(reloadImg(QImage*)));
	QObject::connect(originViewer, SIGNAL(reloadInfo(QImage*)), viewer2,
		SLOT(reloadImg(QImage*)));
	QObject::connect(originViewer, SIGNAL(reloadInfo(QImage*)), viewer3,
		SLOT(reloadImg(QImage*)));
	//convolution
	QObject::connect(tabPanel->edgeButton, SIGNAL(clicked()), viewer1,
		SLOT(edgeDetect()));
	QObject::connect(tabPanel->edgeButton, SIGNAL(clicked()), viewer2,
		SLOT(edgeDetect()));
	QObject::connect(tabPanel->edgeButton, SIGNAL(clicked()), viewer3,
		SLOT(edgeDetect()));
	QObject::connect(tabPanel->denoiseButton, SIGNAL(clicked()), viewer2,
		SLOT(denosing()));
	QObject::connect(tabPanel->denoiseButton, SIGNAL(clicked()), viewer3,
		SLOT(denosing()));
	//erode 
	QObject::connect(tabPanel->erodeButton, SIGNAL(clicked()), viewer1,
		SLOT(erode()));
	QObject::connect(tabPanel->erodeButton, SIGNAL(clicked()), viewer2,
		SLOT(erode()));
	QObject::connect(tabPanel->erodeButton, SIGNAL(clicked()), viewer3,
		SLOT(erode()));
	//dilate
	QObject::connect(tabPanel->dilateButton, SIGNAL(clicked()), viewer1,
		SLOT(dilate()));
	QObject::connect(tabPanel->dilateButton, SIGNAL(clicked()), viewer2,
		SLOT(dilate()));
	QObject::connect(tabPanel->dilateButton, SIGNAL(clicked()), viewer3,
		SLOT(dilate()));
	//open
	QObject::connect(tabPanel->openButton, SIGNAL(clicked()), viewer1,
		SLOT(openOpr()));
	QObject::connect(tabPanel->openButton, SIGNAL(clicked()), viewer2,
		SLOT(openOpr()));
	QObject::connect(tabPanel->openButton, SIGNAL(clicked()), viewer3,
		SLOT(openOpr()));
	//close
	QObject::connect(tabPanel->closeButton, SIGNAL(clicked()), viewer1,
		SLOT(closeOpr()));
	QObject::connect(tabPanel->closeButton, SIGNAL(clicked()), viewer2,
		SLOT(closeOpr()));
	QObject::connect(tabPanel->closeButton, SIGNAL(clicked()), viewer3,
		SLOT(closeOpr()));
	//Binary mophology
	QObject::connect(tabPanel->binaryDTButton, SIGNAL(clicked()), viewer1,
		SLOT(binaryDistanceTrans()));
	QObject::connect(tabPanel->binaryDTButton, SIGNAL(clicked()), viewer2,
		SLOT(binaryDistanceTrans()));
	QObject::connect(tabPanel->binaryDTButton, SIGNAL(clicked()), viewer3,
		SLOT(binaryDistanceTrans()));
	QObject::connect(tabPanel->skeletonButton, SIGNAL(clicked()), viewer1,
		SLOT(skeletonize()));
	QObject::connect(tabPanel->skeletonButton, SIGNAL(clicked()), viewer2,
		SLOT(skeletonize()));
	QObject::connect(tabPanel->skeletonButton, SIGNAL(clicked()), viewer3,
		SLOT(skeletonize()));
	QObject::connect(tabPanel->restorteButton, SIGNAL(clicked()), viewer1,
		SLOT(skRestorate()));
	QObject::connect(tabPanel->restorteButton, SIGNAL(clicked()), viewer2,
		SLOT(skRestorate()));
	QObject::connect(tabPanel->restorteButton, SIGNAL(clicked()), viewer3,
		SLOT(skRestorate()));
	//Complex morphology
	QObject::connect(tabPanel->morEdgeButton, SIGNAL(clicked()), viewer1,
		SLOT(morphEdge()));
	QObject::connect(tabPanel->morEdgeButton, SIGNAL(clicked()), viewer2,
		SLOT(morphEdge()));
	QObject::connect(tabPanel->morEdgeButton, SIGNAL(clicked()), viewer3,
		SLOT(morphEdge()));
	QObject::connect(tabPanel->morGradientButton, SIGNAL(clicked()), viewer1,
		SLOT(morphEdge()));
	QObject::connect(tabPanel->morGradientButton, SIGNAL(clicked()), viewer2,
		SLOT(morphEdge()));
	QObject::connect(tabPanel->morGradientButton, SIGNAL(clicked()), viewer3,
		SLOT(morphEdge()));
	//Reconstraction
	QObject::connect(tabPanel->condDilateButton, SIGNAL(clicked()), viewer1,
		SLOT(sendMask()));
	QObject::connect(tabPanel->grayRestorateButton, SIGNAL(clicked()), originViewer,
		SLOT(sendMask()));
	QObject::connect(viewer1, SIGNAL(maskInfo(ImageType::Pointer, ImageTypeProcess::Pointer)), viewer2,
		SLOT(maskDilate(ImageType::Pointer, ImageTypeProcess::Pointer)));
	QObject::connect(viewer1, SIGNAL(maskInfo(ImageType::Pointer, ImageTypeProcess::Pointer)), viewer3,
		SLOT(maskDilate(ImageType::Pointer, ImageTypeProcess::Pointer)));
	QObject::connect(originViewer, SIGNAL(maskInfo(ImageType::Pointer, ImageTypeProcess::Pointer)), viewer2,
		SLOT(maskDilate(ImageType::Pointer, ImageTypeProcess::Pointer)));
	QObject::connect(originViewer, SIGNAL(maskInfo(ImageType::Pointer, ImageTypeProcess::Pointer)), viewer3,
		SLOT(maskDilate(ImageType::Pointer, ImageTypeProcess::Pointer)));
	
    w.show();
    return app.exec();
}
