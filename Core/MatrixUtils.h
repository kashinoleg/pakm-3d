#pragma once
#include "mymath.h"
#include "Matrix.h"

namespace TexGen {
	using namespace std;

	// Maths from http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
	inline CMatrix ConvertRotation(WXYZ Q)
	{
		CMatrix Mat;
		Mat.InitialiseIdentity(3);
		// Diagonal
		Mat(0, 0) = Q.w*Q.w + Q.x*Q.x - Q.y*Q.y - Q.z*Q.z;
		Mat(1, 1) = Q.w*Q.w - Q.x*Q.x + Q.y*Q.y - Q.z*Q.z;
		Mat(2, 2) = Q.w*Q.w - Q.x*Q.x - Q.y*Q.y + Q.z*Q.z;
		// Upper right
		Mat(0, 1) = 2*Q.x*Q.y - 2*Q.w*Q.z;
		Mat(0, 2) = 2*Q.w*Q.y + 2*Q.x*Q.z;
		Mat(1, 2) = 2*Q.y*Q.z - 2*Q.w*Q.x;
		// Lower left
		Mat(1, 0) = 2*Q.w*Q.z + 2*Q.x*Q.y;
		Mat(2, 0) = 2*Q.x*Q.z - 2*Q.w*Q.y;
		Mat(2, 1) = 2*Q.w*Q.x + 2*Q.y*Q.z;
		return Mat;
	}

	inline XYZ operator * (const CMatrix &Transform, const XYZ &Vector)
	{
		CMatrix VecMat;
		VecMat.Initialise(3, 1);
		VecMat(0, 0) = Vector.x;
		VecMat(1, 0) = Vector.y;
		VecMat(2, 0) = Vector.z;
		VecMat = Transform * VecMat;
		return XYZ(VecMat(0, 0), VecMat(1, 0), VecMat(2, 0));		
	}
}
