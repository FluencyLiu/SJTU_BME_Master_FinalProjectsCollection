#include "viewer.h"

#include "itkImageDuplicator.h"
#include "itkImageToHistogramFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkOtsuThresholdImageFilter.h"
#include <itkMaximumEntropyThresholdImageFilter.h>
#include "itkConvolutionImageFilter.h"
#include "itkStatisticsImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkMedianImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include <itkGrayscaleFunctionErodeImageFilter.h>
#include <itkNeighborhood.h>
#include "itkBinaryDilateImageFilter.h"
#include <itkGrayscaleDilateImageFilter.h>
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"
#include "itkFlatStructuringElement.h"
#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itkBinaryThinningImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkSubtractImageFilter.h"
#include <itkBinaryReconstructionByDilationImageFilter.h>
#include <itkReconstructionImageFilter.h>


viewer::viewer(QWidget *parent) :
    QLabel(parent)
{
    this->setAcceptDrops(false);
}

viewer::~viewer(){}

void viewer::loadImg(QImage *sourceImg)
{
	delete this->img;
	QImage *image = new QImage(sourceImg->width(), sourceImg->height(), QImage::Format::Format_Grayscale8);
	*image = sourceImg->convertToFormat(QImage::Format::Format_Grayscale8);
	this->img = image;

	this->pixMap = QPixmap::fromImage(*(this->img));
    QPixmap fitPixMap = pixMap.scaled(this->size(), Qt::KeepAspectRatio);
    this->setPixmap(fitPixMap);
    this->show();

	this->itkImg = qImg2itkImg(this->img);
	this->itkImgProcess = itkShow2process(this->itkImg);
	this->haveDTM = false;
}

void viewer::refreshImg(QImage *newImg)
{
	delete this->img;
	QImage *image = new QImage(newImg->width(), newImg->height(), QImage::Format::Format_Grayscale8);
	*image = newImg->convertToFormat(QImage::Format::Format_Grayscale8);
	this->img = image;

	this->pixMap = QPixmap::fromImage(*(this->img));
	QPixmap fitPixMap = pixMap.scaled(this->size(), Qt::KeepAspectRatio);
	this->setPixmap(fitPixMap);
	this->show();
}

void viewer::setProcessType(int processType)
{
    this->processType = processType;
}

void viewer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QPixmap fitPixMap = pixMap.scaled(this->size(), Qt::KeepAspectRatio);
    this->setPixmap(fitPixMap);
}

void viewer::dragEnterEvent(QDragEnterEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty() || urls.size() > 1)
        return;
    foreach(QUrl url, urls)
    {
        QString file_name = url.toLocalFile();
        std::string urlString = file_name.toStdString();
        std::string suffixStr = urlString.substr(urlString.find_last_of('.') + 1);
		if (suffixStr == "jpg" || suffixStr == "png" || suffixStr == "gif" || suffixStr == "jpeg" || suffixStr == "bmp")
            event->acceptProposedAction();
    }
}

void viewer::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    foreach(QUrl url, urls)
    {
        QString file_name = url.toLocalFile();
        QByteArray qName=file_name.toLatin1();
        char *imgName=qName.data();
		QImage image(imgName);
        this->loadImg(&image);
		emit imgChange(&image);
    }
}

//private function
ImageType::Pointer viewer::qImg2itkImg(QImage *qImg)
{
	ImageType::IndexType start;
	start[0] = start[1] = 0;
	ImageType::SizeType size;
	size[0] = qImg->width();
	size[1] = qImg->height();
	ImageType::RegionType region;
	region.SetSize(size);
	region.SetIndex(start);
	ImageType::IndexType index;
	unsigned char pixValue;

	ImageType::Pointer itkImg = ImageType::New();
	itkImg->SetRegions(region);
	itkImg->Allocate();
	unsigned char *buffer = qImg->bits();

	for (int i = 0; i<size[0]; i++)
	{
		for (int j = 0; j<size[1]; j++)
		{
			index[0] = i;
			index[1] = j;
			pixValue = *(buffer + j*size[0] + i);
			itkImg->SetPixel(index, pixValue);
		}
	}

	return itkImg;
}

QImage viewer::itk2qImg(ImageType::Pointer itkImg)
{
	ImageType::RegionType region = itkImg->GetLargestPossibleRegion();
	ImageType::SizeType size = region.GetSize();
	ImageType::PixelType pixelValue;
	ImageType::IndexType index;
	int qIndex;

	int width = size[0];
	int height = size[1];
	QImage image(width, height, QImage::Format::Format_Grayscale8);
	unsigned char *buffer = image.bits();

	for (int i = 0; i<size[0]; i++)
	{
		for (int j = 0; j<size[1]; j++)
		{
			index[0] = i;
			index[1] = j;
			pixelValue = itkImg->GetPixel(index);
			*(buffer + j*size[0] + i) = pixelValue;
		}
	}

	return image;
}

