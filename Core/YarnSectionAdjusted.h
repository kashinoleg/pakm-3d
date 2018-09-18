#pragma once
#include "YarnSection.h"

namespace TexGen {
	using namespace std;

	/// Bezier interpolation for yarn paths
	class CLASS_DECLSPEC CYarnSectionAdjusted : public CYarnSection {
	public:
		CYarnSectionAdjusted(const CYarnSection &YarnSection);
		CYarnSectionAdjusted(TiXmlElement &Element);
		~CYarnSectionAdjusted(void);

		CYarnSection* Copy() const { return new CYarnSectionAdjusted(*this); }
		string GetType() const { return "CYarnSectionAdjusted"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		vector<XY> GetSection(const YARN_POSITION_INFORMATION PositionInfo, int iNumPoints, bool bEquiSpaced = false) const;
		CMesh GetSectionMesh(const YARN_POSITION_INFORMATION PositionInfo, int iNumPoints, bool bEquiSpaced) const;

		/// At given index and value t the position of the node should be adjusted by given vector
		void AddAdjustment(int iIndex, double t, const vector<pair<double, XY> > &SectionAdjust);

	protected:
		typedef vector<pair<double, XY> > SECTION_ADJUST;
		typedef vector<vector<pair<double, SECTION_ADJUST> > > YARN_ADJUST;

		/// Represents the adjustements to the interpolation
		/**
		The first value in the pair represents the T value (distance along the yarn), the second
		value represents the displacement vector.
		*/
		YARN_ADJUST m_Adjustments;
		CObjectContainer<CYarnSection> m_pYarnSection;
	};
}
