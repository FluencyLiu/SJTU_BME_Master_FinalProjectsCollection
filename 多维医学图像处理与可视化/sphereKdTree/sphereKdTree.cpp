#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkPoints.h>
#include <vtkPointSource.h>
#include <vtkIdList.h>
#include <vtkKdTree.h>
#include <vtkCubeSource.h>

#include <math.h>
#include <iostream>

int main()
{

	std::cout << "please input the coordinate of a point (e.g. 1 1 1):\n";
	int xSearch, ySearch, zSearch;
	std::cin >> xSearch >> ySearch >> zSearch;
	double pointSearch[3] = {xSearch, ySearch, zSearch};
	// Create a sphere
	vtkSmartPointer<vtkSphereSource> sphereSource[50];
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	int spColor[50];//set color of the spheres. red, green and blue is represented by 0, 1, 2 seperately.
	for (int i = 0; i < 50; i++)
	{
		double xSp = rand() % 100;
		double ySp = rand() % 100;
		double zSp = rand() % 100;
		sphereSource[i] = vtkSmartPointer<vtkSphereSource>::New();
		sphereSource[i]->SetCenter(xSp, ySp, zSp);
		double rSp = rand() % 6 + 5;
		sphereSource[i]->SetRadius(rSp);
		double centerSp[3] = { xSp, ySp, zSp };
		points->InsertNextPoint(centerSp);
		spColor[i] = rand() % 3;
	}

	//find the closest point to input point
	vtkSmartPointer<vtkKdTree> pointTree = vtkSmartPointer<vtkKdTree>::New();
	pointTree->BuildLocatorFromPoints(points);
	vtkSmartPointer<vtkIdList> result =vtkSmartPointer<vtkIdList>::New();
	pointTree->FindClosestNPoints(1, pointSearch, result);

	vtkIdType resultSearchInd = result->GetId(0);
	double resultSearch[3];
	points->GetPoint(resultSearchInd, resultSearch);
	std::cout << "Closest point to (" << pointSearch[0] << ", " << pointSearch[1] << ", " << pointSearch[2] << ") is points No."
		<< resultSearchInd << ". And its coordinate is (" << resultSearch[0] << ", " << resultSearch[1] << ", " << resultSearch[2] << ").\n";
	std::cout << "The radius is " << sphereSource[resultSearchInd]->GetRadius() << ". And its color is ";
	switch (spColor[resultSearchInd])
	{
	case 0:
		std::cout << "red.";
		break;
	case 1:
		std::cout << "green.";
		break;
	case 2:
		std::cout << "blue.";
		break;
	default:
		std::cout << "error while setting color for spheres.";
		break;
	}

	//show items on the screen.
	vtkSmartPointer<vtkPolyDataMapper> mapper[50];
	vtkSmartPointer<vtkActor> actor[50];
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
	cubeSource->SetBounds(0,99,0,99,0,99);
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

	//add spheres on the window.
	for (int i = 0; i < 50; i++)
	{
		mapper[i] = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper[i]->SetInputConnection(sphereSource[i]->GetOutputPort());
		actor[i] = vtkSmartPointer<vtkActor>::New();
		actor[i]->SetMapper(mapper[i]);
		switch (spColor[i])
		{
		case 0:
			actor[i]->GetProperty()->SetColor(1, 0, 0);
			break;
		case 1:
			actor[i]->GetProperty()->SetColor(0, 1, 0);
			break;
		case 2:
			actor[i]->GetProperty()->SetColor(0, 0, 1);
			break;
		default:
			std::cout << "error while setting color for spheres.";
			return EXIT_FAILURE;
			break;
		}
		renderer->AddActor(actor[i]);
	}	

	renderer->SetBackground(0.5, 0.5, 0.5); 
	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}