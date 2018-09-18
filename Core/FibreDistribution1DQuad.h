#pragma once
#include "FibreDistribution.h"
#include "Logger.h"

/// Fibre volume fraction is defined as a quadratic equation varying only along the X axis
namespace TexGen {
	class CLASS_DECLSPEC CFibreDistribution1DQuad : public CFibreDistribution {
	public:
		CFibreDistribution1DQuad(double dDropOff);
		CFibreDistribution1DQuad(TiXmlElement &Element);
		~CFibreDistribution1DQuad() {};

		CFibreDistribution* Copy() const { return new CFibreDistribution1DQuad(*this); }
		string GetType() const { return "CFibreDistribution1DQuad"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		double GetVolumeFraction(const vector<XY> &Section, double dFibreArea, XY Location) const;

	protected:
		double ComputeMaxX(const vector<XY> &Section) const;
		double Distribution(double max, double min, double x, double dMaxX) const;
		double IntegrateDistribution(const vector<XY> &Section, double dMaxX) const;

		double m_dDropOff;
	};
}
