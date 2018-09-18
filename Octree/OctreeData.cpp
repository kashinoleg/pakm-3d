#include "OctreeData.hpp"

using namespace hxa;

OctreeData::OctreeData
(
	const OctreeDimensions& dimensions
)
	: bound_m(dimensions.getPosition(), dimensions.getSize())
	, level_m(0)
	, pDimensions_m(&dimensions)
{
}


OctreeData::OctreeData
(
	const OctreeData& parentCellData,
	const int       subCellIndex
)
	: bound_m(parentCellData.bound_m, subCellIndex)
	, level_m(parentCellData.level_m + 1)
	, pDimensions_m(parentCellData.pDimensions_m)
{
}


OctreeData::OctreeData
(
	const OctreeData&       other,
	const OctreeDimensions& dimensions
)
	: bound_m(other.bound_m)
	, level_m(other.level_m)
	, pDimensions_m(&dimensions)
{
}

OctreeData::OctreeData
(
	const OctreeData& other
)
	: bound_m(other.bound_m)
	, level_m(other.level_m)
	, pDimensions_m(other.pDimensions_m)
{
}


OctreeData& OctreeData::operator=(const OctreeData& other) {
	if (&other != this) {
		bound_m = other.bound_m;
		level_m = other.level_m;
		pDimensions_m = other.pDimensions_m;
	}
	return *this;
}
