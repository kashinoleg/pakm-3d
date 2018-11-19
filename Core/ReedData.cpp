#include "stdafx.h"
#include "ReedData.h"

using namespace TexGen;

CReedData::CReedData() {
}

CReedData::~CReedData() {
}

double CReedData::GetClosestReedThickness(double dThickness) {
	vector<double> ReedSizes(ReedThickness, ReedThickness + NUM_REED_SIZES);
	vector<double>::iterator after = lower_bound( ReedSizes.begin(), ReedSizes.end(), dThickness );
	if (after == ReedSizes.begin()) {
		return *after;
	}
	if (after == ReedSizes.end()) {
		return(*(after - 1));
	}
	return (*after - dThickness) < ( dThickness - *(after-1) ) ? *after : *(after - 1);
}
