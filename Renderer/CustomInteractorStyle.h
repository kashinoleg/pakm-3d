#pragma once
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTerrain.h"
#include "vtkCamera.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"

class CCustomInteractorStyle : public vtkInteractorStyleTerrain
{
public:
	static CCustomInteractorStyle *New();
	void Rotate();
};
