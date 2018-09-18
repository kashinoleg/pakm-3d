#include "CustomInteractorStyle.h"

vtkStandardNewMacro(CCustomInteractorStyle);

void CCustomInteractorStyle::Rotate()
{
	if (this->CurrentRenderer == NULL)
	{
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;

	int dx = - (rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0]);
	int dy = - (rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1]);

	int *size = this->CurrentRenderer->GetRenderWindow()->GetSize();

	double a = (double)dx / (double)size[0] * (double)180.0;
	double e = (double)dy / (double)size[1] * (double)180.0;

	if (rwi->GetShiftKey()) 
	{
		if (fabs((double)dx) >= fabs((double)dy))
		{
			e = (double)0.0;
		}
		else
		{
			a = (double)0.0;
		}
	}

	// Move the camera. 
	// Make sure that we don't hit the north pole singularity.

	vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
	camera->Azimuth(a);

	double dop[3], vup[3];

	camera->GetDirectionOfProjection(dop);
	vtkMath::Normalize(dop);
	camera->GetViewUp(vup);
	vtkMath::Normalize(vup);

	const double dAngleLimit = 5.0;

	double angle = acos(vtkMath::Dot(dop, vup)) / vtkMath::RadiansFromDegrees( 1.0 );
	if ((angle + e) > 180.0-dAngleLimit)
	{
		e = 180.0-dAngleLimit-angle;
	}
	else if ((angle + e) < dAngleLimit)
	{
		e = dAngleLimit-angle;
	}

	camera->Elevation(e);
	if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
	}

	rwi->Render();
}
