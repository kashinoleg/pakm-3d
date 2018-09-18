#pragma once
#include "YarnSection.h"

namespace TexGen {
	using namespace std;

	/// Creates a section which is constant all along the yarn
	class CLASS_DECLSPEC CYarnSectionConstant : public CYarnSection {
	public:
		CYarnSectionConstant(const CSection &Section);
		CYarnSectionConstant(TiXmlElement &Element);
		~CYarnSectionConstant(void);

		CYarnSection* Copy() const { return new CYarnSectionConstant(*this); }
		string GetType() const { return "CYarnSectionConstant"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		vector<XY> GetSection(const YARN_POSITION_INFORMATION PositionInfo, int iNumPoints, bool bEquiSpaced = false) const;
		CMesh GetSectionMesh(const YARN_POSITION_INFORMATION PositionInfo, int iNumPoints, bool bEquiSpaced) const;

		// Accessor methods
		const CSection &GetSection() const { return *m_pSection; }

	protected:
		CObjectContainer<CSection> m_pSection;
	};
}
