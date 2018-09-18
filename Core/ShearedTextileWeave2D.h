#pragma once
#include "TextileWeave2D.h"
#include "SectionPolygon.h"
#include "SectionRotated.h"
#include "SectionEllipse.h"
#include "YarnData.h"
#include "Timer.h"

/*
 * Класс - тканный 2D-текстиль со сдвигом.
 * Параметры вводятся для необработанного текстиля, и затем применяется сдвиг.
 * Это приводит к уменьшению расстояния в направлении y.
 */
namespace TexGen {
	class CLASS_DECLSPEC CShearedTextileWeave2D : public CTextileWeave2D {
	private:
		/*
		 * Угол сдвига текстиля.
		 */
		double shearAngle;
		/*
		 * Домен со сдвигом.
		 */
		bool shearedDomain;
		/// Vector containing the dx and dy components for spacings along y yarns
		/*
		 * Вектор, содержащий компоненты dx и dy расстояний вдоль y нитей.
		 */
		mutable vector<XY> m_YSpacing;
	public:
		/*
		 * Сеттер угла сдвига текстиля.
		 * param setShearAngle - угол сдвига текстиля.
		 */
		void setShearAngle(double setShearAngle) {
			shearAngle = setShearAngle;
		}
		/*
		 * Геттер угла сдвига текстиля.
		 * return - угол сдвига текстиля.
		 */
		double getShearAngle() const {
			return shearAngle;
		}
		/*
		 * Сеттер сдвинут ли домен.
		 * param setShearedDomain - сдвинут ли домен.
		 */
		void setShearedDomain(bool setShearedDomain) {
			shearedDomain = setShearedDomain;
		}
		/*
		 * Геттер сдвинут ли домен.
		 * return - сдвинут ли домен.
		 */
		bool GetShearedDomain() const {
			return shearedDomain;
		}


		/// Build a 2d weave unit cell of given width, height, yarn spacing and fabric thickness
		/**
		\param dThickness  Textile thickness
		\param ShearAngle  Angle in radians
		*/
		CShearedTextileWeave2D(int iWidth, int iHeight, double dSpacing, double dThickness,
			double ShearAngle, bool bRefine = true, bool bInPlaneTangents = true);
		CShearedTextileWeave2D(TiXmlElement &Element);
		~CShearedTextileWeave2D(void);

		CTextile* Copy() const { return new CShearedTextileWeave2D(*this); }
		string GetType() const { return "CShearedTextileWeave2D"; }
		void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType);

		/// Creates the domain as a box if bSheared = false
		/// Planes parallel to the y yarns (ie oriented at the shear angle) if bSheared = true
		CDomainPlanes GetDefaultDomain( bool bSheared = false, bool bAddedDomainHeight = true );
	protected:
		bool BuildTextile() const;
		/// Get the x and y components of spacings along y yarns
		void GetYSpacings() const;
		/// Sums all y components of spacings to get y repeat
		XY GetYRepeat() const;

		void Refine() const;
		bool AdjustSectionsForRotation() const;
		double CalcSinAngle( double R, double Width, double Height ) const;
		bool ConvertToInterpNodes() const;

		/// Adjust cross section shapes to correct interference
		/**
		Cross section shapes will be adjusted in order to remove interference, this is done
		by shrinking the position of the nodes where they interfere. The cross sections are
		replaced with polygons to provide an interference free shape.
		\param bMaxCorrection Corrects by whole of modifier if true, by half of modifier if false (Creates gaps with large intersections if set to true)
		*/
		void CorrectInterference( bool bMaxCorrection = false ) const;
		void CalculateModifiers( const CInterpolation *pInterpolation, int Yarn, YARN_POSITION_INFORMATION YarnPosInfo, CMesh& Mesh, vector<vector<vector<double> > >& YarnSectionModifiers, bool bMaxCorrection) const;
		void ModifySection(CYarnSectionInterpNode* pYarnSection, YARN_POSITION_INFORMATION& YarnPosInfo, vector<double>& Modifiers, int iNodeIndex, int iMidIndex = -1) const;

		void AdjustSections() const;
		/// Create meshes for top and bottom surfaces of domain
		void CreateTopAndBottomMeshes( CMesh& TopMesh, CMesh& BottomMesh ) const;
		void CalculateModifiers( const CInterpolation *pInterpolation, int Yarn, YARN_POSITION_INFORMATION YarnPosInfo, CMesh& Mesh, vector<double>& YarnSectionModifiers) const;
		XY Get2DRefPoint( XY& Point, double dAngle ) const;
		XY RotatePoint( XY& Point, double dAngle ) const;
		void ModifySection(CYarnSectionInterpNode* pYarnSection, YARN_POSITION_INFORMATION& YarnPosInfo, vector<double>& Modifiers, double dTargetArea, double dOriginalHeight, int iNodeIndex, int iMidIndex = -1) const;
		double GetArea( vector<XY>& Points ) const;
		void ReplaceSection( vector<XY>& Points, CYarnSectionInterpNode* pYarnSection, int iNodeIndex, int iMidIndex ) const;
		/// Swap x and y in GetCell call if transverse yarn is x yarn
		const vector<PATTERN2D> &GetCellDir( int x, int y, int iDir ) const;
		int ModifyPoints( vector<XY>& Points, vector<double>& Modifiers, vector<XY>& MaxHeight, int iModCount,
			double dy, vector<XY>& NewPoints ) const;
	};
}
