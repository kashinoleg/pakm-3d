#pragma once
#include "Textile3DWeave.h"
#include "SectionPowerEllipse.h"

namespace TexGen {
	using namespace std;

	/// Represents a 3D angle interlock woven textile
	class CLASS_DECLSPEC CTextileAngleInterlock : public CTextile3DWeave {
	public:

		/// Build a weave unit cell of given width, height, number of layers, yarn spacing and fabric thickness
		CTextileAngleInterlock(int iNumXYarns, int iNumYYarns, double dXSpacing, double dYSpacing,
							double dXHeight, double dYHeight);
		CTextileAngleInterlock(TiXmlElement &Element);
		virtual ~CTextileAngleInterlock(void);

		virtual CTextile* Copy() const { return new CTextileAngleInterlock(*this); }
		virtual string GetType() const { return "CTextileAngleInterlock"; }
		virtual void PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType);

		void SetBinderYarnPositions(int x, int y);

		/// Find min and max z values for the textile
		//virtual void FindMinMaxZ( double& dMinZ, double& dMaxZ );
		virtual void SetupLayers(int iNumWarpLayers, int iNumWeftLayers, int iNumBinderLayers = 1);
		virtual bool BuildTextile() const;

	protected:
		/// Shape the binder yarns around the adjacent weft yarns
		void ShapeBinderYarns() const;
		/// Add extra nodes to binder yarns to match shape of adjacent weft yarns
		int AddBinderNodes(int CurrentNode, int i, int j) const;

		void SetBinderPosition(int z, vector<PATTERN3D>& Cell);
		int FindBinderHeight(const vector<PATTERN3D>& Cell) const;
	};
}
