#pragma once
#include "FibreDistribution.h"
#include "Logger.h"

namespace TexGen {
	/// Fibre volume fraction is constant throughout the yarn
	class CLASS_DECLSPEC CFibreDistributionConst : public CFibreDistribution {
	public:
		CFibreDistributionConst();
		CFibreDistributionConst(TiXmlElement &Element);
		~CFibreDistributionConst() {};

		CFibreDistribution* Copy() const { return new CFibreDistributionConst(*this); }
		string GetType() const { return "CFibreDistributionConst"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		double GetVolumeFraction(const vector<XY> &Section, double dFibreArea, XY Location) const;
		double GetVolumeFraction(double dArea, double dFibreArea) const;
	protected:
		double IntegrateDistribution(const vector<XY> &Section) const;
	};
};
