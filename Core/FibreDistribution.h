#pragma once
#include "Misc.h"
#include "tinyxml.h"

using namespace std;

/// Abstract base class that defines how the fibres are distributed within a yarn
/**
* This class is there to provide local volume fraction information for a yarn
* cross section, from this different Vf distributions can be derived
*/
namespace TexGen {
	class CLASS_DECLSPEC CFibreDistribution {
	public:
		CFibreDistribution() {};
		CFibreDistribution(TiXmlElement &Element) {};
		virtual ~CFibreDistribution(void) {};

		/// Create a copy of the derived fibre distribution and return a pointer to the newly created instance
		virtual CFibreDistribution* Copy() const = 0;
		/// Derived class should return the class name
		virtual string GetType() const = 0;
		/// Used for saving data to XML
		virtual void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		/// Get the volume fraction for a given location
		virtual double GetVolumeFraction(const vector<XY> &Section, double dFibreArea, XY Location) const = 0;
		/// Get the volume fraction given an area
		virtual double GetVolumeFraction(double dArea, double dFibreArea) const;
	};
}
