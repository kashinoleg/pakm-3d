#pragma once
#include "Section.h"
#include "SectionEllipse.h"
#include "Logger.h"

/// <summary>
/// Гибрид любого числа других сечений.
/// </summary>
namespace TexGen {
	class CLASS_DECLSPEC CSectionHybrid : public CSection {
	protected:
		vector<double> m_Divisions;
		vector<CObjectContainer<CSection> > m_Sections;
	public:
		/// Empty hybrid section, parts can be added with the AddPart function
		CSectionHybrid(void);
		/// Hybrid of two sections split into the upper and lower half
		CSectionHybrid(const CSection &TopHalf, const CSection &BottomHalf);
		/// Hybrid of four sections split into four quadrants
		CSectionHybrid(const CSection &TopRight, const CSection &TopLeft, const CSection &BottomLeft, const CSection &BottomRight);
		CSectionHybrid(TiXmlElement &Element);
		~CSectionHybrid(void);

		bool operator == (const CSection &CompareMe) const;
		CSection* Copy() const { return new CSectionHybrid(*this); }

		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const;

		string GetType() const { return "CSectionHybrid"; }
		string GetDefaultName() const;

		/// Add a division where the sections will transfer from one to the other
		void AddDivision(double dFraction);
		/// Assign a section between divisions
		bool AssignSection(int iIndex, const CSection &Section);

		// Accessor methods
		int GetNumDivisions() const { return (int)m_Divisions.size(); }
		int GetNumSections() { return (int)m_Sections.size(); }
		double GetDivision(int iIndex) const;
		const CSection &GetSection(int iIndex) const;
		XY GetPoint(double t) const;
	};
}
