#include "TextileWeave2D.h"

using namespace TexGen;

CTextileWeave2D::CTextileWeave2D(int iWidth, int iHeight, double dSpacing, double dThickness, bool bRefine, bool bInPlaneTangents )
: CTextileWeave(iWidth, iHeight, dSpacing, dThickness) {
	setRefine(bRefine);
	setInPlaneTangents(bInPlaneTangents);
	vector<bool> Cell;
	Cell.push_back(PATTERN_YYARN);
	Cell.push_back(PATTERN_XYARN);
	for (int i = 0; i < m_iNumYYarns; i++) {
		for (int j = 0; j < m_iNumXYarns; j++) {
			GetCell(i, j) = Cell;
		}
	}
}

CTextileWeave2D::CTextileWeave2D(TiXmlElement &Element) : CTextileWeave(Element) {
	setRefine(valueify<bool>(Element.Attribute("Refine")));
	setInPlaneTangents(valueify<bool>(Element.Attribute("InPlaneTangents")));
}

void CTextileWeave2D::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) {
	CTextileWeave::PopulateTiXmlElement(Element, OutputType);
	Element.SetAttribute("Refine", stringify(getRefine()));
	Element.SetAttribute("InPlaneTangents", stringify(getInPlaneTangents()));
}

void CTextileWeave2D::SwapPosition(int x, int y) {
	if (x < 0 || x >= m_iNumYYarns || y < 0 || y >= m_iNumXYarns) {
		TGERROR("Unable to swap positions, index out of range: " << x << ", " << y);
		return;
	}
	vector<bool> &Cell = GetCell(x, y);
	assert(Cell.size() == 2);
	Cell[0] = !Cell[0];
	Cell[1] = !Cell[1];
	m_bNeedsBuilding = true;
}

void CTextileWeave2D::SwapAll() {
	for (int i = 0; i < m_iNumYYarns; i++) {
		for (int j = 0; j < m_iNumXYarns; j++) {
			SwapPosition(i, j);
		}
	}
	m_bNeedsBuilding = true;
}