ImageTypeProcess::Pointer viewer::itkShow2process(ImageType::Pointer itkImg)
{
	ImageType::RegionType regionOri = itkImg->GetLargestPossibleRegion();
	ImageType::SizeType sizeOri = regionOri.GetSize();
	int width = sizeOri[0];
	int height = sizeOri[1];

	ImageTypeProcess::IndexType start;
	start[0] = start[1] = 0;
	ImageTypeProcess::SizeType size;
	size[0] = width;
	size[1] = height;
	ImageTypeProcess::RegionType region;
	region.SetSize(size);
	region.SetIndex(start);
	ImageTypeProcess::Pointer itkImgProcess = ImageTypeProcess::New();
	itkImgProcess->SetRegions(region);
	itkImgProcess->Allocate();

	ImageTypeProcess::PixelType pixelValueProcess;
	ImageTypeProcess::IndexType index;
	ImageType::PixelType pixelValue;
	ImageType::IndexType indexOri;

	for (int i = 0; i<size[0]; i++)
	{
		for (int j = 0; j<size[1]; j++)
		{
			index[0] = i;
			index[1] = j;
			indexOri[0] = i;
			indexOri[1] = j;
			pixelValue = itkImg->GetPixel(indexOri);
			pixelValueProcess = static_cast<ImageTypeProcess::PixelType> (itkImg->GetPixel(indexOri));
			itkImgProcess->SetPixel(index, pixelValueProcess);
		}
	}

	return itkImgProcess;
}

ImageType::Pointer viewer::itkProcess2show(ImageTypeProcess::Pointer itkImgProcess)
{
	/*ImageTypeProcess::RegionType regionProcess = itkImgProcess->GetLargestPossibleRegion();
	ImageTypeProcess::SizeType sizeProcess = regionProcess.GetSize();
	int width = sizeProcess[0];
	int height = sizeProcess[1];

	ImageType::IndexType start;
	start[0] = start[1] = 0;
	ImageType::SizeType size;
	size[0] = width;
	size[1] = height;
	ImageType::RegionType region;
	region.SetSize(size);
	region.SetIndex(start);
	ImageType::Pointer itkImg = ImageType::New();
	itkImg->SetRegions(region);
	itkImg->Allocate();

	ImageType::PixelType pixelValue;
	ImageType::IndexType indexOri;
	ImageTypeProcess::PixelType pixelValueProcess;
	ImageTypeProcess::IndexType index;

	typedef itk::StatisticsImageFilter< ImageTypeProcess > StatisticsFilterType;
	StatisticsFilterType::Pointer statistics = StatisticsFilterType::New();
	statistics->SetInput(itkImgProcess);
	statistics->Update();
	float max = statistics->GetMaximum();
	float min = statistics->GetMinimum();
	float range = max - min;

	for (int i = 0; i<size[0]; i++)
	{
		for (int j = 0; j<size[1]; j++)
		{
			index[0] = i;
			index[1] = j;
			indexOri[0] = i;
			indexOri[1] = j;
			pixelValueProcess = itkImgProcess->GetPixel(index);
			pixelValueProcess = (pixelValueProcess - min) / range * 255;
			pixelValue = static_cast<ImageType::PixelType> (pixelValueProcess);
			itkImg->SetPixel(indexOri, pixelValue);
		}
	}*/

	typedef itk::RescaleIntensityImageFilter< ImageTypeProcess, ImageType > RescaleType;
	RescaleType::Pointer rescaler = RescaleType::New();
	rescaler->SetInput(itkImgProcess);
	rescaler->SetOutputMinimum(0);
	rescaler->SetOutputMaximum(255);
	rescaler->Update();

	return rescaler->GetOutput();
}

//slots
void viewer::reloadImg(QImage *newImg)
{
	loadImg(newImg);
}

void viewer::sendReload()
{
	emit reloadInfo(this->img);
}

// image processing 
bool viewer::isThreshold(ImageType::Pointer itkImg)
{
	ImageType::RegionType region = itkImg->GetLargestPossibleRegion();
	ImageType::SizeType size = region.GetSize();
	ImageType::PixelType pixelValue;
	ImageType::IndexType index;
	int qIndex;

	int width = size[0];
	int height = size[1];

	for (int i = 0; i<size[0]; i++)
	{
		for (int j = 0; j<size[1]; j++)
		{
			index[0] = i;
			index[1] = j;
			pixelValue = itkImg->GetPixel(index);
			if (pixelValue != 0 && pixelValue != 255)
			{
				return false;
			}
		}
	}

	return true;
}

