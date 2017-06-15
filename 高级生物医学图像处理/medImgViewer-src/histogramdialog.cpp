#include "histogramdialog.h"

#include <itkImageToVTKImageFilter.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkBarChartActor.h>
#include <vtkFieldData.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkImageExtractComponents.h>
#include <vtkIntArray.h>
#include <vtkLegendBoxActor.h>
#include <vtkProperty2D.h>
#include <vtkSmartPointer.h>
#include <vtkImageMagnitude.h>
#include <vtkXYPlotActor.h>

histogramDialog::histogramDialog(ImageType::Pointer itkImg, QWidget *parent) : QMainWindow(parent)
{
	typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(itkImg);
	connector->Update();

	int numComponents = connector->GetOutput()->GetNumberOfScalarComponents();

	vtkSmartPointer<vtkXYPlotActor> plot =
		vtkSmartPointer<vtkXYPlotActor>::New();
	plot->ExchangeAxesOff();
	plot->SetLabelFormat("%g");
	plot->SetXTitle("Intensity");
	plot->SetYTitle("Frequency");
	plot->SetXValuesToValue();

	double xmax = 0.;
	double ymax = 0.;

	double colors[3][3] = {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 }
	};

	const char* labels[3] = { "Red", "Green", "Blue" };
	for (int i = 0; i < numComponents; ++i)
	{
		vtkSmartPointer<vtkImageExtractComponents> extract =
			vtkSmartPointer<vtkImageExtractComponents>::New();
		extract->SetInputData(connector->GetOutput());
		extract->SetComponents(i);
		extract->Update();
		double range[2];
		extract->GetOutput()->GetScalarRange(range);
		vtkSmartPointer<vtkImageAccumulate> histogram =
			vtkSmartPointer<vtkImageAccumulate>::New();
		histogram->SetInputConnection(extract->GetOutputPort());
		histogram->SetComponentExtent(
			0,
			static_cast<int>(range[1]) - static_cast<int>(range[0]) - 1, 0, 0, 0, 0);
		histogram->SetComponentOrigin(range[0], 0, 0);
		histogram->SetComponentSpacing(1, 0, 0);
		int ignoreZero = 0;
		histogram->SetIgnoreZero(ignoreZero);
		histogram->Update();
		if (range[1] > xmax)
		{
			xmax = range[1];
		}
		if (histogram->GetOutput()->GetScalarRange()[1] > ymax)
		{
			ymax = histogram->GetOutput()->GetScalarRange()[1];
		}
		plot->AddDataSetInputConnection(histogram->GetOutputPort());

		if (numComponents > 1)
		{
			plot->SetPlotColor(i, colors[i]);
			plot->SetPlotLabel(i, labels[i]);
			plot->LegendOn();
		}
	}



	plot->SetXRange(0, xmax);
	plot->SetYRange(0, ymax);

	vtkSmartPointer<vtkRenderer> histRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	histRenderer->AddActor(plot);

	this->resize(600, 600);
	QVTKWidget *widget = new QVTKWidget;

	this->setCentralWidget(widget);

	widget->GetRenderWindow()->AddRenderer(histRenderer);
	widget->show();
	this->show();
}