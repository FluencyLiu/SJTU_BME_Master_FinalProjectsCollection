#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkSphereSource.h>
#include <vtkMetaImageWriter.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencil.h>
#include <vtkPointData.h>
#include <vtkSphereSource.h>
#include <vtkPoints.h>
#include <vtkPointSource.h>
#include <vtkIdList.h>
#include <vtkSmartPointer.h>
#include <vtkLineSource.h>
#include <vtkDoubleArray.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkPCAStatistics.h>
#include <vtkStringArray.h>
#include <vtkTable.h>
#include <vtkCubeSource.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <math.h>
#include <iostream>

void clacBinaryImageCenter(vtkSmartPointer<vtkImageData> inputImage, double result[3]);
vtkSmartPointer<vtkPCAStatistics> binaryImagePCA(vtkSmartPointer<vtkImageData> inputImage);
void getEigenInfo(vtkSmartPointer<vtkPCAStatistics> pcaStatistics, double eigenvalueVect[3], int indexEig[3], double eigenvectorVect[3][3]);

int main()
{
	//for there may be overlapping of different spheres, some error will occur if we apply PCA to poly data directly.
	//in order to eliminate this effect, a convert from poly data into binary image data is necessay.
	//and there are also some spheres of which centers are at the boundary of the 100*100*100 space, some part of them may not be covered by the 100*100*100 space totally.
	//in case of that, we extend the space to 120*120*120.
	std::srand((unsigned)time(NULL));
	//set black image, which will be set to.
	vtkSmartPointer<vtkImageData> blackImage = vtkSmartPointer<vtkImageData>::New();
	double spacing[3]; // desired volume spacing
	spacing[0] = 1;
	spacing[1] = 1;
	spacing[2] = 1;
	blackImage->SetSpacing(spacing);

	// set dimensions
	//if there are some spheres of which centers are at the boundary of the 100*100*100 space,
	//a part of them may not be covered by the 100*100*100 space.
	int dim[3] = { 120, 120, 120 };
	blackImage->SetDimensions(dim);

	double origin[3];
	origin[0] = 0;
	origin[1] = 0;
	origin[2] = 0;
	blackImage->SetOrigin(origin);

#if VTK_MAJOR_VERSION <= 5
	blackImage->SetScalarTypeToUnsignedChar();
	blackImage->AllocateScalars();
#else
	blackImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
#endif

	// fill the image with background voxels:
	unsigned char inval = 1;
	unsigned char outval = 0;
	vtkIdType count = blackImage->GetNumberOfPoints();
	for (vtkIdType i = 0; i < count; ++i)
	{
		blackImage->GetPointData()->GetScalars()->SetTuple1(i, outval);
	}

	// create spheres
	vtkSmartPointer<vtkSphereSource> sphereSource[50];
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	int spColor[50];//set color of the spheres. red, green and blue is represented by 0, 1, 2 seperately.
	for (int i = 0; i < 50; i++)
	{
		/*if there are some spheres of which centers are at the boundary of the 100*100*100 space, 
		a part of them may not be covered by the 100*100*100 space. in case of that, we extend the space to 120*120*120,
		and make some related modification on the coordinate for new space. */
		double xSp = std::rand() % 100 + 10;
		double ySp = std::rand() % 100 + 10;
		double zSp = std::rand() % 100 + 10;
		sphereSource[i] = vtkSmartPointer<vtkSphereSource>::New();
		sphereSource[i]->SetCenter(xSp, ySp, zSp);
		double rSp = std::rand() % 6 + 5;
		sphereSource[i]->SetRadius(rSp);
		double centerSp[3] = { xSp, ySp, zSp };
		points->InsertNextPoint(centerSp);
		spColor[i] = std::rand() % 3;
		sphereSource[i]->Update();
	}

	//use an image stencil iteratively to convert sphere poly data into image data.
	vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyDataToImageStencil> pol2stenc = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
	vtkSmartPointer<vtkImageStencil> imgstenc = vtkSmartPointer<vtkImageStencil>::New();

	//add the rest of spheres in the image stencil.
	for (int i = 1; i < 50; i++)
	{
		pd = vtkSmartPointer<vtkPolyData>::New();
		pd = sphereSource[i]->GetOutput();

#if VTK_MAJOR_VERSION <= 5
		pol2stenc->SetInput(pd);
#else
		pol2stenc->SetInputData(pd);
#endif
		pol2stenc->SetOutputOrigin(origin);
		pol2stenc->SetOutputSpacing(spacing);
		pol2stenc->SetOutputWholeExtent(blackImage->GetExtent());
		pol2stenc->Update();

#if VTK_MAJOR_VERSION <= 5
		imgstenc->SetInput(blackImage);
		imgstenc->SetStencil(pol2stenc->GetOutput());
#else
		imgstenc->SetInputData(blackImage);
		imgstenc->SetStencilConnection(pol2stenc->GetOutputPort());
#endif
		imgstenc->ReverseStencilOn();
		imgstenc->SetBackgroundValue(inval);
		imgstenc->Update();
		blackImage = imgstenc->GetOutput();
	}

	//calculate the mass center of the binary image.
	double massCenter[3];
	clacBinaryImageCenter(blackImage, massCenter);

	//pca analysis
	vtkSmartPointer<vtkPCAStatistics> pcaStatistics = vtkSmartPointer<vtkPCAStatistics>::New();
	pcaStatistics = binaryImagePCA(blackImage);
	double eigenvalueVect[3];
	int indexEig[3] = { 0, 1, 2 };
	double eigenvectorVect[3][3];
	getEigenInfo(pcaStatistics, eigenvalueVect, indexEig, eigenvectorVect);

	//show items on the screen.
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	//generate a 100*100*100 cubic space to show the space which contain all the centers of the spheres.
	vtkSmartPointer<vtkCubeSource> cubeSource =
		vtkSmartPointer<vtkCubeSource>::New();
	cubeSource->SetBounds(10, 109, 10, 109, 10, 109);
	cubeSource->Update();

	// Create a mapper and actor.
	vtkSmartPointer<vtkPolyDataMapper> spaceMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	spaceMapper->SetInputConnection(cubeSource->GetOutputPort());

	vtkSmartPointer<vtkActor> spaceActor =
		vtkSmartPointer<vtkActor>::New();
	spaceActor->SetMapper(spaceMapper);
	spaceActor->GetProperty()->SetOpacity(0.1);
	renderer->AddActor(spaceActor);

	//show the major axis
	double majorVect[3] = { eigenvectorVect[indexEig[2]][0], eigenvectorVect[indexEig[2]][1], eigenvectorVect[indexEig[2]][2] };
	double pMajor0[3] = { massCenter[0] + 100 * majorVect[0], massCenter[1] + 100 * majorVect[1], massCenter[2] + 100 * majorVect[2] };
	double pMajor1[3] = { massCenter[0] - 100 * majorVect[0], massCenter[1] - 100 * majorVect[1], massCenter[2] - 100 * majorVect[2] };

	vtkSmartPointer<vtkLineSource> majorLineSource =
		vtkSmartPointer<vtkLineSource>::New();
	majorLineSource->SetPoint1(pMajor0);
	majorLineSource->SetPoint2(pMajor1);
	majorLineSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> majorLineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	majorLineMapper->SetInputConnection(majorLineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> majorLineActor =
		vtkSmartPointer<vtkActor>::New();
	majorLineActor->SetMapper(majorLineMapper);
	majorLineActor->GetProperty()->SetLineWidth(2);
	majorLineActor->GetProperty()->SetColor(0.5, 0.5, 1);
	renderer->AddActor(majorLineActor);

	//show the minor axis
	double minorVect[3] = { eigenvectorVect[indexEig[0]][0], eigenvectorVect[indexEig[0]][1], eigenvectorVect[indexEig[0]][2] };
	double pMinor0[3] = { massCenter[0] + 50 * minorVect[0], massCenter[1] + 50 * minorVect[1], massCenter[2] + 50 * minorVect[2] };
	double pMinor1[3] = { massCenter[0] - 50 * minorVect[0], massCenter[1] - 50 * minorVect[1], massCenter[2] - 50 * minorVect[2] };

	vtkSmartPointer<vtkLineSource> minorLineSource =
		vtkSmartPointer<vtkLineSource>::New();
	minorLineSource->SetPoint1(pMinor0);
	minorLineSource->SetPoint2(pMinor1);
	minorLineSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> minorLineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	minorLineMapper->SetInputConnection(minorLineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> minorLineActor =
		vtkSmartPointer<vtkActor>::New();
	minorLineActor->SetMapper(minorLineMapper);
	minorLineActor->GetProperty()->SetLineWidth(2);
	minorLineActor->GetProperty()->SetColor(1, 0.5, 0.5);
	renderer->AddActor(minorLineActor);

	//show the third axis
	double thirdVect[3] = { eigenvectorVect[indexEig[1]][0], eigenvectorVect[indexEig[1]][1], eigenvectorVect[indexEig[1]][2] };
	double pThird0[3] = { massCenter[0] + 70 * thirdVect[0], massCenter[1] + 70 * thirdVect[1], massCenter[2] + 70 * thirdVect[2] };
	double pThird1[3] = { massCenter[0] - 70 * thirdVect[0], massCenter[1] - 70 * thirdVect[1], massCenter[2] - 70 * thirdVect[2] };

	vtkSmartPointer<vtkLineSource> thirdLineSource =
		vtkSmartPointer<vtkLineSource>::New();
	thirdLineSource->SetPoint1(pThird0);
	thirdLineSource->SetPoint2(pThird1);
	thirdLineSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> thirdLineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	thirdLineMapper->SetInputConnection(thirdLineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> thirdLineActor =
		vtkSmartPointer<vtkActor>::New();
	thirdLineActor->SetMapper(thirdLineMapper);
	thirdLineActor->GetProperty()->SetLineWidth(2);
	thirdLineActor->GetProperty()->SetColor(0.5, 1, 0.5);
	renderer->AddActor(thirdLineActor);

	//show x, y and z axis
	double pX0[3] = { 10, 10, 10 };
	double pX1[3] = { 125, 10, 10 };

	vtkSmartPointer<vtkLineSource> xLineSource =
		vtkSmartPointer<vtkLineSource>::New();
	xLineSource->SetPoint1(pX0);
	xLineSource->SetPoint2(pX1);
	xLineSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> xLineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	xLineMapper->SetInputConnection(xLineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> xLineActor =
		vtkSmartPointer<vtkActor>::New();
	xLineActor->SetMapper(xLineMapper);
	xLineActor->GetProperty()->SetLineWidth(2);
	xLineActor->GetProperty()->SetColor(0.5, 1, 1);//cyan
	renderer->AddActor(xLineActor);

	double pY0[3] = { 10, 10, 10 };
	double pY1[3] = { 10, 125, 10 };

	vtkSmartPointer<vtkLineSource> yLineSource =
		vtkSmartPointer<vtkLineSource>::New();
	yLineSource->SetPoint1(pY0);
	yLineSource->SetPoint2(pY1);
	yLineSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> yLineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	yLineMapper->SetInputConnection(yLineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> yLineActor =
		vtkSmartPointer<vtkActor>::New();
	yLineActor->SetMapper(yLineMapper);
	yLineActor->GetProperty()->SetLineWidth(2);
	yLineActor->GetProperty()->SetColor(1, 0.5, 1);//pink
	renderer->AddActor(yLineActor);

	double pZ0[3] = { 10, 10, 10 };
	double pZ1[3] = { 10, 10, 125 };

	vtkSmartPointer<vtkLineSource> zLineSource =
		vtkSmartPointer<vtkLineSource>::New();
	zLineSource->SetPoint1(pZ0);
	zLineSource->SetPoint2(pZ1);
	zLineSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> zLineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	zLineMapper->SetInputConnection(zLineSource->GetOutputPort());
	vtkSmartPointer<vtkActor> zLineActor =
		vtkSmartPointer<vtkActor>::New();
	zLineActor->SetMapper(zLineMapper);
	zLineActor->GetProperty()->SetLineWidth(2);
	zLineActor->GetProperty()->SetColor(1, 1, 0.5);//light yellow
	renderer->AddActor(zLineActor);

	//show spheres on the screen.
	vtkSmartPointer<vtkPolyDataMapper> spMapper[50];
	vtkSmartPointer<vtkActor> spActor[50];
	for (int i = 0; i < 50; i++)
	{
		spMapper[i] = vtkSmartPointer<vtkPolyDataMapper>::New();
		spMapper[i]->SetInputConnection(sphereSource[i]->GetOutputPort());
		spActor[i] = vtkSmartPointer<vtkActor>::New();
		spActor[i]->SetMapper(spMapper[i]);
		switch (spColor[i])
		{
		case 0:
			spActor[i]->GetProperty()->SetColor(1, 0, 0);
			break;
		case 1:
			spActor[i]->GetProperty()->SetColor(0, 1, 0);
			break;
		case 2:
			spActor[i]->GetProperty()->SetColor(0, 0, 1);
			break;
		default:
			std::cout << "error while setting color for spheres.";
			return EXIT_FAILURE;
			break;
		}
		renderer->AddActor(spActor[i]);
	}

	renderer->SetBackground(0.5, 0.5, 0.5);
	renderWindow->Render();
	renderWindowInteractor->Start();
	return EXIT_SUCCESS;
}

void clacBinaryImageCenter(vtkSmartPointer<vtkImageData> inputImage, double result[3])
{
	vtkIdType numTotal = inputImage->GetNumberOfPoints();
	double sumX = 0;
	double sumY = 0;
	double sumZ = 0;
	double numBlack = 0;
	double blackPoint[3];
	for (vtkIdType i = 0; i < numTotal; i++)
	{
		double valVol = inputImage->GetPointData()->GetScalars()->GetTuple1(i);
		if (valVol == 1)
		{
			inputImage->GetPoint(i, blackPoint);
			sumX += blackPoint[0];
			sumY += blackPoint[1];
			sumZ += blackPoint[2];
			numBlack++;
		}
	}
	result[0] = sumX / numBlack;
	result[1] = sumY / numBlack;
	result[2] = sumZ / numBlack;
}

vtkSmartPointer<vtkPCAStatistics> binaryImagePCA(vtkSmartPointer<vtkImageData> inputImage)
{
	// These will be all of your "x" values.
	const char m0Name[] = "M0";
	vtkSmartPointer<vtkDoubleArray> dataset0Arr =
		vtkSmartPointer<vtkDoubleArray>::New();
	dataset0Arr->SetNumberOfComponents(1);
	dataset0Arr->SetName(m0Name);
	// These will be all of your "y" values.
	const char m1Name[] = "M1";
	vtkSmartPointer<vtkDoubleArray> dataset1Arr =
		vtkSmartPointer<vtkDoubleArray>::New();
	dataset1Arr->SetNumberOfComponents(1);
	dataset1Arr->SetName(m1Name);
	// These will be all of your "z" values.
	const char m2Name[] = "M2";
	vtkSmartPointer<vtkDoubleArray> dataset2Arr =
		vtkSmartPointer<vtkDoubleArray>::New();
	dataset2Arr->SetNumberOfComponents(1);
	dataset2Arr->SetName(m2Name);

	int count = inputImage->GetNumberOfPoints();
	for (vtkIdType i = 0; i < count; ++i)
	{
		if (inputImage->GetPointData()->GetScalars()->GetTuple1(i) == 1)
		{
			double forePonit[3];
			inputImage->GetPoint(i, forePonit);
			dataset0Arr->InsertNextValue(forePonit[0]);
			dataset1Arr->InsertNextValue(forePonit[1]);
			dataset2Arr->InsertNextValue(forePonit[2]);
		}
	}

	vtkSmartPointer<vtkTable> datasetTable =
		vtkSmartPointer<vtkTable>::New();
	datasetTable->AddColumn(dataset0Arr);
	datasetTable->AddColumn(dataset1Arr);
	datasetTable->AddColumn(dataset2Arr);

	vtkSmartPointer<vtkPCAStatistics> pcaStatistics =
		vtkSmartPointer<vtkPCAStatistics>::New();
#if VTK_MAJOR_VERSION <= 5
	pcaStatistics->SetInput(vtkStatisticsAlgorithm::INPUT_DATA, datasetTable);
#else
	pcaStatistics->SetInputData(vtkStatisticsAlgorithm::INPUT_DATA, datasetTable);
#endif

	pcaStatistics->SetColumnStatus("M0", 1);
	pcaStatistics->SetColumnStatus("M1", 1);
	pcaStatistics->SetColumnStatus("M2", 1);
	pcaStatistics->RequestSelectedColumns();
	pcaStatistics->SetDeriveOption(true);
	pcaStatistics->Update();

	return pcaStatistics;
}

void getEigenInfo(vtkSmartPointer<vtkPCAStatistics> pcaStatistics, double eigenvalueVect[3], int indexEig[3], double eigenvectorVect[3][3])
{
	//eigenvalues.
	vtkSmartPointer<vtkDoubleArray> eigenvalues =
		vtkSmartPointer<vtkDoubleArray>::New();
	pcaStatistics->GetEigenvalues(eigenvalues);
	for (vtkIdType i = 0; i < eigenvalues->GetNumberOfTuples(); i++)
	{
		eigenvalueVect[i] = eigenvalues->GetValue(i);
	}

	//order the eigenvalues.
	double mediaValue;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2 - i; j++)
		{
			if (eigenvalueVect[j] > eigenvalueVect[j + 1])
			{
				mediaValue = eigenvalueVect[j + 1];
				eigenvalueVect[j + 1] = eigenvalueVect[j];
				eigenvalueVect[j] = mediaValue;

				mediaValue = indexEig[j + 1];
				indexEig[j + 1] = indexEig[j];
				indexEig[j] = mediaValue;
			}
		}
	}

	//eigenvectors
	vtkSmartPointer<vtkDoubleArray> eigenvectors =
		vtkSmartPointer<vtkDoubleArray>::New();

	pcaStatistics->GetEigenvectors(eigenvectors);
	for (vtkIdType i = 0; i < eigenvectors->GetNumberOfTuples(); i++)
	{
		double* evec = new double[eigenvectors->GetNumberOfComponents()];
		eigenvectors->GetTuple(i, evec);
		vtkSmartPointer<vtkDoubleArray> eigenvectorSingle =
			vtkSmartPointer<vtkDoubleArray>::New();
		pcaStatistics->GetEigenvector(i, eigenvectorSingle);
		for (vtkIdType j = 0; j < eigenvectors->GetNumberOfComponents(); j++)
		{
			eigenvectorVect[i][j] = evec[j];
		}
		delete[] evec;
	}
}