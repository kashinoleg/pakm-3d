#pragma once
#include "YarnSection.h"

namespace TexGen {
	using namespace std;

	class CSection;

	/// Abstract base class to handle interpolation for derived classes
	class CLASS_DECLSPEC CYarnSectionInterp : public CYarnSection {
	public:
		/**
		If bRamped is set to true, the interpolation fraction given to functions in GetInterpedSection
		and GetInterpedSectionMesh will be ramped using a cubic equation. Where du = 0 at u = 0, and du = 0
		at u = 1. This provides a smoother transition between sections when the interpolation begins and ends.
		*/
		CYarnSectionInterp(bool bRamped, bool bPolar, bool bConstMesh = true);
		CYarnSectionInterp(TiXmlElement &Element);
		virtual ~CYarnSectionInterp(void);

		virtual void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		// Accessor methods
		bool GetRamped() { return m_bRamped; }
		bool GetPolar() { return m_bPolar; }

	protected:
		bool GetInterpedSection(const CSection &Section1, const CSection &Section2, double u, int iNumPoints, bool bEquiSpaced, vector<XY> &Points) const;
		bool GetInterpedSectionMesh(const CSection &Section1, const CSection &Section2, double u, int iNumPoints, bool bEquiSpaced, CMesh &Mesh) const;
		XY InterpolatePoints(XY P1, XY P2, double u) const;
		int CalculateNumberofLayers(const vector<XY> &Section) const;

		bool m_bRamped;
		bool m_bPolar;
	};
}
