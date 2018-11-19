#include "stdafx.h"
#include "InterpolationAdjusted.h"

using namespace TexGen;

CInterpolationAdjusted::CInterpolationAdjusted(const CInterpolation &Interpolation)
: CInterpolation(true, false, false)	// this is not important
, m_pInterpolation(Interpolation)
{
}

CInterpolationAdjusted::~CInterpolationAdjusted(void)
{
}

CInterpolationAdjusted::CInterpolationAdjusted(TiXmlElement &Element)
: CInterpolation(Element)
{
	FOR_EACH_TIXMLELEMENT(pAdjustment, Element, "Adjustment")
	{
		vector<pair<double, XYZ> > Adjustments;
		FOR_EACH_TIXMLELEMENT(pAdjustmentSection, *pAdjustment, "AdjustmentSection")
		{
			pair<double, XYZ> Adjustment(0.0, XYZ());
			pAdjustmentSection->Attribute("T", &Adjustment.first);
			Adjustment.second = valueify<XYZ>(pAdjustmentSection->Attribute("Vector"));
			Adjustments.push_back(Adjustment);
		}
		m_Adjustments.push_back(Adjustments);
	}
	TiXmlElement* pInterpolation = Element.FirstChildElement("Interpolation");
	if (pInterpolation)
		m_pInterpolation = CreateInterpolation(*pInterpolation);
}

void CInterpolationAdjusted::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	CInterpolation::PopulateTiXmlElement(Element, OutputType);
	int i, j;
	for (i=0; i<(int)m_Adjustments.size(); ++i)
	{
		TiXmlElement Adjustment("Adjustment");
		Adjustment.SetAttribute("index", i);
		for (j=0; j<(int)m_Adjustments[i].size(); ++j)
		{
			TiXmlElement AdjustmentSection("AdjustmentSection");
			AdjustmentSection.SetAttribute("T", stringify(m_Adjustments[i][j].first));
			AdjustmentSection.SetAttribute("Vector", stringify(m_Adjustments[i][j].second));
			Adjustment.InsertEndChild(AdjustmentSection);
		}
		Element.InsertEndChild(Adjustment);
	}
	TiXmlElement Interpolation("Interpolation");
	m_pInterpolation->PopulateTiXmlElement(Interpolation, OutputType);
	Element.InsertEndChild(Interpolation);
}

CSlaveNode CInterpolationAdjusted::GetNode(const vector<CNode> &MasterNodes, int iIndex, double t) const
{
	CSlaveNode Node = m_pInterpolation->GetNode(MasterNodes, iIndex, t);

	if (iIndex < 0 || iIndex >= (int)m_Adjustments.size())
	{
		TGERROR("Unable to adjust node position. Index out of range: " << iIndex);
		return Node;
	}

	XYZ NewPos = Node.GetPosition() + GetInterpedValue(m_Adjustments[iIndex], t);
	Node.SetPosition(NewPos);

	return Node;
}

void CInterpolationAdjusted::Initialise(const vector<CNode> &MasterNodes) const
{
	m_pInterpolation->Initialise(MasterNodes);
}

void CInterpolationAdjusted::AddAdjustment(int iIndex, double t, XYZ Vector)
{
	if (iIndex < 0)
	{
		TGERROR("Unable to add adjustment. Index out of range: " << iIndex);
		return;
	}
	if (iIndex >= (int)m_Adjustments.size())
	{
		m_Adjustments.resize(iIndex+1);
	}
	m_Adjustments[iIndex].push_back(make_pair(t, Vector));
	// Sort the vector to make sure t values are ordered from lowest to highest
	sort(m_Adjustments[iIndex].begin(), m_Adjustments[iIndex].end());
}




