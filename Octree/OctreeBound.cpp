#include "OctreeBound.hpp"

using namespace hxa;


/// standard object services ---------------------------------------------------
OctreeBound::OctreeBound() {
	positionOfLowerCorner_m = OctreeVector(0.0f, 0.0f, 0.0f);
	center_m = OctreeVector(0.5f, 0.5f, 0.5f);
	positionOfUpperCorner_m = OctreeVector(1.0f, 1.0f, 1.0f);
	circumSphereRadius_m = center_m.length();
}


OctreeBound::OctreeBound
(
	const OctreeVector& positionOfLowerCorner,
	const float     size
)
	: positionOfLowerCorner_m(positionOfLowerCorner)
	, positionOfUpperCorner_m(positionOfLowerCorner +
		OctreeVector(size, size, size))
	, center_m((positionOfLowerCorner_m + positionOfUpperCorner_m)
		*= 0.5f)
	, circumSphereRadius_m((OctreeVector::HALF() * size).length())
{


}

OctreeBound::OctreeBound(const OctreeBound& parentCellBound, const int subCellIndex) {
	{
		const OctreeVector* lowMidHigh[] =
		{
			&(parentCellBound.positionOfLowerCorner_m),
			&(parentCellBound.center_m),
			&(parentCellBound.positionOfUpperCorner_m)
		};

		positionOfLowerCorner_m.setX(lowMidHigh[subCellIndex & 1]->getX());
		positionOfLowerCorner_m.setY(lowMidHigh[(subCellIndex >> 1) & 1]->getY());
		positionOfLowerCorner_m.setZ(lowMidHigh[(subCellIndex >> 2) & 1]->getZ());

		positionOfUpperCorner_m.setX((lowMidHigh + 1)[subCellIndex & 1]->getX());
		positionOfUpperCorner_m.setY((lowMidHigh + 1)[(subCellIndex >> 1) & 1]->getY());
		positionOfUpperCorner_m.setZ((lowMidHigh + 1)[(subCellIndex >> 2) & 1]->getZ());
	}

	((center_m = positionOfLowerCorner_m) += positionOfUpperCorner_m) *= 0.5f;
	circumSphereRadius_m = parentCellBound.circumSphereRadius_m * 0.5f;
}

OctreeBound::OctreeBound(const OctreeBound& other)
	: positionOfLowerCorner_m(other.positionOfLowerCorner_m),
	positionOfUpperCorner_m(other.positionOfUpperCorner_m),
	center_m(other.center_m),
	circumSphereRadius_m(other.circumSphereRadius_m)
{
}

OctreeBound& OctreeBound::operator=(const OctreeBound& other) {
	if (&other != this) {
		positionOfLowerCorner_m = other.positionOfLowerCorner_m;
		positionOfUpperCorner_m = other.positionOfUpperCorner_m;
		center_m = other.center_m;
		circumSphereRadius_m = other.circumSphereRadius_m;
	}
	return *this;
}
