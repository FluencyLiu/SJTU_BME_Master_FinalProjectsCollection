#ifndef TABBOARD_H
#define TABBOARD_H

#include <QTabWidget>
#include <QLayout>
#include <QPushButton>
#include <QLineEdit> 
#include <QSlider>

class tabBoard : public QTabWidget
{
    Q_OBJECT

public:
    explicit tabBoard(QWidget *parent = 0);
    QWidget *page1 = new QWidget();
    QVBoxLayout *pageLayout1 = new QVBoxLayout();
	QHBoxLayout *widgetLayout = new QHBoxLayout();
	QWidget *container = new QWidget();
	QLineEdit *lineEdit = new QLineEdit("50");
	QSlider *slider = new QSlider(Qt::Horizontal);
	QPushButton *histogramButton = new QPushButton("Show Grayscale Histogram");
	QPushButton *thresholdButton1 = new QPushButton("Binary Threshold");
	QPushButton *thresholdButton2 = new QPushButton("Otsu");
	QPushButton *thresholdButton3 = new QPushButton("Entropy");

    QWidget *page2 = new QWidget();
	QVBoxLayout *pageLayout2 = new QVBoxLayout();
	QPushButton *reloadButton = new QPushButton("Reload");
	QPushButton *edgeButton = new QPushButton("Edge Detection");
	QPushButton *denoiseButton = new QPushButton("Denoising");

	QWidget *page3 = new QWidget();
	QVBoxLayout *pageLayout3 = new QVBoxLayout();
	QPushButton *erodeButton = new QPushButton("Erosion");
	QPushButton *dilateButton = new QPushButton("Dilation");
	QPushButton *openButton = new QPushButton("Opening");
	QPushButton *closeButton = new QPushButton("Closing");

	QWidget *page4 = new QWidget();
	QVBoxLayout *pageLayout4 = new QVBoxLayout();
	QPushButton *binaryDTButton = new QPushButton("Distance Transform");
	QPushButton *skeletonButton = new QPushButton("Skeletonize");
	QPushButton *restorteButton = new QPushButton("Binary Restoration");
	QPushButton *morEdgeButton = new QPushButton("Edge Detection");
	QPushButton *morGradientButton = new QPushButton("Morphological Gradient");
	QPushButton *condDilateButton = new QPushButton("Conditional Dilation ");
	QPushButton *grayRestorateButton = new QPushButton("Grayscale Restoration");

signals:

public slots :
	void setLineEditValue(int value);
	void setSiderValue(QString text);

};

#endif // TABBOARD_H