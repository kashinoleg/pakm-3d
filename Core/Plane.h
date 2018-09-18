#pragma once
#include "mymath.h"

namespace TexGen {
	/// Struct for representing a Plane
	struct CLASS_DECLSPEC PLANE {
	public:
		PLANE();
		PLANE(XYZ Normal, double d);

		XYZ Normal;
		double d;
	};

	inline PLANE::PLANE()
	: d(0)
	{
	}

	inline PLANE::PLANE(XYZ Normal, double d)
	: Normal(Normal)
	, d(d)
	{
		if (GetLength(this->Normal) == 0)
			assert(false);
		else
			Normalise(this->Normal);
	}
}
