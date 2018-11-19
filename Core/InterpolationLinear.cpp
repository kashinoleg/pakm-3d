#include "stdafx.h"
#include "InterpolationLinear.h"

using namespace TexGen;

CInterpolationLinear::CInterpolationLinear(bool bPeriodic, bool bForceInPlaneTangent, bool bForceMasterNodeTangent)
: CInterpolation(bPeriodic, bForceInPlaneTangent, bForceMasterNodeTangent)
{
}

CInterpolationLinear::~CInterpolationLinear(void)
{
}

CInterpolationLinear::CInterpolationLinear(TiXmlElement &Element)
: CInterpolation(Element)
{
	// TODO: IMPLEMENT ME
}

/*
void CInterpolationLinear::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
}*/

CSlaveNode CInterpolationLinear::GetNode(const vector<CNode> &MasterNodes, int iIndex, double t) const
{
	assert(iIndex >= 0 && iIndex < int(MasterNodes.size()-1));
	// Errors may occur here if the Initialise function was not called before
	// calling this function. For performance reasons it is necessary to call
	// initialise once at the start before making calls to this function
	if ( m_Tangents.empty() )
		Initialise( MasterNodes );
	assert(m_Tangents.size() == MasterNodes.size());

	const CNode &Node1 = MasterNodes[iIndex];
	const CNode &Node2 = MasterNodes[iIndex+1];

	XYZ P1, P2;
	XYZ T1, T2, U1, U2;
	XYZ SlaveNodePos, SlaveTangent;

	// This is a rather inefficient way of doing things, these points could be calculated
	// in the initialise function (optimise if this becomes an issue)
	P1 = Node1.GetPosition();
	P2 = Node2.GetPosition();
	T1 = m_Tangents[iIndex];
	T2 = m_Tangents[iIndex+1];

	SlaveNodePos = P1 + (P2-P1)*t;
	SlaveTangent = T1 + (T2-T1)*t;

	if (m_bForceInPlaneTangent)
		SlaveTangent.z = 0;

	Normalise(SlaveTangent);

	CSlaveNode NewNode(SlaveNodePos, SlaveTangent, XYZ());

	InterpolateUp(Node1, Node2, NewNode, t);
	InterpolateAngle(Node1, Node2, NewNode, t);

	NewNode.SetIndex(iIndex);
	NewNode.SetT(t);

	return NewNode;
}

void CInterpolationLinear::Initialise(const vector<CNode> &MasterNodes) const
{
	CalculateNodeCoordinateSystem(MasterNodes, m_Tangents);
}






