#pragma once
#include "TextileWeave.h"
#include "SectionEllipse.h"
#include "SectionPowerEllipse.h"
#include "SectionRotated.h"
#include "PatternDraft.h"
#include "Timer.h"

/*
 * Представляет 2d тканый текстиль.
 */
namespace TexGen {
	class CLASS_DECLSPEC CTextileWeave2D : public CTextileWeave {
	private:
		/*
		 * Улучшенный теекстиль.
		 */
		bool refine;
		/*
		 * Плоскостные тангенсы.
		 */
		bool inPlaneTangents;
	public:
		/*
		 * Сеттер улучшения текстиля.
		 * param setRefine - улучшенный текстиль.
		 */
		void setRefine(bool setRefine) {
			refine = setRefine;
		}
		/*
		 * Геттер улучшения текстиля.
		 * return - улучшенный текстиль.
		 */
		bool getRefine() const {
			return refine;
		}
		/*
		 * Сеттер плоскостных тангенсов.
		 * param setInPlaneTangents - плоскостные тангенсы.
		 */
		void setInPlaneTangents(bool setInPlaneTangents) {
			inPlaneTangents = setInPlaneTangents;
		}
		/*
		 * Геттер плоскостных тангенсов.
		 * return - плоскостные тангенсы.
		 */
		bool getInPlaneTangents() const {
			return inPlaneTangents;
		}

		/// Build a 2d weave unit cell of given width, height, yarn spacing and fabric thickness
		CTextileWeave2D(int iWidth, int iHeight, double dSpacing, double dThickness, bool bRefine = true, bool bInPlaneTangents = true);
		CTextileWeave2D(TiXmlElement &Element);
		~CTextileWeave2D(void) {};

		CTextile* Copy() const { return new CTextileWeave2D(*this); }
		string GetType() const { return "CTextileWeave2D"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType);

		string GetDefaultName() const;

		/// Swap the cross over order of yarns at given index
		void SwapPosition(int x, int y);
		/// Swap the cross over order of all yarns
		void SwapAll();
		/// Refine the textile to eliminate interference. 
		/**
		 * May be called subsequent to initial generation of textile 
		 * allowing weave to be modified and then refined
		 * Must get CTextile::GetWeave() to use as member of CTextileWeave not CTextile
		 */
		void RefineTextile( bool bCorrectWidths = true, bool bCorrectInterference = true, bool bPeriodic = true );
		
		virtual CDomainPlanes GetDefaultDomain( bool bSheared = false, bool bAddedDomainHeight = true );
		void ConvertToPatternDraft();
	protected:
		bool BuildTextile() const;
		virtual void Refine( bool bCorrectWidths = true, bool bCorrectInterference = true, bool bPeriodic = true ) const;
		void AdjustTangents() const;
		virtual bool AdjustSectionsForRotation( bool bPeriodic = true ) const;
		void SetInPlaneTangents() const;
		void SetInPlaneTangents( int Yarn ) const;
	};
}
