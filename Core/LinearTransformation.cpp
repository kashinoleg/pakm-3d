#include "stdafx.h"
#include "LinearTransformation.h"

using namespace TexGen;

CLinearTransformation::CLinearTransformation(void)
{
	InitialiseIdentity(3);
}

CLinearTransformation::CLinearTransformation(const CMatrix &Matrix)
{
	if (Matrix.GetWidth() == 3 && Matrix.GetHeight() == 3)
	{
		*((CMatrix*)this) = Matrix;
	}
	else
	{
		TGERROR("Tried to initialise a linear transformation with a matrix of size: " << Matrix.GetWidth() << "x" << Matrix.GetHeight());
		InitialiseIdentity(3);
	}
}

CLinearTransformation::~CLinearTransformation(void)
{
}

void CLinearTransformation::ResetTransformation()
{
	Identity();
}

void CLinearTransformation::MultiplyTransformation(const CMatrix &Deformation)
{
	*this = Deformation * *this;
}

void CLinearTransformation::AddScale(double dXScale, double dYScale, double dZScale)
{
	CMatrix Deform;
	Deform.InitialiseIdentity(3);
	Deform(0, 0) = dXScale;
	Deform(1, 1) = dYScale;
	Deform(2, 2) = dZScale;
	MultiplyTransformation(Deform);
}

/*
void CLinearTransformation::AddShearX(double dYbydX, double dZbydX)
{
	CMatrix Deform;
	Deform.InitialiseIdentity(3);
	Deform(1, 0) = dYbydX;
	Deform(2, 0) = dZbydX;
	AddDeformation(Deform);
}

void CLinearTransformation::AddShearY(double dXbydY, double dZbydY)
{
	CMatrix Deform;
	Deform.InitialiseIdentity(3);
	Deform(0, 1) = dXbydY;
	Deform(2, 1) = dZbydY;
	AddDeformation(Deform);
}

void CLinearTransformation::AddShearZ(double dXbydZ, double dYbydZ)
{
	CMatrix Deform;
	Deform.InitialiseIdentity(3);
	Deform(0, 2) = dXbydZ;
	Deform(1, 2) = dYbydZ;
	AddDeformation(Deform);
}
*/
void CLinearTransformation::Rotate(WXYZ Rotation)
{
	MultiplyTransformation(ConvertRotation(Rotation));
}










