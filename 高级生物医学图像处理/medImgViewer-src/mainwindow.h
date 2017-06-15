#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QWidget>
#include <QLayout>
#include "itkImage.h"
#include "histogramdialog.h"

namespace Ui {
class MainWindow;
}

typedef itk::Image<unsigned char, 2> ImageType;
typedef itk::Image<float, 2> ImageTypeProcess;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSplitter* splitter;
    QWidget *mainWidget;
    QWidget *leftWidget;
    QWidget *rightWidget;
    QVBoxLayout *lLayout;
    QVBoxLayout *rLayout;

public slots:
	void showHistogram(ImageType::Pointer itkImg);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