void viewer::thresholding()
{	
	switch (this->processType)
	{
	case 0:
	{
			  this->itkImg = qImg2itkImg(this->img);
			  this->itkImgProcess = itkShow2process(this->itkImg);
			  break;
	}
	case 1:
	{
			  if (!this->isThreshold(this->itkImg))
			  {
				  typedef itk::OtsuThresholdImageFilter <ImageType, ImageType> OtsuFilterType;
				  OtsuFilterType::Pointer otsuFilter = OtsuFilterType::New();
				  otsuFilter->SetInput(this->itkImg);
				  otsuFilter->SetInsideValue(0);
				  otsuFilter->SetOutsideValue(255);
				  otsuFilter->Update();

				  this->itkImg = otsuFilter->GetOutput();
				  this->itkImgProcess = itkShow2process(this->itkImg);
				  QImage image = itk2qImg(this->itkImg);
				  this->refreshImg(&image);
			  }
			  break;
	}
	case 2:
	{
			  if (!this->isThreshold(this->itkImg))
			  {
				  typedef itk::MaximumEntropyThresholdImageFilter <ImageType, ImageType> EntropyFilterType;
				  EntropyFilterType::Pointer entropyFilter = EntropyFilterType::New();
				  entropyFilter->SetInput(this->itkImg);
				  entropyFilter->SetInsideValue(0);
				  entropyFilter->SetOutsideValue(255);
				  entropyFilter->Update();

				  this->itkImg = entropyFilter->GetOutput();
				  this->itkImgProcess = itkShow2process(this->itkImg);
				  QImage image = itk2qImg(this->itkImg);
				  this->refreshImg(&image);
			  }
			  break;
	}
	default:
	{
			   break;
	}
	}
}

void viewer::thresholding(int thresholdValue)
{
	typedef itk::BinaryThresholdImageFilter<ImageType, ImageType>  FilterType;
	FilterType::Pointer filter = FilterType::New();
	filter->SetInput(this->itkImg);
	filter->SetOutsideValue(0);
	filter->SetInsideValue(255);
	filter->SetLowerThreshold(thresholdValue);
	filter->SetUpperThreshold(255);
	filter->Update();
	ImageType::Pointer mediateImg = filter->GetOutput();
	QImage image = itk2qImg(mediateImg);
	this->refreshImg(&image);
}

void viewer::edgeDetect()
{
	ImageTypeProcess::Pointer kernel = ImageTypeProcess::New();
	ImageTypeProcess::IndexType start;
	start.Fill(0);
	ImageTypeProcess::SizeType size;
	size.Fill(3);
	ImageTypeProcess::RegionType region;
	region.SetSize(size);
	region.SetIndex(start);
	kernel->SetRegions(region);
	kernel->Allocate();
	ImageTypeProcess::IndexType index;
	kernel->FillBuffer(0);
	index[0] = 0; index[1] = 0;
	kernel->SetPixel(index, 1);
	index[0] = 0; index[1] = 1;
	kernel->SetPixel(index, 1);
	index[0] = 0; index[1] = 2;
	kernel->SetPixel(index, 1);

	index[0] = 2; index[1] = 0;
	kernel->SetPixel(index, -1);
	index[0] = 2; index[1] = 1;
	kernel->SetPixel(index, -1);
	index[0] = 2; index[1] = 2;
	kernel->SetPixel(index, -1);

	typedef itk::ConvolutionImageFilter<ImageTypeProcess> ConvFilterType;
	ConvFilterType::Pointer convFilter = ConvFilterType::New();
	convFilter->SetInput(this->itkImgProcess);
	convFilter->SetKernelImage(kernel);
	convFilter->Update();
	this->itkImgProcess = convFilter->GetOutput();

	this->itkImg = itkProcess2show(this->itkImgProcess);
	QImage image = itk2qImg(this->itkImg);
	this->refreshImg(&image);
}

void viewer::denosing()
{
	switch (this->processType)
	{
	case 0:
	{
			  break;
	}
	case 1:
	{
			  double variance = 4.0;
			  typedef itk::DiscreteGaussianImageFilter<ImageTypeProcess, ImageTypeProcess> GaussianFilterType;
			  GaussianFilterType::Pointer gaussianFilter = GaussianFilterType::New();
			  gaussianFilter->SetInput(this->itkImgProcess);
			  gaussianFilter->SetVariance(variance);
			  //gaussianFilter->SetMaximumKernelWidth(kernelWidth);
			  gaussianFilter->Update();
			  this->itkImgProcess = gaussianFilter->GetOutput();

			  this->itkImg = itkProcess2show(this->itkImgProcess);
			  QImage image = itk2qImg(this->itkImg);
			  this->refreshImg(&image);
			  break;
	}
	case 2:
	{
			  int radium = 3;
			  typedef itk::MedianImageFilter<ImageTypeProcess, ImageTypeProcess> MedianFilterType;
			  MedianFilterType::Pointer medianFilter = MedianFilterType::New();
			  medianFilter->SetInput(this->itkImgProcess);
			  MedianFilterType::InputSizeType radius;
			  radius.Fill(radium);
			  medianFilter->SetRadius(radius);
			  medianFilter->Update();
			  this->itkImgProcess = medianFilter->GetOutput();

			  this->itkImg = itkProcess2show(this->itkImgProcess);
			  QImage image = itk2qImg(this->itkImg);
			  this->refreshImg(&image);
			  break;
	}
	default:
	{
			   break;
	}
	}
}

