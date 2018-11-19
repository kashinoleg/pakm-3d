#include "stdafx.h"
#include "FibreDistribution.h"

using namespace TexGen;

void CFibreDistribution::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const {
	Element.SetAttribute("type", GetType());
}

double CFibreDistribution::GetVolumeFraction(double dArea, double dFibreArea) const {
	return -1.0;
}
