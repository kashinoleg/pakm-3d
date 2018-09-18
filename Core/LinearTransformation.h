#pragma once
#include "Matrix.h"
#include "MatrixUtils.h"
#include "Logger.h"

namespace TexGen {
	using namespace std;

	class CTextile;

	/// Represents a linear transformation as a 3x3 matrix
	/**
	This class is derived from the CMatrix class, and adds a few additional functions
	specific to 3x3 linear transformation matrices.
	*/
	class CLASS_DECLSPEC CLinearTransformation : public CMatrix
	{
	public:
		CLinearTransformation(void);
		CLinearTransformation(const CMatrix &Matrix);
		~CLinearTransformation(void);

		void ResetTransformation();
		void MultiplyTransformation(const CMatrix &Deformation);
		void AddScale(double dXScale = 1, double dYScale = 1, double dZScale = 1);
		void AddSheardYbydX(double dValue) { (*this)(1, 0) += dValue; }
		void AddSheardZbydX(double dValue) { (*this)(2, 0) += dValue; }
		void AddSheardXbydY(double dValue) { (*this)(0, 1) += dValue; }
		void AddSheardZbydY(double dValue) { (*this)(2, 1) += dValue; }
		void AddSheardXbydZ(double dValue) { (*this)(0, 2) += dValue; }
		void AddSheardYbydZ(double dValue) { (*this)(1, 2) += dValue; }
/*		void AddShearX(double dYbydX = 0, double dZbydX = 0);
		void AddShearY(double dXbydY = 0, double dZbydY = 0);
		void AddShearZ(double dXbydZ = 0, double dYbydZ = 0);*/
		void Rotate(WXYZ Rotation);
	};
}
