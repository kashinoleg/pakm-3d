#pragma once
#include "vtkArrowSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtk3DWidget.h"
#include "vtkObjectFactory.h"
#include "vtkCallbackCommand.h"
#include "vtkCellPicker.h"

#include "TexGen.h"
using namespace TexGen;

class vtkActor;
class vtkCellPicker;
class vtkPlanes;
class vtkPoints;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkProp;
class vtkProperty;
class vtkSphereSource;
class vtkTransform;

class vtkCustomWidget : public vtk3DWidget
{
public:
	enum WidgetState
	{
		START=0,
		ACTIVE,
		OUTSIDE
	};

	// Description:
	// Instantiate the object.
	static vtkCustomWidget *New();

	vtkTypeMacro(vtkCustomWidget, vtk3DWidget);
//	void PrintSelf(ostream& os, vtkIndent indent) {;}

	// Description:
	// Methods that satisfy the superclass' API.
	void SetEnabled(int);
	void PlaceWidget(double bounds[6]) {};
	void PlaceWidget() {this->Superclass::PlaceWidget();}
	void PlaceWidget(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
	{
		this->Superclass::PlaceWidget(xmin,xmax,ymin,ymax,zmin,zmax);
	}
	vector<vtkProp*> GetProps();
	void SetScale(double dScale);

	void SetPosition(XYZ Position, bool DeselectAxis = true);
	XYZ GetPosition();

	void SetSnapSize(double dSnapSize) { m_dSnapDistance = dSnapSize; }
	void SetSnap(bool bSnap) { m_bSnap = bSnap; }
	void SetRelativeSnap(bool bRelative) { m_bRelativeSnap = bRelative; }
	double GetSnapSize() const { return m_dSnapDistance; }
	bool GetSnap() const { return m_bSnap; }
	bool GetRelativeSnap() const { return m_bRelativeSnap; }

protected:
	vtkCustomWidget();
	~vtkCustomWidget();

	// Handles the events
	static void ProcessEvents(vtkObject* object, 
							unsigned long event,
							void* clientdata, 
							void* calldata);

	// ProcessEvents() dispatches to these methods.
	void OnMouseMove();
	void OnLeftButtonDown();
	void OnLeftButtonUp();
	void OnMiddleButtonDown();
	void OnMiddleButtonUp();
	void OnRightButtonDown() {};
	void OnRightButtonUp() {};

	void HighlightHandle(vtkActor *pActor);
	void UpdateHandlePositions();

	// The arrows
	vtkActor* m_pXArrow;
	vtkActor* m_pYArrow;
	vtkActor* m_pZArrow;

	// Do the picking
	vtkCellPicker* m_pHandlePicker;
	int m_iSelectAxis;
	WidgetState m_State;

	XYZ m_CurrentPosition;
	XYZ m_StartPosition;
	XYZ m_UnsnappedPosition;
	double m_dSnapDistance;
	bool m_bSnap;
	bool m_bRelativeSnap;

private:
	vtkCustomWidget(const vtkCustomWidget&);  //Not implemented
	void operator=(const vtkCustomWidget&);  //Not implemented
};

inline double RoundValue(double dValue) {
	if (dValue < 0) {
		return ceil(dValue);
	}
	else {
		return floor(dValue);
	}
}
