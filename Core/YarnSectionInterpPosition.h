#pragma once
#include "YarnSectionInterp.h"

namespace TexGen { 
	using namespace std;

	/// Interpolate sections between arbritrary positions along the length of the yarn
	/**
	Sections should be added to the class by use of the AddSection function.
	*/
	class CLASS_DECLSPEC CYarnSectionInterpPosition : public CYarnSectionInterp {
	public:
		CYarnSectionInterpPosition(bool bRamped = true, bool bPolar = false, bool bConstMesh = true);
		CYarnSectionInterpPosition(TiXmlElement &Element);
		~CYarnSectionInterpPosition(void);

		vector<XY> GetSection(const YARN_POSITION_INFORMATION PositionInfo, int iNumPoints, bool bEquiSpaced = false) const;
		CMesh GetSectionMesh(const YARN_POSITION_INFORMATION PositionInfo, int iNumPoints, bool bEquiSpaced) const;
		CYarnSection* Copy() const { return new CYarnSectionInterpPosition(*this); }
		string GetType() const { return "CYarnSectionInterpPosition"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		/// Add a section at a specific point along the path of the yarn
		/**
		Positions in between specified sections will be interpolated. If no section
		is specified at the start and end then the sections are applied periodically.
		\param dPosition Represents the position along the length of the yarn
			where the section is to be applied, this value should vary between 0 and 1
		\param Section The section to be applied at that position
		*/
		void AddSection(double dPosition, const CSection &Section);

		/// Function to set the number of layers equal for all sections in a given yarn
		void SetSectionMeshLayersEqual( int iNumPoints ) const;

		// Accessor methods
		int GetNumNodeSections() const { return (int)m_Sections.size(); }
		const CSection &GetSection(int iIndex) const;
		double GetSectionPosition(int iIndex) const;

	protected:
		vector<pair<double, CObjectContainer<CSection> > > m_Sections;
	};
}