bool CTextileWeave2D::AdjustSectionsForRotation( bool bPeriodic ) const {

	CYarn *pYarn;
	XYZ PrevXPos, NextXPos;
	XYZ PrevYPos, NextYPos;
	XYZ Up;
	CNode NewNode;

	// Assign more adequate cross sections
	double dWidth, dHeight;
	int iPrevYarnx, iPrevYarny;
	int iNextYarnx, iNextYarny;
//	int iNextCrossx, iNextCrossy;

	double dAngle;
	int iNumYarns;
	int iYarnLength;
	int x, y;

	// First loop for Y yarns, second loop for X yarns
	for (int iDirection = 0; iDirection < 2; ++iDirection) {
		if (iDirection==0) {
			iNumYarns = m_iNumYYarns;
			iYarnLength = m_iNumXYarns;
		}
		else {
			iNumYarns = m_iNumXYarns;
			iYarnLength = m_iNumYYarns;
		}
		int start = 0;
		if (!bPeriodic) {
			start = 1;
		}
		for (int i = start; i < iNumYarns; ++i) {
			if (iDirection==0) {
				iPrevYarnx = i-1;
				if (iPrevYarnx < 0) {
					iPrevYarnx += iNumYarns;
				}
				if (bPeriodic) {
					iNextYarnx = (i + 1) % iNumYarns;
				}
				else {
					//  Assumes that one extra node than number of yarns
					iNextYarnx = i + 1;
				}
				// The angle is the maximum rotation angle to apply to the yarn at points where it needs
				// rotating, specified in radians.
				dAngle = atan2(0.5*m_dFabricThickness, m_YYarnData[iPrevYarnx].dSpacing+m_YYarnData[i].dSpacing);
				// Get the yarn width and height for this X yarn
				dWidth = m_YYarnData[i].dWidth;
				dHeight = m_YYarnData[i].dHeight;
				// Get a pointer to the current yarn
				pYarn = &m_Yarns[m_YYarns[i][0]];
				// Used for getting the cell coordinates
				x = i;
			}
			else {
				iPrevYarny = i - 1;
				if (iPrevYarny < 0) {
					iPrevYarny += iNumYarns;
				}
				if (bPeriodic) {
					//  Assumes that one extra node than number of yarns
					iNextYarny = (i + 1) % iNumYarns;
				}
				else {
					iNextYarny = i + 1;
				}
				// The angle is the maximum rotation angle to apply to the yarn at points where it needs
				// rotating, specified in radians.
				dAngle = atan2(0.5*m_dFabricThickness, m_XYarnData[iPrevYarny].dSpacing+m_XYarnData[i].dSpacing);
				// Get the yarn width and height for this X yarn
				dWidth = m_XYarnData[i].dWidth;
				dHeight = m_XYarnData[i].dHeight;
				// Get a pointer to the current yarn
				pYarn = &m_Yarns[m_XYarns[i][0]];
				// Used for getting the cell coordinates
				y = i;
			}
			/*
			CSectionEllipse DefaultEllipseSection(dWidth, dHeight);

			// Get a copy of the yarn sections that is applied to the nodes
			if (pYarn->GetYarnSection()->GetType() != "CYarnSectionInterpNode") {
				return false;
			}
			CYarnSectionInterpNode* pYarnSection = (CYarnSectionInterpNode*)pYarn->GetYarnSection()->Copy();

			for (int j = 0; j < iYarnLength; j++) {
				if (iDirection==0) {
					// Set the parameters which will be used by GetCell for traversing an X yarn
					y = j;
					iPrevYarny = j;
					iNextYarny = j;
				}
				else {
					// Set the parameters which will be used by GetCell for traversing a Y yarn
					x = j;
					iPrevYarnx = j;
					iNextYarnx = j;
				}
				// If the yarns on either side are the same then no rotation should occur
				
				int iRot;		// Should have 1 of three values (-1, 0 or 1). -1 (rotation to the right), 0 (no rotation), 1 (rotation to the left)
				if (GetCell(iPrevYarnx, iPrevYarny) == GetCell(iNextYarnx, iNextYarny)) {
					iRot = 0;
				}
				else if (GetCell(iPrevYarnx, iPrevYarny)[0] == PATTERN_YYARN) {
					// Rotate to the right
					iRot = -1;
				}
				else {
					// Rotate to the left
					iRot = 1;
				}
				
				CSectionEllipse* EllipseSection = NULL;
				if (pYarnSection->GetNodeSection(j).GetType() == "CSectionEllipse") {
					EllipseSection = (CSectionEllipse*)pYarnSection->GetNodeSection(j).Copy();
				}
				else {
					EllipseSection = (CSectionEllipse*)DefaultEllipseSection.Copy();
				}
				// Assign section based on the rotation it should have
				switch (iRot) {
				case 0:
//					pYarnSection->ReplaceSection(j, EllipseSection);
					break;
				case -1:
					pYarnSection->ReplaceSection(j, CSectionRotated(*EllipseSection, -dAngle));
					break;
				case 1:
					pYarnSection->ReplaceSection(j, CSectionRotated(*EllipseSection, dAngle));
					break;
				}
				delete EllipseSection;
				
			}
			// Assign the same section to the end as at the start (periodic yarns)
			if (bPeriodic) {
				pYarnSection->ReplaceSection(j, pYarnSection->GetNodeSection(0));
			}
			pYarn->AssignSection(*pYarnSection);
			delete pYarnSection;
			pYarn->AssignSection(*pYarnSection);
			//*/

			//CYarnSectionInterpNode* yarnsection = (CYarnSectionInterpNode*)pYarn->GetYarnSection()->Copy();
			CYarnSectionInterpNode yarnsection = CYarnSectionInterpNode(0, 1, 1);
			for (int j = 0; j <= iYarnLength; j++) {
				//CSectionEllipse section(dWidth, dHeight);
				CSectionPowerEllipse section = CSectionPowerEllipse(dWidth, dHeight, 0.67, 0);
				yarnsection.AddSection(section);
				//yarnsection->InsertSection(j, 0.5, section);
			}
			pYarn->AssignSection(yarnsection);
		}
	}
	return true;
}

