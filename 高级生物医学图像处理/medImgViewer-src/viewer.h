#ifndef VIEWER_H
#define VIEWER_H

#include <QLabel>
#include <QImage>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QList>
#include <QDebug>
#include <QPixmap>
#include <string>

#include "itkImage.h"

typedef itk::Image<unsigned char, 2> ImageType;
typedef itk::Image<float, 2> ImageTypeProcess;

class viewer : public QLabel
{
    Q_OBJECT

public:
    explicit viewer(QWidget *parent = 0);
    ~viewer();
	QImage *img = new QImage();
	ImageType::Pointer itkImg;
	ImageTypeProcess::Pointer itkImgProcess;
	ImageType::Pointer itkDTM = ImageType::New();
    QPixmap pixMap;
    int processType;
	bool haveDTM = false;

    void loadImg(QImage *sourceImg);
    void setProcessType(int processType);
    void viewer::resizeEvent(QResizeEvent *event);
    void viewer::dragEnterEvent(QDragEnterEvent *event);
    void viewer::dropEvent(QDropEvent *event);

	void refreshImg(QImage *newImg);

public slots:
	void reloadImg(QImage *newImg);
	void sendReload();
	void thresholding();
	void thresholding(int thresholdValue);
	void edgeDetect();
	void denosing();
	void erode();
	void dilate();
	void openOpr();
	void closeOpr();
	void binaryDistanceTrans();
	void skeletonize();
	void skRestorate();
	void morphEdge();
	void sendMask();
	void sendItk();
	void maskDilate(ImageType::Pointer itkMaskImg, ImageTypeProcess::Pointer itkMaskImgProcess);

signals:
	void imgChange(QImage *newImg);
	void reloadInfo(QImage *newImg);
	void maskInfo(ImageType::Pointer itkMaskImg, ImageTypeProcess::Pointer itkMaskImgProcess);
	void itkInfo(ImageType::Pointer itkImg);

private:
	ImageType::Pointer qImg2itkImg(QImage *qImg);
	QImage itk2qImg(ImageType::Pointer itkImg);
	ImageTypeProcess::Pointer itkShow2process(ImageType::Pointer itkImg);
	ImageType::Pointer itkProcess2show(ImageTypeProcess::Pointer itkImgProcess);
	bool viewer::isThreshold(ImageType::Pointer itkImg);
	bool viewer::isImgEmpty(ImageType::Pointer itkImg);

};

#endif // VIEWER_H
