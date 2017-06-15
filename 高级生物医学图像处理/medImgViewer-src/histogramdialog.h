#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QMainWindow>
#include <QWidget>
#include <QVTKWidget.h>
#include <QLayout>
#include "itkImage.h"

typedef itk::Image<unsigned char, 2> ImageType;
typedef itk::Image<float, 2> ImageTypeProcess;

class histogramDialog : public QMainWindow
{
    Q_OBJECT
public:
	explicit histogramDialog(ImageType::Pointer itkImg, QWidget *parent = 0);

private:

signals:

public slots:


};

#endif // HISTOGRAMDIALOG_H