void CTextileWeave2D::AdjustTangents() const {
	for (int i = 0; i < m_iNumXYarns; i++) {
		for (vector<int>::iterator itpYarn = m_XYarns[i].begin(); itpYarn != m_XYarns[i].end(); ++itpYarn) {
			vector<CNode> MasterNodes = m_Yarns[*itpYarn].GetMasterNodes();
			for (vector<CNode>::iterator itNode = MasterNodes.begin(); itNode != MasterNodes.end(); ++itNode) {
				itNode->SetTangent(XYZ(1, 0, 0));
			}
		}
	}
	for (int i = 0; i < m_iNumYYarns; i++) {
		for (vector<int>::iterator itpYarn = m_YYarns[i].begin(); itpYarn != m_YYarns[i].end(); ++itpYarn) {
			vector<CNode> MasterNodes = m_Yarns[*itpYarn].GetMasterNodes();
			for (vector<CNode>::iterator itNode = MasterNodes.begin(); itNode != MasterNodes.end(); ++itNode) {
				itNode->SetTangent(XYZ(0, 1, 0));
			}
		}
	}
}

bool CTextileWeave2D::BuildTextile() const {
	if (!CTextileWeave::BuildTextile()) {
		return false;
	}
	if (!getRefine() && !getInPlaneTangents()) {
		return true;
	}
	// Set interpolation to force in-plane tangents at node if m_bInPlaneTangents set
	if (getInPlaneTangents()) {
		SetInPlaneTangents();
	}
	AdjustTangents();
	if (!getRefine()) {
		return true;
	}
	TGLOGINDENT("Refining textile 2d weave \"" << GetName() << "\"");
	Refine();
	return true;
}

void CTextileWeave2D::Refine( bool bCorrectWidths, bool bCorrectInterference, bool bPeriodic ) const {
	CTimer timer;
	timer.start("Timing Refine");
	if ( bCorrectWidths ) {
		CorrectYarnWidths();
	}
	if (bCorrectInterference) {
		CorrectInterference();
	}
	AdjustSectionsForRotation( bPeriodic );
	if (bCorrectInterference) {
		CorrectInterference();
	}//*/
	timer.check("End of Refine");
	timer.stop();
}

void CTextileWeave2D::RefineTextile(bool bCorrectWidths, bool bCorrectInterference, bool bPeriodic) {
	//BuildTextileIfNeeded();
	if (GetType() != "CTextileWeave2D") {
		return;
	}
	Refine( bCorrectWidths, bCorrectInterference, bPeriodic );
	//m_bNeedsBuilding = true;
}

string CTextileWeave2D::GetDefaultName() const {
	 return "2DWeave(W:" + stringify(m_iNumYYarns) + ",H:" + stringify(m_iNumXYarns) + ")";
}

/// Function to set interpolation so that in-plane tangents are forced at master nodes
void CTextileWeave2D::SetInPlaneTangents() const {
	for (int i = 0; i < m_iNumXYarns; i++) {
		for (vector<int>::iterator itpYarn = m_XYarns[i].begin(); itpYarn != m_XYarns[i].end(); ++itpYarn) {
			SetInPlaneTangents(*itpYarn);
		}
	}
	for (int i = 0; i < m_iNumYYarns; i++) {
		for (vector<int>::iterator itpYarn = m_YYarns[i].begin(); itpYarn != m_YYarns[i].end(); ++itpYarn) {
			SetInPlaneTangents(*itpYarn);
		}
	}
}

void CTextileWeave2D::SetInPlaneTangents(int Yarn) const {
	const CInterpolation* Interpolation = m_Yarns[Yarn].GetInterpolation();
	if (Interpolation->GetType() == "CInterpolationCubic") {
		m_Yarns[Yarn].AssignInterpolation(CInterpolationCubic(true, false, true));
	}
	else {
		m_Yarns[Yarn].AssignInterpolation(CInterpolationBezier(true, false, true));
	}
}

CDomainPlanes CTextileWeave2D::GetDefaultDomain(bool bSheared, bool bAddedDomainHeight) {
	return CTextileWeave::GetDefaultDomain(bSheared, bAddedDomainHeight);
}

void CTextileWeave2D::ConvertToPatternDraft() {
	m_PatternDraft.ClearWeavePattern();
	for (int i = m_iNumYYarns - 1; i >= 0; i--) {
		string Row;
		for (int j = m_iNumXYarns - 1; j >= 0 ; j--) {
			if (GetCell(i, j)[1] == PATTERN_XYARN) {
				Row.push_back('1');
			}
			else {
				Row.push_back('0');
			}
		}
		m_PatternDraft.AddRow(Row);
	}
}
