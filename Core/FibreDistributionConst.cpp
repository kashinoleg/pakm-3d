#include "stdafx.h"
#include "FibreDistributionConst.h"
#include "Section.h"

using namespace TexGen;

CFibreDistributionConst::CFibreDistributionConst()
{
}

CFibreDistributionConst::CFibreDistributionConst(TiXmlElement &Element)
: CFibreDistribution(Element)
{
}

void CFibreDistributionConst::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	CFibreDistribution::PopulateTiXmlElement(Element, OutputType);
}

double CFibreDistributionConst::IntegrateDistribution(const vector<XY> &Section) const
{
	return CSection::GetArea(Section);
}

double CFibreDistributionConst::GetVolumeFraction(const vector<XY> &Section, double dFibreArea, XY Location) const
{
	double dArea = IntegrateDistribution(Section);
	double dVf = dFibreArea/dArea;
	if (dVf>0.86||dVf<0)
	{
		TGERROR("Warning: Volume fraction is not realistic: " << dVf);
	}
	return dVf;
}

double CFibreDistributionConst::GetVolumeFraction(double dArea, double dFibreArea) const
{
	double dVf = dFibreArea/dArea;
	if (dVf>0.86||dVf<0)
	{
		TGERROR("Warning: Volume fraction is not realistic: " << dVf);
	}
	return dVf;
}

