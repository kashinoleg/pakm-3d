#include "InterpolationBezier.h"

using namespace TexGen;
CInterpolationBezier::CInterpolationBezier(bool bPeriodic, bool bForceInPlaneTangent, bool bForceMasterNodeTangent)
: CInterpolation(bPeriodic, bForceInPlaneTangent, bForceMasterNodeTangent)
{
}

CInterpolationBezier::~CInterpolationBezier(void)
{
}

CInterpolationBezier::CInterpolationBezier(TiXmlElement &Element)
: CInterpolation(Element)
{
	// TODO: IMPLEMENT ME
}

/*
void CInterpolationBezier::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
}*/

CSlaveNode CInterpolationBezier::GetNode(const vector<CNode> &MasterNodes, int iIndex, double t) const
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

	XYZ P1, P2, P3, P4;
	XYZ T1, T2, U1, U2;
	XYZ SlaveNodePos, SlaveTangent;
	double dLength;

	// This is a rather inefficient way of doing things, these points could be calculated
	// in the initialise function (optimise if this becomes an issue)
	P1 = Node1.GetPosition();
	P4 = Node2.GetPosition();
	T1 = m_Tangents[iIndex];
	T2 = m_Tangents[iIndex+1];
	dLength = ::GetLength(P1, P4)/3;
	P2 = P1 + T1 * dLength;
	P3 = P4 - T2 * dLength;
/*
//	int iIters = 0;
	double dMidLength, dMiddiff;
	do
	{
		P2 = P1 + T1 * dLength;
		P3 = P4 - T2 * dLength;
		dMidLength = GetLength(P2, P3);
		dMiddiff = dMidLength - dLength;
		dLength += dMiddiff/3;
//		++iIters;
	} while(dMiddiff > 1e-12);
//	cout << "Frac: " << dLength/::GetLength(P1, P4) << ", Iters: " << iIters << endl;
*/

	SlaveNodePos = CalculateBezierPoint(P1, P2, P3, P4, t);
	SlaveTangent = CalculateBezierTangent(P1, P2, P3, P4, t);

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

void CInterpolationBezier::Initialise(const vector<CNode> &MasterNodes) const
{
	CalculateNodeCoordinateSystem(MasterNodes, m_Tangents);
}

