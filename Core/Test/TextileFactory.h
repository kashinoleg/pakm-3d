#pragma once
#include "TexGen.h"

using namespace TexGen;

class CTextileFactory
{
public:
	CTextileFactory() {};
	~CTextileFactory() {};

	CTextileWeave2D SatinWeave();
	CTextileWeave3D Weave3D8x4();
	/**
	Yarn includes
	CYarnSection: Constant, NodeInterp, PositionInterp
	Interpolation: Bezier, Cubic
	Sections: Ellipse, Lenticular
	*/
	CTextile InterpTest();
	/**
	Yarn includes
	CYarnSection: PositionInterp
	Interpolation: CInterpolationBezier
	Sections: CSectionBezier, CSectionEllipse, CSectionLenticular, CSectionPolygon, CSectionPowerEllipse
		CSectionRotated, CSectionScaled, CSectionHybrid
	*/
	CTextile SectionsTest();

	CTextile GetSingleYarn(int iNumMasterNodes, int iResolution);

	// Plain weave with rectangular mesh assigned
	CTextileWeave2D MeshedWeave();

	// Cotton weave with properties set
	CTextileWeave2D CottonWeave();

	// Simple plain weave
	CTextileWeave2D PlainWeave();

	// Straight yarns with some properties set
	CTextile StraightYarns();
};