void viewer::erode()
{
	if (this->isThreshold(this->itkImg))
	{
		int radium = 1;
		typedef itk::FlatStructuringElement<2> StructuringElementType;
		StructuringElementType::RadiusType elementRadius;
		elementRadius.Fill(radium);
		StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);

		typedef itk::BinaryErodeImageFilter<ImageType, ImageType, StructuringElementType> BinaryErodeImageFilterType;
		BinaryErodeImageFilterType::Pointer binaryErodeFilter = BinaryErodeImageFilterType::New();
		binaryErodeFilter->SetInput(this->itkImg);
		binaryErodeFilter->SetKernel(structuringElement);
		binaryErodeFilter->Update();

		this->itkImg = binaryErodeFilter->GetOutput();
		this->itkImgProcess = itkShow2process(this->itkImg);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
	else
	{
		float kernelValue = 2;

		typedef itk::Neighborhood<float, 2> KernelType;
		KernelType neighborhood;
		neighborhood.SetRadius(kernelValue);
		for (unsigned int i = 0; i < neighborhood.GetSize()[0] * neighborhood.GetSize()[1]; ++i)
		{
			neighborhood[i] = 1;
		}

		typedef itk::GrayscaleFunctionErodeImageFilter< ImageTypeProcess, ImageTypeProcess, KernelType > GrayscaleErodeImageFilterType;
		GrayscaleErodeImageFilterType::Pointer erodeFilter = GrayscaleErodeImageFilterType::New();
		erodeFilter->SetInput(this->itkImgProcess);
		erodeFilter->SetKernel(neighborhood);
		erodeFilter->Update();
		this->itkImgProcess = erodeFilter->GetOutput();

		this->itkImg = itkProcess2show(this->itkImgProcess);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
}

void viewer::dilate()
{
	if (this->isThreshold(this->itkImg))
	{
		int radium = 1;
		typedef itk::FlatStructuringElement<2> StructuringElementType;
		StructuringElementType::RadiusType elementRadius;
		elementRadius.Fill(radium);
		StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);

		typedef itk::BinaryDilateImageFilter <ImageType, ImageType, StructuringElementType> BinaryDilateImageFilterType;
		BinaryDilateImageFilterType::Pointer binaryDilateFilter = BinaryDilateImageFilterType::New();
		binaryDilateFilter->SetInput(this->itkImg);
		binaryDilateFilter->SetKernel(structuringElement);
		binaryDilateFilter->Update();
		this->itkImg = binaryDilateFilter->GetOutput();
		this->itkImgProcess = itkShow2process(this->itkImg);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
	else
	{
		float kernelValue = 3;

		typedef itk::Neighborhood<float, 2> KernelType;
		KernelType neighborhood;
		neighborhood.SetRadius(kernelValue);
		for (unsigned int i = 0; i < neighborhood.GetSize()[0] * neighborhood.GetSize()[1]; ++i)
		{
			neighborhood[i] = 1;
		}

		typedef itk::GrayscaleDilateImageFilter< ImageTypeProcess, ImageTypeProcess, KernelType > GrayscaleDilateImageFilterType;
		GrayscaleDilateImageFilterType::Pointer dilateFilter = GrayscaleDilateImageFilterType::New();
		dilateFilter->SetInput(this->itkImgProcess);
		dilateFilter->SetKernel(neighborhood);
		dilateFilter->Update();
		this->itkImgProcess = dilateFilter->GetOutput();

		this->itkImg = itkProcess2show(this->itkImgProcess);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
}

void viewer::openOpr()
{
	if (this->isThreshold(this->itkImg))
	{
		int radium = 1;
		typedef itk::FlatStructuringElement<2> StructuringElementType;
		StructuringElementType::RadiusType elementRadius;
		elementRadius.Fill(radium);
		StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);
		typedef itk::BinaryMorphologicalOpeningImageFilter <ImageType, ImageType, StructuringElementType>
			BinaryMorphologicalOpeningImageFilterType;
		BinaryMorphologicalOpeningImageFilterType::Pointer openingFilter
			= BinaryMorphologicalOpeningImageFilterType::New();
		openingFilter->SetInput(this->itkImg);
		openingFilter->SetKernel(structuringElement);
		openingFilter->Update();
		this->itkImg = openingFilter->GetOutput();
		this->itkImgProcess = itkShow2process(this->itkImg);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
	else
	{
		float kernelValue = 1;

		typedef itk::Neighborhood<float, 2> KernelType;
		KernelType neighborhood;
		neighborhood.SetRadius(kernelValue);
		for (unsigned int i = 0; i < neighborhood.GetSize()[0] * neighborhood.GetSize()[1]; ++i)
		{
			neighborhood[i] = 1;
		}

		typedef itk::GrayscaleFunctionErodeImageFilter< ImageTypeProcess, ImageTypeProcess, KernelType > GrayscaleErodeImageFilterType;
		GrayscaleErodeImageFilterType::Pointer erodeFilter = GrayscaleErodeImageFilterType::New();
		erodeFilter->SetInput(this->itkImgProcess);
		erodeFilter->SetKernel(neighborhood);
		erodeFilter->Update();
		this->itkImgProcess = erodeFilter->GetOutput();

		typedef itk::GrayscaleDilateImageFilter< ImageTypeProcess, ImageTypeProcess, KernelType > GrayscaleDilateImageFilterType;
		GrayscaleDilateImageFilterType::Pointer dilateFilter = GrayscaleDilateImageFilterType::New();
		dilateFilter->SetInput(this->itkImgProcess);
		dilateFilter->SetKernel(neighborhood);
		dilateFilter->Update();
		this->itkImgProcess = dilateFilter->GetOutput();

		this->itkImg = itkProcess2show(this->itkImgProcess);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
}

void viewer::closeOpr()
{
	if (this->isThreshold(this->itkImg))
	{
		int radium = 1;
		typedef itk::FlatStructuringElement<2> StructuringElementType;
		StructuringElementType::RadiusType elementRadius;
		elementRadius.Fill(radium);
		StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);
		typedef itk::BinaryMorphologicalClosingImageFilter <ImageType, ImageType, StructuringElementType>
			BinaryMorphologicalClosingImageFilterType;
		BinaryMorphologicalClosingImageFilterType::Pointer closingFilter
			= BinaryMorphologicalClosingImageFilterType::New();
		closingFilter->SetInput(this->itkImg);
		closingFilter->SetKernel(structuringElement);
		closingFilter->Update();
		this->itkImg = closingFilter->GetOutput();
		this->itkImgProcess = itkShow2process(this->itkImg);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
	else
	{
		float kernelValue = 1;

		typedef itk::Neighborhood<float, 2> KernelType;
		KernelType neighborhood;
		neighborhood.SetRadius(kernelValue);
		for (unsigned int i = 0; i < neighborhood.GetSize()[0] * neighborhood.GetSize()[1]; ++i)
		{
			neighborhood[i] = 1;
		}

		typedef itk::GrayscaleDilateImageFilter< ImageTypeProcess, ImageTypeProcess, KernelType > GrayscaleDilateImageFilterType;
		GrayscaleDilateImageFilterType::Pointer dilateFilter = GrayscaleDilateImageFilterType::New();
		dilateFilter->SetInput(this->itkImgProcess);
		dilateFilter->SetKernel(neighborhood);
		dilateFilter->Update();
		this->itkImgProcess = dilateFilter->GetOutput();

		typedef itk::GrayscaleFunctionErodeImageFilter< ImageTypeProcess, ImageTypeProcess, KernelType > GrayscaleErodeImageFilterType;
		GrayscaleErodeImageFilterType::Pointer erodeFilter = GrayscaleErodeImageFilterType::New();
		erodeFilter->SetInput(this->itkImgProcess);
		erodeFilter->SetKernel(neighborhood);
		erodeFilter->Update();
		this->itkImgProcess = erodeFilter->GetOutput();
		this->itkImg = itkProcess2show(this->itkImgProcess);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
}

bool viewer::isImgEmpty(ImageType::Pointer itkImg)
{
	ImageType::RegionType region = itkImg->GetLargestPossibleRegion();
	ImageType::SizeType size = region.GetSize();
	ImageType::PixelType pixelValue;
	ImageType::IndexType index;

	int width = size[0];
	int height = size[1];

	for (int i = 0; i<size[0]; i++)
	{
		for (int j = 0; j<size[1]; j++)
		{
			index[0] = i;
			index[1] = j;
			pixelValue = itkImg->GetPixel(index);
			if (pixelValue != 0)
			{
				return false;
			}
		}
	}

	return true;
}

void viewer::binaryDistanceTrans()
{
	/*if (this->isThreshold(this->itkImg))
	{
		typedef  itk::SignedMaurerDistanceMapImageFilter< ImageType, ImageTypeProcess  > SignedMaurerDistanceMapImageFilterType;
		SignedMaurerDistanceMapImageFilterType::Pointer distanceMapImageFilter = SignedMaurerDistanceMapImageFilterType::New();
		distanceMapImageFilter->SetInput(this->itkImg);
		distanceMapImageFilter->Update();
		ImageTypeProcess::Pointer result = ImageTypeProcess::New();
		result = distanceMapImageFilter->GetOutput();
		ImageTypeProcess::RegionType region = result->GetLargestPossibleRegion();
		ImageTypeProcess::SizeType size = region.GetSize();
		ImageTypeProcess::IndexType index;
		ImageTypeProcess::PixelType pixelValue;
		int width = size[0];
		int height = size[1];
		for (int i = 0; i<size[0]; i++)
		{
			for (int j = 0; j<size[1]; j++)
			{
				index[0] = i;
				index[1] = j;
				pixelValue = result->GetPixel(index);
				if (pixelValue > 0)
				{
					result->SetPixel(index, 0);
				}
				else
				{
					result->SetPixel(index, -pixelValue);
				}
			}
		}
		this->itkImg = itkProcess2show(result);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}*/

	if (this->isThreshold(this->itkImg))
	{
		int radium = 1;
		typedef itk::FlatStructuringElement<2> StructuringElementType;
		StructuringElementType::RadiusType elementRadius;
		elementRadius.Fill(radium);
		StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);
		typedef itk::BinaryErodeImageFilter<ImageType, ImageType, StructuringElementType> BinaryErodeImageFilterType;
		BinaryErodeImageFilterType::Pointer binaryErodeFilter = BinaryErodeImageFilterType::New();

		ImageType::Pointer erodedImg = ImageType::New();

		binaryErodeFilter->SetInput(this->itkImg);
		binaryErodeFilter->SetKernel(structuringElement);
		binaryErodeFilter->Update();
		erodedImg = binaryErodeFilter->GetOutput();
		erodedImg->DisconnectPipeline();

		typedef itk::ImageDuplicator< ImageType > DuplicatorType;
		DuplicatorType::Pointer duplicator = DuplicatorType::New();
		duplicator->SetInputImage(this->itkImg);
		duplicator->Update();

		ImageType::Pointer DTM = ImageType::New();
		DTM = duplicator->GetOutput();
		DTM->DisconnectPipeline();

		ImageType::RegionType region = DTM->GetLargestPossibleRegion();
		ImageType::SizeType size = region.GetSize();
		ImageType::PixelType pixelValue;
		ImageType::IndexType index;
		ImageType::PixelType pixelValuePre;

		for (int i = 0; i<size[0]; i++)
		{
			for (int j = 0; j<size[1]; j++)
			{
				index[0] = i;
				index[1] = j;
				pixelValue = this->itkImg->GetPixel(index);
				if (pixelValue != 0)
				{
					DTM->SetPixel(index, 1);
				}
			}
		}

		while (!this->isImgEmpty(erodedImg))
		{
			for (int i = 0; i<size[0]; i++)
			{
				for (int j = 0; j<size[1]; j++)
				{
					index[0] = i;
					index[1] = j;
					pixelValue = erodedImg->GetPixel(index);
					if (pixelValue != 0)
					{
						pixelValuePre = DTM->GetPixel(index);
						pixelValuePre++;
						DTM->SetPixel(index, pixelValuePre);
					}
				}
			}

			binaryErodeFilter->SetInput(erodedImg);
			binaryErodeFilter->SetKernel(structuringElement);
			binaryErodeFilter->Update();
			erodedImg = binaryErodeFilter->GetOutput();
			erodedImg->DisconnectPipeline();
		}

		this->itkDTM = DTM;
		this->haveDTM = true;
		typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleType;
		RescaleType::Pointer rescaler = RescaleType::New();
		rescaler->SetInput(DTM);
		rescaler->SetOutputMinimum(0);
		rescaler->SetOutputMaximum(255);
		rescaler->Update();
		this->itkImg = rescaler->GetOutput();
		this->itkImgProcess = itkShow2process(this->itkImg);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
}

void viewer::skeletonize()
{
	if (this->isThreshold(this->itkImg))
	{
		int radium = 1;
		typedef itk::FlatStructuringElement<2> StructuringElementType;
		StructuringElementType::RadiusType elementRadius;
		elementRadius.Fill(radium);
		StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);
		typedef itk::BinaryErodeImageFilter<ImageType, ImageType, StructuringElementType> BinaryErodeImageFilterType;
		BinaryErodeImageFilterType::Pointer binaryErodeFilter = BinaryErodeImageFilterType::New();

		ImageType::Pointer erodedImg = ImageType::New();

		binaryErodeFilter->SetInput(this->itkImg);
		binaryErodeFilter->SetKernel(structuringElement);
		binaryErodeFilter->Update();
		erodedImg = binaryErodeFilter->GetOutput();
		erodedImg->DisconnectPipeline();

		typedef itk::ImageDuplicator< ImageType > DuplicatorType;
		DuplicatorType::Pointer duplicator = DuplicatorType::New();
		duplicator->SetInputImage(this->itkImg);
		duplicator->Update();

		ImageType::Pointer DTM = ImageType::New();
		DTM = duplicator->GetOutput();
		DTM->DisconnectPipeline();

		ImageType::RegionType region = DTM->GetLargestPossibleRegion();
		ImageType::SizeType size = region.GetSize();
		ImageType::PixelType pixelValue;
		ImageType::IndexType index;
		ImageType::PixelType pixelValuePre;

		for (int i = 0; i<size[0]; i++)
		{
			for (int j = 0; j<size[1]; j++)
			{
				index[0] = i;
				index[1] = j;
				pixelValue = this->itkImg->GetPixel(index);
				if (pixelValue != 0)
				{
					DTM->SetPixel(index, 1);
				}
			}
		}

		while (!this->isImgEmpty(erodedImg))
		{
			for (int i = 0; i<size[0]; i++)
			{
				for (int j = 0; j<size[1]; j++)
				{
					index[0] = i;
					index[1] = j;
					pixelValue = erodedImg->GetPixel(index);
					if (pixelValue != 0)
					{
						pixelValuePre = DTM->GetPixel(index);
						pixelValuePre++;
						DTM->SetPixel(index, pixelValuePre);
					}
				}
			}

			binaryErodeFilter->SetInput(erodedImg);
			binaryErodeFilter->SetKernel(structuringElement);
			binaryErodeFilter->Update();
			erodedImg = binaryErodeFilter->GetOutput();
			erodedImg->DisconnectPipeline();
		}

		//skeletonize
		typedef itk::ImageDuplicator< ImageType > DuplicatorType;
		DuplicatorType::Pointer duplicator2 = DuplicatorType::New();
		duplicator2->SetInputImage(DTM);
		duplicator2->Update();

		this->itkImg = duplicator2->GetOutput();
		this->itkImg->DisconnectPipeline();

		ImageType::IndexType neighborIndex;
		ImageType::PixelType neighborPixelValue;
		int innerX;
		int innerY;
		for (int i = 0; i<size[0]; i++)
		{
			for (int j = 0; j<size[1]; j++)
			{
				index[0] = i;
				index[1] = j;
				pixelValue = DTM->GetPixel(index);
				if (pixelValue != 0)
				{
					for (int innerI = 0; innerI < 3; innerI++)
					{
						for (int innerJ = 0; innerJ < 3; innerJ++)
						{
							innerX = i + innerI - 1;
							innerY = j + innerJ - 1;
							if (innerX >= 0 && innerX < size[0] && innerY >= 0 && innerY < size[1])
							{
								neighborIndex[0] = innerX;
								neighborIndex[1] = innerY;
								neighborPixelValue = DTM->GetPixel(neighborIndex);
								if (neighborPixelValue > pixelValue)
								{
									this->itkImg->SetPixel(index, 0);
								}
							}
						}
					}
				}
			}
		}

		this->itkDTM = this->itkImg;
		this->haveDTM = true;
		typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleType;
		RescaleType::Pointer rescaler = RescaleType::New();
		rescaler->SetInput(this->itkImg);
		rescaler->SetOutputMinimum(0);
		rescaler->SetOutputMaximum(255);
		rescaler->Update();
		this->itkImg = rescaler->GetOutput();
		this->itkImgProcess = itkShow2process(this->itkImg);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
}

void viewer::skRestorate()
{
	if (this->haveDTM)
	{
		ImageType::Pointer resMap = ImageType::New();
		typedef itk::ImageDuplicator< ImageType > DuplicatorType;
		DuplicatorType::Pointer duplicator = DuplicatorType::New();
		duplicator->SetInputImage(this->itkDTM);
		duplicator->Update();
		resMap = duplicator->GetOutput();
		resMap->DisconnectPipeline();

		ImageType::RegionType region = this->itkDTM->GetLargestPossibleRegion();
		ImageType::SizeType size = region.GetSize();
		ImageType::PixelType pixelValue;
		ImageType::IndexType index;
		ImageType::PixelType pixelValuePre;

		ImageType::IndexType resIndex;
		int innerX;
		int innerY;
		for (int i = 0; i<size[0]; i++)
		{
			for (int j = 0; j<size[1]; j++)
			{
				index[0] = i;
				index[1] = j;
				pixelValue = this->itkDTM->GetPixel(index);
				if (pixelValue > 1)
				{
					int areaLength = (pixelValue - 1) * 2 + 1;
					if (pixelValue != 0)
					{
						for (int innerI = 0; innerI < areaLength; innerI++)
						{
							for (int innerJ = 0; innerJ < areaLength; innerJ++)
							{
								innerX = i + innerI - pixelValue;
								innerY = j + innerJ - pixelValue;
								if (innerX >= 0 && innerX < size[0] && innerY >= 0 && innerY < size[1])
								{
									resIndex[0] = innerX;
									resIndex[1] = innerY;
									resMap->SetPixel(resIndex, 1);

								}
							}
						}
					}
				}	
			}
		}

		typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleType;
		RescaleType::Pointer rescaler = RescaleType::New();
		rescaler->SetInput(resMap);
		rescaler->SetOutputMinimum(0);
		rescaler->SetOutputMaximum(255);
		rescaler->Update();
		this->itkImg = rescaler->GetOutput();
		QImage image = itk2qImg(this->itkImg);
		this->itkImgProcess = itkShow2process(this->itkImg);
		this->refreshImg(&image);
	}
}

void viewer::morphEdge()
{
	if (this->isThreshold(this->itkImg))
	{
		int radium = 1;
		typedef itk::FlatStructuringElement<2> StructuringElementType;
		StructuringElementType::RadiusType elementRadius;
		elementRadius.Fill(radium);
		StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);

		typedef itk::BinaryErodeImageFilter<ImageType, ImageType, StructuringElementType> BinaryErodeImageFilterType;
		BinaryErodeImageFilterType::Pointer binaryErodeFilter = BinaryErodeImageFilterType::New();
		binaryErodeFilter->SetInput(this->itkImg);
		binaryErodeFilter->SetKernel(structuringElement);
		binaryErodeFilter->Update();

		typedef itk::BinaryDilateImageFilter <ImageType, ImageType, StructuringElementType> BinaryDilateImageFilterType;
		BinaryDilateImageFilterType::Pointer binaryDilateFilter = BinaryDilateImageFilterType::New();
		binaryDilateFilter->SetInput(this->itkImg);
		binaryDilateFilter->SetKernel(structuringElement);
		binaryDilateFilter->Update();

		typedef itk::SubtractImageFilter<ImageType> SubtractType;
		SubtractType::Pointer diff = SubtractType::New();

		switch (this->processType)
		{
		case 0:
		{
				  diff->SetInput1(binaryDilateFilter->GetOutput());
				  diff->SetInput2(binaryErodeFilter->GetOutput());
				  diff->Update();
				  break;
		}
		case 1:
		{
				  diff->SetInput1(binaryDilateFilter->GetOutput());
				  diff->SetInput2(this->itkImg);
				  diff->Update();
				  break;
		}
		case 2:
		{
				  diff->SetInput1(this->itkImg);
				  diff->SetInput2(binaryErodeFilter->GetOutput());
				  diff->Update();
				  break;
		}
		default:
			break;
		}

		this->itkImg = diff->GetOutput();
		this->itkImgProcess = itkShow2process(this->itkImg);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
	else
	{
		float kernelValue = 1;

		typedef itk::Neighborhood<float, 2> KernelType;
		KernelType neighborhood;
		neighborhood.SetRadius(kernelValue);
		for (unsigned int i = 0; i < neighborhood.GetSize()[0] * neighborhood.GetSize()[1]; ++i)
		{
			neighborhood[i] = 1;
		}

		typedef itk::GrayscaleFunctionErodeImageFilter< ImageTypeProcess, ImageTypeProcess, KernelType > GrayscaleErodeImageFilterType;
		GrayscaleErodeImageFilterType::Pointer erodeFilter = GrayscaleErodeImageFilterType::New();
		erodeFilter->SetInput(this->itkImgProcess);
		erodeFilter->SetKernel(neighborhood);
		erodeFilter->Update();

		typedef itk::GrayscaleDilateImageFilter< ImageTypeProcess, ImageTypeProcess, KernelType > GrayscaleDilateImageFilterType;
		GrayscaleDilateImageFilterType::Pointer dilateFilter = GrayscaleDilateImageFilterType::New();
		dilateFilter->SetInput(this->itkImgProcess);
		dilateFilter->SetKernel(neighborhood);
		dilateFilter->Update();

		typedef itk::SubtractImageFilter<ImageTypeProcess> SubtractType;
		SubtractType::Pointer diff = SubtractType::New();

		switch (this->processType)
		{
		case 0:
		{
				  diff->SetInput1(dilateFilter->GetOutput());
				  diff->SetInput2(erodeFilter->GetOutput());
				  diff->Update();
				  break;
		}
		case 1:
		{
				  diff->SetInput1(dilateFilter->GetOutput());
				  diff->SetInput2(this->itkImgProcess);
				  diff->Update();
				  break;
		}
		case 2:
		{
				  diff->SetInput1(this->itkImgProcess);
				  diff->SetInput2(erodeFilter->GetOutput());
				  diff->Update();
				  break;
		}
		default:
			break;
		}

		this->itkImg = itkProcess2show(diff->GetOutput());
		this->itkImgProcess = itkShow2process(this->itkImg);
		QImage image = itk2qImg(this->itkImg);
		this->refreshImg(&image);
	}
}

void viewer::sendMask()
{
	emit maskInfo(this->itkImg, this->itkImgProcess);
}

void viewer::sendItk()
{
	emit itkInfo(this->itkImg);
}

void viewer::maskDilate(ImageType::Pointer itkMaskImg, ImageTypeProcess::Pointer itkMaskImgProcess)
{
	if (this->isThreshold(this->itkImg))
	{
		if (this->isThreshold(itkMaskImg))
		{
			typedef itk::BinaryReconstructionByDilationImageFilter<ImageType> BinaryReconstructionImageFilterType;
			BinaryReconstructionImageFilterType::Pointer binaryReconstructionFilter = BinaryReconstructionImageFilterType::New();
			binaryReconstructionFilter->SetMarkerImage(this->itkImg);
			binaryReconstructionFilter->SetMaskImage(itkMaskImg);
			binaryReconstructionFilter->Update();
			this->itkImg = binaryReconstructionFilter->GetOutput();
			this->itkImgProcess = itkShow2process(this->itkImg);
			QImage image = itk2qImg(this->itkImg);
			this->refreshImg(&image);
		}
	}
	else
	{
		if (!this->isThreshold(itkMaskImg))
		{
			typedef itk::ReconstructionImageFilter<ImageTypeProcess, ImageTypeProcess, std::greater<ImageTypeProcess::PixelType>> ReconstructionImageFilterType;
			ReconstructionImageFilterType::Pointer reconstructionFilter = ReconstructionImageFilterType::New();
			reconstructionFilter->SetMarkerImage(this->itkImgProcess);
			reconstructionFilter->SetMaskImage(itkMaskImgProcess);
			reconstructionFilter->Update();
			this->itkImgProcess = reconstructionFilter->GetOutput();
			this->itkImg = itkProcess2show(this->itkImgProcess);
			QImage image = itk2qImg(this->itkImg);
			this->refreshImg(&image);
		}
	}
}