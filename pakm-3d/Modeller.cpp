#include "Modeller.h"
#include "TexGenRenderer.h"
#include "TexGenMainFrame.h"
#include "TexGenApp.h"
#include "SectionSelect.h"
#include "YarnSectionSelect.h"
#include "LoggerGUI.h"
#include "CustomWidget.h"
#include "vtkCellPicker.h"

BEGIN_EVENT_TABLE(COutlineHandler, wxEvtHandler)
	EVT_TREE_SEL_CHANGED(XRCID("TreeCtrl"), COutlineHandler::OnSelectionChanged)
	EVT_BUTTON(XRCID("DeleteSelected"), COutlineHandler::OnDeleteSelected)
	EVT_BUTTON(XRCID("InsertNode"), COutlineHandler::OnInsertNode)
	EVT_BUTTON(XRCID("DuplicateYarn"), COutlineHandler::OnDuplicateYarn)
END_EVENT_TABLE()


CModeller::CModeller(CTexGenRenderer *pRenderer, string TextileName)
: m_pRenderer(pRenderer)
, m_TextileName(TextileName)
, m_ActiveTool(TOOL_MOVE)
, m_pCustomWidget(NULL)
, m_pCallBack(NULL)
, m_pCellPicker(NULL)
, m_pPropPicker(NULL)
, m_bSelectNodes(true)
, m_bSelectPaths(true)
, m_bSelectSurfaces(true)
, m_bSelectImages(true)
{
	CreateWidgets();
	CreatePicker();
}

CModeller::~CModeller(void)
{
	while (!m_SelectedObjects.empty())
	{
		delete m_SelectedObjects.back();
		m_SelectedObjects.pop_back();
	}
	if (m_pCallBack)
		m_pCallBack->Delete();
	if (m_pCustomWidget)
		m_pCustomWidget->Delete();
	if (m_pCellPicker)
		m_pCellPicker->Delete();
	if (m_pPropPicker)
		m_pPropPicker->Delete();
}

void CModeller::CreateWidgets()
{
	m_pCallBack = new vtkMyCallback(this);

	m_pCustomWidget = vtkCustomWidget::New();
	m_pCustomWidget->PlaceWidget(0, 10, 0, 10, 0, 5);
	m_pCustomWidget->SetInteractor(m_pRenderer->GetInteractor());
	m_pCustomWidget->AddObserver(vtkCommand::StartInteractionEvent, m_pCallBack);
	m_pCustomWidget->AddObserver(vtkCommand::InteractionEvent, m_pCallBack);
	m_pCustomWidget->AddObserver(vtkCommand::EndInteractionEvent, m_pCallBack);
}

void CModeller::CreateTestTextile()
{
	CTextile* pTextile = TEXGEN.GetTextile(m_TextileName);
	if (!pTextile) {
		return;
	}
	CYarn Yarn;
	Yarn.AddNode(CNode(XYZ(0,0,0)));
	Yarn.AddNode(CNode(XYZ(5,0,0)));
	Yarn.AddNode(CNode(XYZ(10,0,0)));
	pTextile->AddYarn(Yarn);
	Yarn.Translate(XYZ(0, 10, 0));
	pTextile->AddYarn(Yarn);
	Yarn.Rotate(WXYZ(XYZ(0, 0, 1), PI/2));
	Yarn.Translate(XYZ(10, 0, 2));
	pTextile->AddYarn(Yarn);
	Yarn.Translate(XYZ(10, 0, 0));
	pTextile->AddYarn(Yarn);
	SetDefaultRenderState();
}

void CModeller::SetDefaultRenderState()
{
	m_pRenderer->SetXRay(true);
	m_pRenderer->RemoveTextiles();
	m_pRenderer->RemoveNodes();
	m_pRenderer->RemovePaths();
	m_pRenderer->RenderTextile(m_TextileName);
	m_pRenderer->RenderNodes(m_TextileName);
	m_pRenderer->RenderPaths(m_TextileName);
}

void CModeller::OnButtonDown(wxMouseEvent &event)
{
	bool bPicked = false;
	if (event.GetButton() == wxMOUSE_BTN_LEFT && !event.ControlDown()/* && m_ActiveTool == TOOL_SELECT*/)
	{
		vtkRenderWindowInteractor* rwi = m_pRenderer->GetInteractor();
		int *eventPos = rwi->GetEventPosition();
		if (!HitWidget(eventPos[0], eventPos[1]))
		{
			if (!event.ShiftDown())
				DeselectAll();
			if (!bPicked && m_bSelectNodes)
				bPicked = SelectNode(eventPos[0], eventPos[1]);
			if (!bPicked && m_bSelectPaths)
				bPicked = SelectPath(eventPos[0], eventPos[1]);
			if (!bPicked && m_bSelectSurfaces)
				bPicked = SelectSurface(eventPos[0], eventPos[1]);
			if (!bPicked && m_bSelectImages)
				bPicked = SelectImage(eventPos[0], eventPos[1]);
		}
	}
	if (bPicked)
	{
		HighlightSelectedObjects();
		m_pRenderer->RefreshView();
	}
	else
	{
		event.Skip();
	}
}

void CModeller::OnKeyDown(wxKeyEvent &event)
{
	switch (event.GetKeyCode())
	{
	case WXK_DELETE:
		DeleteSelectedObjects();
		break;
	case WXK_INSERT:
		InsertNodes();
		break;
	case 'D':
		DuplicateYarns();
		break;
	default:
		return;
	}
	event.Skip();
}

void CModeller::CreatePicker()
{
	m_pCellPicker = vtkCellPicker::New();
	m_pCellPicker->SetTolerance(0.001);
	m_pCellPicker->PickFromListOn();
	m_pPropPicker = vtkPropPicker::New();
	m_pPropPicker->PickFromListOn();
}

void CModeller::SetPickFilter(const vector<vtkProp*> &Props)
{
	m_pCellPicker->InitializePickList();
	m_pPropPicker->InitializePickList();
	vector<vtkProp*>::const_iterator itProp;
	for (itProp = Props.begin(); itProp != Props.end(); ++itProp)
	{
		m_pCellPicker->AddPickList(*itProp);
		m_pPropPicker->AddPickList(*itProp);
	}
}

bool CModeller::HitWidget(double dX, double dY)
{
	if (!m_pCustomWidget->GetEnabled())
		return false;
	SetPickFilter(m_pCustomWidget->GetProps());
	m_pCellPicker->Pick(dX, dY, 0.0, m_pRenderer->GetRenderer());
	return (m_pCellPicker->GetViewProp() != NULL);
}

bool CModeller::SelectNode(double dX, double dY)
{
	SetPickFilter(m_pRenderer->GetProps(CTexGenRenderer::PROP_NODE));

	m_pCellPicker->Pick(dX, dY, 0.0, m_pRenderer->GetRenderer());
	vtkActor* pActor = m_pCellPicker->GetActor();
	if (pActor)
	{
		const PROP_NODE_INFO* pNodeInfo = m_pRenderer->GetNodePropInfo(pActor);
		if (pNodeInfo)
		{
			SelectObject(new PROP_NODE_INFO(*pNodeInfo));
			return true;
		}
	}
	return false;
}

bool CModeller::SelectPath(double dX, double dY)
{
	SetPickFilter(m_pRenderer->GetProps(CTexGenRenderer::PROP_PATH));

	m_pCellPicker->Pick(dX, dY, 0.0, m_pRenderer->GetRenderer());
	vtkActor* pActor = m_pCellPicker->GetActor();
	if (pActor)
	{
		const PROP_YARN_INFO* pYarnInfo = m_pRenderer->GetYarnPropInfo(pActor);
		if (pYarnInfo)
		{
			SelectObject(new PROP_YARN_INFO(*pYarnInfo));
			return true;
		}
	}
	return false;
}

bool CModeller::SelectSurface(double dX, double dY)
{
	SetPickFilter(m_pRenderer->GetProps(CTexGenRenderer::PROP_SURFACE));

	m_pCellPicker->Pick(dX, dY, 0.0, m_pRenderer->GetRenderer());
	vtkActor* pActor = m_pCellPicker->GetActor();
	if (pActor)
	{
		const PROP_YARN_INFO* pYarnInfo = m_pRenderer->GetYarnPropInfo(pActor);
		if (pYarnInfo)
		{
			SelectObject(new PROP_YARN_INFO(*pYarnInfo));
			return true;
		}
	}
	return false;
}

bool CModeller::SelectImage(double dX, double dY)
{
	SetPickFilter(m_pRenderer->GetProps(CTexGenRenderer::PROP_IMAGE));

	m_pPropPicker->Pick(dX, dY, 0.0, m_pRenderer->GetRenderer());
	vtkProp* pProp = m_pPropPicker->GetViewProp();
	if (pProp)
	{
		const PROP_IMAGE_INFO* pImageInfo = m_pRenderer->GetImagePropInfo(pProp);
		if (pImageInfo)
		{
			SelectObject(new PROP_IMAGE_INFO(*pImageInfo));
			return true;
		}
	}
	return false;
}

void CModeller::OnButtonUp(wxMouseEvent &event)
{
	event.Skip();
}

void CModeller::DeselectAll(bool bUpdateOutliner)
{
	while (!m_SelectedObjects.empty())
		DeselectObject(m_SelectedObjects.front(), false, false);
	UpdateWidget();
	if (bUpdateOutliner)
		UpdateOutlinerSelection();
}

void CModeller::DeselectObject(PROP_INFO* pObject, bool bUpdateWidget, bool bUpdateOutliner)
{
	vector<PROP_INFO*>::iterator itObject = find(m_SelectedObjects.begin(), m_SelectedObjects.end(), pObject);
	if (itObject != m_SelectedObjects.end())
	{
		vector<vtkProp*> Props = m_pRenderer->GetProps(pObject);
		vector<vtkProp*>::iterator itProp;

		vtkActor* pActor;
		for (itProp = Props.begin(); itProp != Props.end(); ++itProp)
		{
			pActor = dynamic_cast<vtkActor*>(*itProp);
			if (pActor) {
				m_pRenderer->ApplyDefaultColor(pActor);
			}
		}
		delete *itObject;
		m_SelectedObjects.erase(itObject);
	}
	if (bUpdateWidget)
		UpdateWidget();
	if (bUpdateOutliner)
		UpdateOutlinerSelection();
}

void CModeller::SelectObject(PROP_INFO* pObject, bool bUpdateOutliner)
{
	// TODO: Should really check if the object is already selected or not....
	m_SelectedObjects.push_back(pObject);
	if (bUpdateOutliner)
	{
		HighlightSelectedObjects();
		UpdateWidget();
		UpdateOutlinerSelection();
	}
}

void CModeller::HighlightSelectedObjects()
{
	vector<PROP_INFO*>::iterator itObject;
	vector<vtkProp*> Props;
	vector<vtkProp*>::iterator itProp;
	vtkActor* pActor;
	for (itObject = m_SelectedObjects.begin(); itObject != m_SelectedObjects.end(); ++itObject)
	{
		Props = m_pRenderer->GetProps(*itObject);
		for (itProp = Props.begin(); itProp != Props.end(); ++itProp)
		{
			pActor = dynamic_cast<vtkActor*>(*itProp);
			if (pActor)
			{
				if (*itObject == m_SelectedObjects.back())
				{
					CTexGenRenderer::ApplyColor(pActor, COLOR(1, 1, 1));
				}
				else
				{
					CTexGenRenderer::ApplyColor(pActor, COLOR(0.7, 0.7, 0.7));
					pActor->GetProperty()->SetSpecular(0);
					pActor->GetProperty()->SetSpecularPower(0);
				}
			}
		}
	}
}

void CModeller::UpdateWidget()
{
	vtkProp3D* pProp3D = NULL;
	vector<vtkProp*> Props;
	if (!m_SelectedObjects.empty())
	{
		Props = m_pRenderer->GetProps(m_SelectedObjects.back());
	}

	if (!Props.empty())
	{
		if (m_ActiveTool == TOOL_MOVE)
		{
			pProp3D = dynamic_cast<vtkProp3D*>(Props.front());
			if (pProp3D)
				m_pCustomWidget->SetPosition(pProp3D->GetCenter());
			m_PrevWidgetPos = m_pCustomWidget->GetPosition();

			CTextile* pTextile = TEXGEN.GetTextile(m_TextileName);
			if (pTextile)
				m_pCustomWidget->SetScale(0.2*pTextile->GetApproximateSize());
			m_pCustomWidget->On();
		}
		else
		{
			m_pCustomWidget->Off();
		}
	}
	else
	{
		m_pCustomWidget->Off();
	}
	CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
	pMainFrame->UpdatePositionText();
	m_pRenderer->RefreshView();
}

void CModeller::OnWidgetMove()
{
	// Disable log messages while interactively modifying geometry
	TEXGEN.SetLogger(CLoggerNull());

	XYZ WidgetPos = m_pCustomWidget->GetPosition();
	XYZ DeltaPos = WidgetPos-m_PrevWidgetPos;
	MoveSelectedObjects(DeltaPos);
	m_PrevWidgetPos = WidgetPos;

	CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
	pMainFrame->UpdatePositionText();
	// Re-enable log messages
	if ( TEXGEN.GetMessagesOn() )
		TEXGEN.SetLogger(CLoggerGUI());
}

void CModeller::MoveSelectedObjects(XYZ DeltaPos)
{
	vector<PROP_INFO*>::iterator itObject;
	vector<vtkProp*> Props;
	vector<vtkProp*>::iterator itProp;
	XYZ Pos;
	for (itObject = m_SelectedObjects.begin(); itObject != m_SelectedObjects.end(); ++itObject)
	{
		// Move the props
		Props = m_pRenderer->GetProps(*itObject);
		for (itProp = Props.begin(); itProp != Props.end(); ++itProp)
		{
			vtkProp3D* pProp3D = dynamic_cast<vtkProp3D*>(*itProp);
			if (pProp3D)
			{
				Pos = pProp3D->GetPosition();
				Pos += DeltaPos;
				pProp3D->SetPosition(Pos.x, Pos.y, Pos.z);
			}
		}
		// Move the TexGen objects they refer to
		PROP_YARN_INFO* pYarnInfo = dynamic_cast<PROP_YARN_INFO*>(*itObject);
		PROP_NODE_INFO* pNodeInfo = dynamic_cast<PROP_NODE_INFO*>(*itObject);
		PROP_IMAGE_INFO* pImageInfo = dynamic_cast<PROP_IMAGE_INFO*>(*itObject);
		if (pYarnInfo)
		{
			CTextile* pTextile = TEXGEN.GetTextile(pYarnInfo->TextileName);
			if (pTextile)
			{
				assert(pYarnInfo->iYarn >= 0 && pYarnInfo->iYarn < (int)pTextile->GetYarns().size());
				CYarn &Yarn = pTextile->GetYarns()[pYarnInfo->iYarn];

				const vector<CNode> &Nodes = Yarn.GetMasterNodes();
				vector<CNode>::const_iterator itNode;
				int i;
				for (itNode = Nodes.begin(), i=0; itNode != Nodes.end(); ++itNode, ++i)
				{
					CNode Node = *itNode;
					Node.SetPosition(Node.GetPosition() + DeltaPos);
					Yarn.ReplaceNode(i, Node);
				}

//				RefreshYarns.insert(*pYarnInfo);
			}
		}
		if (pNodeInfo)
		{
			CTextile* pTextile = TEXGEN.GetTextile(pNodeInfo->TextileName);
			if (pTextile)
			{
				assert(pNodeInfo->iYarn >= 0 && pNodeInfo->iYarn < (int)pTextile->GetYarns().size());
				CYarn &Yarn = pTextile->GetYarns()[pNodeInfo->iYarn];
				assert(pNodeInfo->iNode >= 0 && pNodeInfo->iNode < (int)Yarn.GetMasterNodes().size());
				CNode Node = Yarn.GetMasterNodes()[pNodeInfo->iNode];
				Node.SetPosition(Node.GetPosition() + DeltaPos);
				Yarn.ReplaceNode(pNodeInfo->iNode, Node);

/*				PROP_YARN_INFO YarnInfo;
				YarnInfo.TextileName = pNodeInfo->TextileName;
				YarnInfo.iYarn = pNodeInfo->iYarn;
				RefreshYarns.insert(YarnInfo);*/
			}
		}
		if (pImageInfo)
		{

		}
	}
	RefreshSelectedYarns();
}

void CModeller::ScaleSelectedObjects(XYZ DeltaScale)
{
	vector<PROP_INFO*>::iterator itObject;
	vector<vtkProp*> Props;
	vector<vtkProp*>::iterator itProp;
	XYZ Scale;
	for (itObject = m_SelectedObjects.begin(); itObject != m_SelectedObjects.end(); ++itObject)
	{
		// Scale the props
		Props = m_pRenderer->GetProps(*itObject);
		for (itProp = Props.begin(); itProp != Props.end(); ++itProp)
		{
			vtkProp3D* pProp3D = dynamic_cast<vtkProp3D*>(*itProp);
			if (pProp3D)
			{
				Scale = pProp3D->GetScale();
				Scale *= DeltaScale;
				pProp3D->SetScale(Scale.x, Scale.y, Scale.z);
			}
		}
	}
}

void CModeller::SetActiveTool(TOOLS Tool)
{
	m_ActiveTool = Tool;
	UpdateWidget();
}

vector<PROP_YARN_INFO> CModeller::GetSelectedYarns() const
{
	vector<PROP_INFO*>::const_iterator itObject;
	vector<PROP_YARN_INFO> SelectedYarns;
	for (itObject = m_SelectedObjects.begin(); itObject != m_SelectedObjects.end(); ++itObject)
	{
		PROP_YARN_INFO* pYarnInfo = dynamic_cast<PROP_YARN_INFO*>(*itObject);
		if (pYarnInfo)
		{
			SelectedYarns.push_back(*pYarnInfo);
		}
	}
	return SelectedYarns;
}

vector<PROP_NODE_INFO> CModeller::GetSelectedNodes() const
{
	vector<PROP_INFO*>::const_iterator itObject;
	vector<PROP_NODE_INFO> SelectedNodes;
	for (itObject = m_SelectedObjects.begin(); itObject != m_SelectedObjects.end(); ++itObject)
	{
		PROP_NODE_INFO* pNodeInfo = dynamic_cast<PROP_NODE_INFO*>(*itObject);
		if (pNodeInfo)
		{
			SelectedNodes.push_back(*pNodeInfo);
		}
	}
	return SelectedNodes;
}

CYarn *CModeller::GetYarn(PROP_YARN_INFO YarnInfo) const
{
	CTextile* pTextile = TEXGEN.GetTextile(YarnInfo.TextileName);
	if (pTextile)
	{
		if (YarnInfo.iYarn >= 0 && YarnInfo.iYarn < (int)pTextile->GetYarns().size())
			return &pTextile->GetYarns()[YarnInfo.iYarn];
	}
	return NULL;
}

const CNode *CModeller::GetNode(PROP_NODE_INFO NodeInfo) const
{
	CTextile* pTextile = TEXGEN.GetTextile(NodeInfo.TextileName);
	if (!pTextile)
		return NULL;
	if (NodeInfo.iYarn < 0 || NodeInfo.iYarn >= (int)pTextile->GetYarns().size())
		return NULL;
	CYarn &Yarn = pTextile->GetYarns()[NodeInfo.iYarn];
	if (NodeInfo.iNode < 0 || NodeInfo.iNode >= (int)Yarn.GetMasterNodes().size())
		return NULL;
	return &Yarn.GetMasterNodes()[NodeInfo.iNode];
}

void CModeller::OnDeleteSelectedNodes()
{
	vector<PROP_NODE_INFO> SelectedNodes = GetSelectedNodes();
	// Make sure nodes are sorted in descending order...
	// This ensures that selected node information is valid at the time of deletion
	sort(SelectedNodes.rbegin(), SelectedNodes.rend());
	for (auto itNode = SelectedNodes.begin(); itNode != SelectedNodes.end(); ++itNode)
	{
		CTexGen::Instance().GetTextile(m_TextileName)->GetYarn(itNode->iYarn)->DeleteNode(itNode->iNode);
	}
}

void CModeller::OnDeleteSelectedYarns()
{
	vector<PROP_YARN_INFO> SelectedYarns = GetSelectedYarns();
	// Make sure yarns are sorted in descending order...
	// This ensures that selected yarn information is valid at the time of deletion
	sort(SelectedYarns.rbegin(), SelectedYarns.rend());
	for (auto itYarn = SelectedYarns.begin(); itYarn != SelectedYarns.end(); itYarn++)
	{
		CTexGen::Instance().GetTextile(m_TextileName)->DeleteYarn(itYarn->iYarn);
	}
}

void CModeller::DeleteSelectedObjects()
{
	OnDeleteSelectedNodes();
	OnDeleteSelectedYarns();
	DeselectAll();
	DeleteInvalidYarns();
	m_pRenderer->RefreshTextile(m_TextileName);
	UpdateOutlinerItems();
}

void CModeller::DeleteInvalidYarns()
{
	CTextile* pTextile = TEXGEN.GetTextile(m_TextileName);
	if (pTextile)
	{
		vector<CYarn> &Yarns = pTextile->GetYarns();
		int i;
		for (i=(int)Yarns.size()-1; i>=0; --i)
		{
			if (Yarns[i].GetMasterNodes().size() < 2)
			{
				pTextile->DeleteYarn(i);
			}
		}
	}
}

void CModeller::InsertNodes()
{
	vector<PROP_NODE_INFO> SelectedNodes = GetSelectedNodes();

	// Make sure nodes are sorted in descending order...
	// This ensures that selected node information is valid at the time of insertion
	sort(SelectedNodes.rbegin(), SelectedNodes.rend());

	for (auto itNodeInfo = SelectedNodes.begin(); itNodeInfo != SelectedNodes.end(); ++itNodeInfo)
	{
		CTextile* pTextile = TEXGEN.GetTextile(itNodeInfo->TextileName);
		if (pTextile)
		{
			assert(itNodeInfo->iYarn >= 0 && itNodeInfo->iYarn < (int)pTextile->GetYarns().size());
			CYarn &Yarn = pTextile->GetYarns()[itNodeInfo->iYarn];

			const vector<CNode> &Nodes = Yarn.GetMasterNodes();
			assert(itNodeInfo->iNode >= 0 && itNodeInfo->iNode < (int)Nodes.size());
			if (itNodeInfo->iNode > 0)
			{
	/*			vector<CNode>::const_iterator itNode;
				int i;
				for (itNode = Nodes.begin(), i=0; itNode != Nodes.end(); ++itNode, ++i)
				{
					CNode Node = *itNode;
					Node.SetPosition(Node.GetPosition() + DeltaPos);
					Yarn.ReplaceNode(i, Node);
				}*/
				const CNode &PrevNode = Nodes[itNodeInfo->iNode-1];
				const CNode &NextNode = Nodes[itNodeInfo->iNode];

				XYZ NewNodePos = 0.5*(NextNode.GetPosition() + PrevNode.GetPosition());

				auto yarn = CTexGen::Instance().GetTextile(m_TextileName)->GetYarn(itNodeInfo->iYarn);
				auto node = CNode(NewNodePos);
				yarn->InsertNode(node, itNodeInfo->iNode);
			}
		}
	}
	RefreshSelectedYarns();
	UpdateWidget();
	UpdateOutlinerItems();
}

void CModeller::DuplicateYarns()
{
	vector<PROP_YARN_INFO> SelectedYarns = GetSelectedYarns();

	CTextile* pTextile = TEXGEN.GetTextile(m_TextileName);
	if (pTextile)
	{
		int iFirstNewYarn = (int)pTextile->GetYarns().size();
		int iLastNewYarn = iFirstNewYarn;
		auto textile = CTexGen::Instance().GetTextile(m_TextileName);
		for (auto itYarn = SelectedYarns.begin(); itYarn != SelectedYarns.end(); itYarn++)
		{
			auto newyarn = CYarn(*textile->GetYarn(itYarn->iYarn));
			textile->AddYarn(newyarn);
			++iLastNewYarn;
		}
		DeselectAll();

		m_pRenderer->RefreshTextile(m_TextileName);

		PROP_YARN_INFO YarnInfo;
		YarnInfo.TextileName = m_TextileName;
		for (int i = iFirstNewYarn; i < iLastNewYarn; i++)
		{
			YarnInfo.iYarn = i;
			SelectObject(new PROP_YARN_INFO(YarnInfo));
		}
		UpdateOutlinerItems();
	}
}

void CModeller::AssignSectionToSelectedObjects()
{
	vector<PROP_YARN_INFO> SelectedYarns = GetSelectedYarns();
	if (SelectedYarns.empty())
	{
		wxMessageBox(wxT("Please select at least one yarn before trying to assign a section."), wxT("Unable to assign section"), wxOK | wxICON_INFORMATION);
		return;
	}
	bool bSameNumNodes = true;
	int iNumNodes = 0;
	for (auto itYarn = SelectedYarns.begin(); itYarn != SelectedYarns.end(); itYarn++)
	{
		CYarn *pYarn = GetYarn(*itYarn);
		if (itYarn == SelectedYarns.begin())
		{
			iNumNodes = (int)pYarn->GetMasterNodes().size();
		}
		else if (iNumNodes != pYarn->GetMasterNodes().size())
		{
			bSameNumNodes = false;
			break;
		}
	}
	CYarn *pYarn = GetYarn(SelectedYarns.back());
	const CYarnSection* pSection = nullptr;
	if (pYarn)
	{
		pSection = pYarn->GetYarnSection();
	}
	CYarnSectionSelect Dialog;
	if (bSameNumNodes)
		Dialog.SetNumberOfNodes(iNumNodes);
	if (pSection)
		Dialog.LoadSettings(*pSection);
	if (Dialog.ShowModal() == wxID_OK)
	{
		string Command = Dialog.GetCreateSectionCommand();
		if (!Command.empty())
		{
			stringstream StringStream;
			StringStream << "textile = GetTextile('" << m_TextileName << "')" << endl;
			for (auto itYarn = SelectedYarns.begin(); itYarn != SelectedYarns.end(); itYarn++)
			{
				StringStream << "textile.GetYarn(" << itYarn->iYarn << ").AssignSection(yarnsection)" << endl;
			}
			Command += StringStream.str();

			CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
			pMainFrame->SendPythonCode(Command);

			RefreshSelectedYarns();
		}
	}
}

void CModeller::AssignInterpolationToSelectedObjects()
{
	vector<PROP_YARN_INFO> SelectedYarns = GetSelectedYarns();
	vector<PROP_YARN_INFO>::iterator itYarn;

	if (SelectedYarns.empty())
	{
		wxMessageBox(wxT("Please select at least one yarn before trying to assign an interpolation."), wxT("Unable to assign interpolation"), wxOK | wxICON_INFORMATION);
		return;
	}

	CYarn *pYarn = GetYarn(SelectedYarns.back());
	const CInterpolation* pInterpolation = nullptr;
	if (pYarn)
	{
		pInterpolation = pYarn->GetInterpolation();
	}

	int iInterpSelection = 0;
	bool bPeriodic = true;

	if (pInterpolation)
	{
		bPeriodic = pInterpolation->GetPeriodic();
		string Type = pInterpolation->GetType();
		if (Type == "CInterpolationBezier")
			iInterpSelection = 0;
		else if (Type == "CInterpolationCubic")
			iInterpSelection = 1;
		else if (Type == "CInterpolationLinear")
			iInterpSelection = 2;
	}

	wxDialog Dialog;
	if (wxXmlResource::Get()->LoadDialog(&Dialog, ((CTexGenApp*)wxTheApp)->GetMainFrame(), wxT("Interpolation")))
	{
		XRCCTRL(Dialog, "InterpChoice", wxChoice)->SetValidator(wxGenericValidator(&iInterpSelection));
		XRCCTRL(Dialog, "PeriodicCheck", wxCheckBox)->SetValidator(wxGenericValidator(&bPeriodic));

		if (Dialog.ShowModal() == wxID_OK)
		{
			CInterpolation* interpolation = nullptr;
			switch (iInterpSelection)
			{
			case 0: interpolation = new CInterpolationBezier(bPeriodic); break;
			case 1: interpolation = new CInterpolationCubic(bPeriodic); break;
			case 2: interpolation = new CInterpolationLinear(bPeriodic); break;
			}
			if (interpolation)
			{
				auto textile = CTexGen::Instance().GetTextile(m_TextileName);
				for (auto itYarn = SelectedYarns.begin(); itYarn != SelectedYarns.end(); itYarn++)
				{
					textile->GetYarn(itYarn->iYarn)->AssignInterpolation(*interpolation);
				}
			}
			RefreshSelectedYarns();
		}
	}
}

void CModeller::AssignRepeatsToSelectedObjects()
{
	vector<PROP_YARN_INFO> SelectedYarns = GetSelectedYarns();
	if (SelectedYarns.empty())
	{
		wxMessageBox(wxT("Please select at least one yarn before trying to assign repeats."), wxT("Unable to assign repeats"), wxOK | wxICON_INFORMATION);
		return;
	}

	CYarn *pYarn = GetYarn(SelectedYarns.back());
	const vector<XYZ> &Repeats = pYarn->GetRepeats();
	
	wxDialog Dialog;
	if (wxXmlResource::Get()->LoadDialog(&Dialog, ((CTexGenApp*)wxTheApp)->GetMainFrame(), wxT("Repeats")))
	{
		wxGrid* pGrid = XRCCTRL(Dialog, "RepeatsGrid", wxGrid);
		int iNumRepeats = max((int)Repeats.size(), 3);
		pGrid->CreateGrid(iNumRepeats, 3);
//		wxGridTableBase* pTable = pGrid->GetTable();
		for (size_t i = 0; i < Repeats.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				wxString Val;
				Val << Repeats[i][j];
				pGrid->SetCellValue(i, j, Val);
			}
		}


		pGrid->SetGridLineColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW));
		pGrid->SetDefaultCellBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));

		pGrid->SetColLabelSize(20);
		pGrid->SetColLabelValue(0, wxT("X"));
		pGrid->SetColLabelValue(1, wxT("Y"));
		pGrid->SetColLabelValue(2, wxT("Z"));
		pGrid->SetColFormatFloat(0);
		pGrid->SetColFormatFloat(1);
		pGrid->SetColFormatFloat(2);
		pGrid->EnableDragColSize(false);
		pGrid->EnableDragGridSize(false);
		pGrid->EnableDragRowSize(false);
		Dialog.SetInitialSize(wxSize(400, 180));
		if (Dialog.ShowModal() == wxID_OK)
		{
			vector<XYZ> RepeatVectors;
			pGrid->SaveEditControlValue();
			for (int i = 0; i < pGrid->GetNumberRows(); i++)
			{
				XYZ RepeatVector;
				wxString X = pGrid->GetCellValue(i, 0);
				wxString Y = pGrid->GetCellValue(i, 1);
				wxString Z = pGrid->GetCellValue(i, 2);
				X.ToDouble(&RepeatVector.x);
				Y.ToDouble(&RepeatVector.y);
				Z.ToDouble(&RepeatVector.z);
				if (RepeatVector)
				{
					RepeatVectors.push_back(RepeatVector);
				}
				else
				{
					break;
				}
			}
			auto textile = CTexGen::Instance().GetTextile(m_TextileName);
			vector<XYZ> repeats;
			for (size_t i = 0; i < RepeatVectors.size(); i++)
			{
				repeats.push_back(XYZ(RepeatVectors[i]));
			}
			for (auto itYarn = SelectedYarns.begin(); itYarn != SelectedYarns.end(); itYarn++)
			{
				textile->GetYarn(itYarn->iYarn)->SetRepeats(repeats);
			}			
			RefreshSelectedYarns();
/*			double x = pTable->GetValueAsDouble(0, 0);
			double y = pTable->GetValueAsDouble(0, 1);
			double z = pTable->GetValueAsDouble(0, 2);*/
		}
	}
}

void CModeller::AssignPropertiesToSelectedObjects()
{
	vector<PROP_YARN_INFO> SelectedYarns = GetSelectedYarns();
	if (SelectedYarns.empty())
	{
//		wxMessageBox(wxT("Please select at least one yarn before trying to assign properties."), wxT("Unable to assign properties"), wxOK | wxICON_INFORMATION);
		AssignPropertiesToTextile();
		return;
	}
	auto yarn = GetYarn(SelectedYarns.back());
	wxDialog Dialog;
	if (wxXmlResource::Get()->LoadDialog(&Dialog, ((CTexGenApp*)wxTheApp)->GetMainFrame(), wxT("Properties")))
	{
		vector<pair<wxString, CProperty> > Properties;
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_YARN_LINEAR_DENSITY), yarn->m_YarnLinearDensity));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_FIBRE_DENSITY), yarn->m_FibreDensity));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_FIBRE_AREA), yarn->m_FibreArea));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_FIBRE_DIAMETER), yarn->m_FibreDiameter));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_FIBRES_PER_YARN), yarn->FibresPerYarnProperty()));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_YOUNGS_MODULUS_X), yarn->m_YoungsModulusX));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_YOUNGS_MODULUS_Y), yarn->m_YoungsModulusY));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_YOUNGS_MODULUS_Z), yarn->m_YoungsModulusZ));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_SHEAR_MODULUS_XY), yarn->m_ShearModulusXY));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_SHEAR_MODULUS_XZ), yarn->m_ShearModulusXZ));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_SHEAR_MODULUS_YZ), yarn->m_ShearModulusYZ));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_POISSONS_RATIO_X), yarn->PoissonsRatioXProperty()));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_POISSONS_RATIO_Y), yarn->PoissonsRatioYProperty()));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_POISSONS_RATIO_Z), yarn->PoissonsRatioZProperty()));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_ALPHA_X), yarn->m_AlphaX));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_ALPHA_Y), yarn->m_AlphaY));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_ALPHA_Z), yarn->m_AlphaZ));
		wxGrid* pGrid = XRCCTRL(Dialog, "PropertiesGrid", wxGrid);
		CreatePropertiesGrid(pGrid, Properties);
		Dialog.SetInitialSize(wxSize(400, 250));
		if (Dialog.ShowModal() == wxID_OK)
		{
			SavePropertiesFromGrid(pGrid, Properties);
			auto textile = CTexGen::Instance().GetTextile(m_TextileName);
			for (auto itYarn = SelectedYarns.begin(); itYarn != SelectedYarns.end(); itYarn++)
			{
				for (auto itProperty = Properties.begin(); itProperty != Properties.end(); itProperty++)
				{
					if (itProperty->second.IsSet())
					{
						switch (Convert::ToProperty(itProperty->first))
						{
						case CProperties_ID::ID_YARN_LINEAR_DENSITY:
							textile->GetYarn(itYarn->iYarn)->SetYarnLinearDensity(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_FIBRE_DENSITY:
							textile->GetYarn(itYarn->iYarn)->SetFibreDensity(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_FIBRE_AREA:
							textile->GetYarn(itYarn->iYarn)->SetFibreArea(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_FIBRE_DIAMETER:
							textile->GetYarn(itYarn->iYarn)->SetFibreDiameter(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_FIBRES_PER_YARN:
							textile->GetYarn(itYarn->iYarn)->SetFibresPerYarn(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_YOUNGS_MODULUS_X:
							textile->GetYarn(itYarn->iYarn)->SetYoungsModulusX(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_YOUNGS_MODULUS_Y:
							textile->GetYarn(itYarn->iYarn)->SetYoungsModulusY(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_YOUNGS_MODULUS_Z:
							textile->GetYarn(itYarn->iYarn)->SetYoungsModulusZ(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_SHEAR_MODULUS_XY:
							textile->GetYarn(itYarn->iYarn)->SetShearModulusXY(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_SHEAR_MODULUS_XZ:
							textile->GetYarn(itYarn->iYarn)->SetShearModulusXZ(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_SHEAR_MODULUS_YZ:
							textile->GetYarn(itYarn->iYarn)->SetShearModulusYZ(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_POISSONS_RATIO_X:
							textile->GetYarn(itYarn->iYarn)->SetPoissonsRatioX(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_POISSONS_RATIO_Y:
							textile->GetYarn(itYarn->iYarn)->SetPoissonsRatioY(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_POISSONS_RATIO_Z:
							textile->GetYarn(itYarn->iYarn)->SetPoissonsRatioZ(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_ALPHA_X:
							textile->GetYarn(itYarn->iYarn)->SetAlphaX(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_ALPHA_Y:
							textile->GetYarn(itYarn->iYarn)->SetAlphaY(itProperty->second.GetValue());
							break;
						case CProperties_ID::ID_ALPHA_Z:
							textile->GetYarn(itYarn->iYarn)->SetAlphaZ(itProperty->second.GetValue());
							break;
						}
					}
				}
			}
		}
	}
}

void CModeller::DisplayVolumeFractionForSelectedObjects()
{
	vector<PROP_YARN_INFO> SelectedYarns = GetSelectedYarns();
	if (SelectedYarns.empty())
	{
		wxMessageBox(wxT("Please select at least one yarn for display of volume fraction."), wxT("Unable to display volume fraction"), wxOK | wxICON_INFORMATION);
		return;
	}
	wxString VfMessage;
	for (auto itYarn = SelectedYarns.begin(); itYarn != SelectedYarns.end(); ++itYarn )
	{
		CYarn *pYarn = GetYarn( *itYarn );
		double VolumeFraction = pYarn->GetFibreYarnVolumeFraction();
		VfMessage << wxT("Yarn ") << itYarn->iYarn << wxT(" Fibre volume fraction =  ") << VolumeFraction << wxT("\n");
	}
	wxMessageBox( VfMessage, wxT("Volume Fraction")/*, wxOK|wxCENTRE*/ );
}

void CModeller::AssignPropertiesToTextile()
{
	auto textile = CTexGen::Instance().GetTextile(m_TextileName);
	if (!textile)
	{
		return;
	}
	wxDialog Dialog;
	if (wxXmlResource::Get()->LoadDialog(&Dialog, ((CTexGenApp*)wxTheApp)->GetMainFrame(), wxT("Properties")))
	{
		vector<pair<wxString, CProperty>> Properties;
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_YARN_LINEAR_DENSITY), textile->m_YarnLinearDensity));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_FIBRE_DENSITY), textile->m_FibreDensity));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_FIBRE_AREA), textile->m_FibreArea));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_FIBRE_DIAMETER), textile->m_FibreDiameter));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_FIBRES_PER_YARN), textile->FibresPerYarnProperty()));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_ID::ID_AREAL_DENSITY), textile->m_ArealDensity));
		wxGrid* pGrid = XRCCTRL(Dialog, "PropertiesGrid", wxGrid);
		CreatePropertiesGrid(pGrid, Properties);
		Dialog.SetInitialSize(wxSize(400, 180));
		if (Dialog.ShowModal() == wxID_OK)
		{
			SavePropertiesFromGrid(pGrid, Properties);
			for (auto itProperty = Properties.begin(); itProperty != Properties.end(); itProperty++)
			{
				if (itProperty->second.IsSet())
				{
					switch (Convert::ToProperty(itProperty->first))
					{
					case CProperties_ID::ID_YARN_LINEAR_DENSITY:
						textile->SetYarnLinearDensity(itProperty->second.GetValue(), itProperty->second.GetUnits());
						break;
					case CProperties_ID::ID_FIBRE_DENSITY:
						textile->SetFibreDensity(itProperty->second.GetValue(), itProperty->second.GetUnits());
						break;
					case CProperties_ID::ID_FIBRE_AREA:
						textile->SetFibreArea(itProperty->second.GetValue(), itProperty->second.GetUnits());
						break;
					case CProperties_ID::ID_FIBRE_DIAMETER:
						textile->SetFibreDiameter(itProperty->second.GetValue(), itProperty->second.GetUnits());
						break;
					case CProperties_ID::ID_FIBRES_PER_YARN:
						textile->SetFibresPerYarn(itProperty->second.GetValue());
						break;
					case CProperties_ID::ID_AREAL_DENSITY:
						textile->SetArealDensity(itProperty->second.GetValue(), itProperty->second.GetUnits());
						break;
					}
				}
			}
		}
	}
}

void CModeller::CreatePropertiesGrid(wxGrid* pGrid, vector<pair<wxString, CProperty> > &Properties)
{
	int iNumProperties = (int)Properties.size();
	pGrid->CreateGrid(iNumProperties, 2);

	vector<pair<wxString, CProperty> >::iterator itProperty;
	int iRow;
	for (itProperty = Properties.begin(), iRow=0; itProperty != Properties.end(); ++itProperty, ++iRow)
	{
		pGrid->SetRowLabelValue(iRow, itProperty->first);
		if (itProperty->second.IsSet())
		{
			pGrid->SetCellValue(iRow, 0, wxString() << itProperty->second.GetValue());
			pGrid->SetCellValue(iRow, 1, ConvertString(itProperty->second.GetUnits()));
		}
		else
		{
			pGrid->SetCellValue(iRow, 0, wxT("0.0"));
			pGrid->SetCellValue(iRow, 1, ConvertString(itProperty->second.GetSIUnits()));
		}
	}

	pGrid->SetColLabelSize(20);
	pGrid->SetRowLabelSize(150);
	pGrid->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
	pGrid->SetColLabelValue(0, wxT("Value"));
	pGrid->SetColLabelValue(1, wxT("Units"));
	pGrid->SetColFormatFloat(0);

	pGrid->SetGridLineColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW));
	pGrid->SetDefaultCellBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));

	pGrid->EnableDragColSize(false);
	pGrid->EnableDragGridSize(false);
	pGrid->EnableDragRowSize(false);
}

void CModeller::SavePropertiesFromGrid(wxGrid* pGrid, vector<pair<wxString, CProperty> > &Properties)
{
	pGrid->SaveEditControlValue();

	vector<pair<wxString, CProperty> >::iterator itProperty;
	int iRow;
	// TODO: Only set values which have changed
	for (itProperty = Properties.begin(), iRow=0; itProperty != Properties.end(); ++itProperty, ++iRow)
	{
		string Value = ConvertString(pGrid->GetCellValue(iRow, 0));
		string Units = ConvertString(pGrid->GetCellValue(iRow, 1));
		string ErrorMessage;
		if (!CompatibleUnits(Units, itProperty->second.GetSIUnits(), &ErrorMessage))
		{
			wxMessageBox(ConvertString(ErrorMessage), wxT("Units error"), wxOK | wxICON_ERROR);
		}
		else
		{
			// Set property value
			double dValue = valueify<double>(Value.c_str());
			if ( dValue && ( dValue != itProperty->second.GetValue() ))
			{
				itProperty->second.SetValue( dValue );
			}
			// Set property units
			if ( Units != itProperty->second.GetUnits() )
			{
				itProperty->second.SetUnits( Units );
			}
		}
	}
}

void CModeller::AssignMatrixProperties()
{
	auto textile = CTexGen::Instance().GetTextile(m_TextileName);
	if (!textile)
	{
		return;
	}
	wxDialog Dialog;
	if (wxXmlResource::Get()->LoadDialog(&Dialog, ((CTexGenApp*)wxTheApp)->GetMainFrame(), wxT("Properties")))
	{
		vector<pair<wxString, CProperty>> Properties;
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_Matrix::MATRIX_YOUNGS_MODULUS), textile->m_MatrixYoungsModulus));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_Matrix::MATRIX_POISSONS_RATIO), textile->MatrixPoissonsRatioProperty()));
		Properties.push_back(make_pair(Convert::ToWxString(CProperties_Matrix::MATRIX_ALPHA), textile->MatrixAlphaProperty()));
		auto pGrid = XRCCTRL(Dialog, "PropertiesGrid", wxGrid);
		CreatePropertiesGrid(pGrid, Properties);
		Dialog.SetInitialSize(wxSize(400, 150));
		if (Dialog.ShowModal() == wxID_OK)
		{
			SavePropertiesFromGrid(pGrid, Properties);
			for (auto itProperty = Properties.begin(); itProperty != Properties.end(); itProperty++)
			{
				if (itProperty->second.IsSet())
				{
					switch (Convert::ToMatrixProperty(itProperty->first))
					{
					case CProperties_Matrix::MATRIX_YOUNGS_MODULUS:
						textile->SetMatrixYoungsModulus(itProperty->second.GetValue(), itProperty->second.GetUnits());
						break;
					case CProperties_Matrix::MATRIX_POISSONS_RATIO:
						textile->SetMatrixPoissonsRatio(itProperty->second.GetValue());
						break;
					case CProperties_Matrix::MATRIX_ALPHA:
						textile->SetMatrixAlpha(itProperty->second.GetValue());
						break;
					}
				}
			}
		}
	}
}

void CModeller::CreateYarn()
{
	CTextile* pTextile = TEXGEN.GetTextile(m_TextileName);
	if (!pTextile)
	{
		TGERROR("Need to create textile before adding yarn");
		return;
	}

	bool bFirstYarn = pTextile->GetYarns().empty();
	wxString OriginX = wxT("0.0"), OriginY = wxT("0.0"), OriginZ = wxT("0.0");
	wxString EndX = wxT("10.0"), EndY = wxT("0.0"), EndZ = wxT("0.0");
	wxString NumNodes = wxT("2");

	wxDialog Dialog;
	if (wxXmlResource::Get()->LoadDialog(&Dialog, ((CTexGenApp*)wxTheApp)->GetMainFrame(), wxT("CreateYarn")))
	{
		XRCCTRL(Dialog, "OriginX", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &OriginX));
		XRCCTRL(Dialog, "OriginY", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &OriginY));
		XRCCTRL(Dialog, "OriginZ", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &OriginZ));
		XRCCTRL(Dialog, "EndX", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &EndX));
		XRCCTRL(Dialog, "EndY", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &EndY));
		XRCCTRL(Dialog, "EndZ", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &EndZ));
		XRCCTRL(Dialog, "NumNodes", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &NumNodes));

		if (Dialog.ShowModal() == wxID_OK)
		{
			XYZ Origin, End;
			long iNumNodes = 2;
			OriginX.ToDouble(&Origin.x);
			OriginY.ToDouble(&Origin.y);
			OriginZ.ToDouble(&Origin.z);
			EndX.ToDouble(&End.x);
			EndY.ToDouble(&End.y);
			EndZ.ToDouble(&End.z);
			NumNodes.ToLong(&iNumNodes);
			
			auto yarn = CYarn();
			yarn.AddNode(CNode(XYZ(Origin)));
			for (long i = 1; i < iNumNodes - 1; i++)
			{
				double u = i/double(iNumNodes-1);
				yarn.AddNode(CNode(XYZ(Origin + u * (End - Origin))));
			}
			yarn.AddNode(CNode(XYZ(End)));
			CTexGen::Instance().GetTextile(m_TextileName)->AddYarn(yarn);
			if (bFirstYarn)
			{
				SetDefaultRenderState();
			}
			else
			{
				m_pRenderer->RefreshTextile(m_TextileName);
			}
		}
	}
	UpdateOutlinerItems();
}

void CModeller::SetPosition(int iCoordinate, double Value)
{
	XYZ DeltaPos;
	DeltaPos[iCoordinate] = Value - GetPosition()[iCoordinate];
	m_pCustomWidget->SetPosition(m_pCustomWidget->GetPosition() + DeltaPos);
	MoveSelectedObjects(DeltaPos);
}

void CModeller::SetScale(XYZ Scale)
{
	ScaleSelectedObjects(Scale);
}

void CModeller::RefreshSelectedYarns()
{
	vector<PROP_INFO*>::iterator itObject;
	set<PROP_YARN_INFO> RefreshYarns;
	for (itObject = m_SelectedObjects.begin(); itObject != m_SelectedObjects.end(); ++itObject)
	{
		PROP_YARN_INFO* pYarnInfo = dynamic_cast<PROP_YARN_INFO*>(*itObject);
		PROP_NODE_INFO* pNodeInfo = dynamic_cast<PROP_NODE_INFO*>(*itObject);
		if (pYarnInfo)
		{
			RefreshYarns.insert(*pYarnInfo);
		}
		if (pNodeInfo)
		{
			PROP_YARN_INFO YarnInfo;
			YarnInfo.TextileName = pNodeInfo->TextileName;
			YarnInfo.iYarn = pNodeInfo->iYarn;
			RefreshYarns.insert(YarnInfo);
		}
	}
	m_pRenderer->BeginBatch();
	set<PROP_YARN_INFO>::iterator itYarn;
	for (itYarn = RefreshYarns.begin(); itYarn != RefreshYarns.end(); ++itYarn)
	{
		m_pRenderer->RefreshYarn(*itYarn);
	}
	HighlightSelectedObjects();
	m_pRenderer->EndBatch();
}

XYZ CModeller::GetPosition() const
{
	if (m_SelectedObjects.empty()) {
		return XYZ();
	}
	PROP_INFO* pPropInfo = m_SelectedObjects.back();
	PROP_YARN_INFO* pYarnInfo = dynamic_cast<PROP_YARN_INFO*>(pPropInfo);
	PROP_NODE_INFO* pNodeInfo = dynamic_cast<PROP_NODE_INFO*>(pPropInfo);
	PROP_NODE_INFO NodeInfo;
	if (pYarnInfo)
	{
		NodeInfo.TextileName = pYarnInfo->TextileName;
		NodeInfo.iYarn = pYarnInfo->iYarn;
		NodeInfo.iNode = 0;
	}
	else if (pNodeInfo)
	{
		NodeInfo = *pNodeInfo;
	}
	else
		return XYZ();
	const CNode* pNode = GetNode(NodeInfo);
	if (!pNode)
		return XYZ();
	return pNode->GetPosition();
}

wxColour CModeller::ConvertColor(COLOR Color)
{
	return wxColour((unsigned char)(Color.r * 255), (unsigned char)(Color.g * 255), (unsigned char)(Color.b * 255));
}

void CModeller::UpdateOutlinerItems()
{
	CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
	if (pMainFrame == nullptr)
	{
		return;
	}
	wxPanel* pOutliner = pMainFrame->GetOutliner();
	if (!pOutliner)
	{
		return;
	}

	wxTreeCtrl* pTreeCtrl = XRCCTRL(*pOutliner, "TreeCtrl", wxTreeCtrl);
	CTextile* pTextile = TEXGEN.GetTextile(m_TextileName);
	if (pTreeCtrl && pTextile)
	{
		pTreeCtrl->DeleteAllItems();
		wxTreeItemId TextileItem = pTreeCtrl->AddRoot(ConvertString(m_TextileName));
		vector<CYarn> &Yarns = pTextile->GetYarns();
		for (size_t i = 0; i < Yarns.size(); i++) {
			wxString YarnName = wxT("Yarn (");
			YarnName << i << wxT(")");
			wxTreeItemId YarnItem = pTreeCtrl->AppendItem(TextileItem, YarnName);
			COLOR YarnColor = GetIndexedColor(i);
			pTreeCtrl->SetItemBackgroundColour(YarnItem, ConvertColor(YarnColor));
			const vector<CNode> &Nodes = Yarns[i].GetMasterNodes();
			for (size_t j = 0; j < Nodes.size(); j++) {
				wxString NodeName = wxT("Node (");
				NodeName << j << wxT(")");
				wxTreeItemId NodeItem = pTreeCtrl->AppendItem(YarnItem, NodeName);
			}
		}
		pTreeCtrl->ExpandAll();
		UpdateOutlinerSelection();
	}
}

void CModeller::UpdateOutlinerSelection()
{
	CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
	wxPanel* pOutliner = pMainFrame->GetOutliner();
	if (!pOutliner)
		return;

	wxTreeCtrl* pTreeCtrl = XRCCTRL(*pOutliner, "TreeCtrl", wxTreeCtrl);
	if (pTreeCtrl)
	{
		pTreeCtrl->UnselectAll();
		wxTreeItemId TextileItem = pTreeCtrl->GetRootItem();

		vector<PROP_INFO*>::iterator itObject;
		for (itObject = m_SelectedObjects.begin(); itObject != m_SelectedObjects.end(); ++itObject)
		{
			PROP_YARN_INFO* pYarnInfo = dynamic_cast<PROP_YARN_INFO*>(*itObject);
			PROP_NODE_INFO* pNodeInfo = dynamic_cast<PROP_NODE_INFO*>(*itObject);
			wxTreeItemId SelectItem;
			if (pYarnInfo)
			{
				SelectItem = GetOutlinerItem(pTreeCtrl, pYarnInfo->iYarn);
			}
			else if (pNodeInfo)
			{
				SelectItem = GetOutlinerItem(pTreeCtrl, pNodeInfo->iYarn, pNodeInfo->iNode);
			}
			else
			{
				continue;
			}
			pTreeCtrl->SelectItem(SelectItem);
		}
	}
}

void CModeller::OnOutlinerSelectionChanged(wxPanel* pOutliner)
{
	if (!pOutliner/* || m_bIgnoreSelectionChange*/)
		return;
	wxTreeCtrl* pTreeCtrl = XRCCTRL(*pOutliner, "TreeCtrl", wxTreeCtrl);
	if (pTreeCtrl)
	{
		int i;
		int j;
		DeselectAll(false);
		wxTreeItemId TextileItem = pTreeCtrl->GetRootItem();
		wxTreeItemIdValue YarnItemCookie;
		wxTreeItemId YarnItem;
		for (YarnItem = pTreeCtrl->GetFirstChild(TextileItem, YarnItemCookie), i=0; YarnItem.IsOk();
			 YarnItem = pTreeCtrl->GetNextChild(TextileItem, YarnItemCookie), ++i)
		{
			if (pTreeCtrl->IsSelected(YarnItem))
			{
				PROP_YARN_INFO YarnInfo;
				YarnInfo.TextileName = m_TextileName;
				YarnInfo.iYarn = i;
				SelectObject(new PROP_YARN_INFO(YarnInfo), false);
			}

			wxTreeItemIdValue NodeItemCookie;
			wxTreeItemId NodeItem;
			for (NodeItem = pTreeCtrl->GetFirstChild(YarnItem, NodeItemCookie), j=0; NodeItem.IsOk();
				NodeItem = pTreeCtrl->GetNextChild(YarnItem, NodeItemCookie), ++j)
			{
				if (pTreeCtrl->IsSelected(NodeItem))
				{
					PROP_NODE_INFO NodeInfo;
					NodeInfo.TextileName = m_TextileName;
					NodeInfo.iYarn = i;
					NodeInfo.iNode = j;
					SelectObject(new PROP_NODE_INFO(NodeInfo), false);
				}
			}
		}
		HighlightSelectedObjects();
		UpdateWidget();
	}
}

wxTreeItemId CModeller::GetOutlinerItem(wxTreeCtrl* pTreeCtrl, int iYarn)
{
	wxTreeItemId TextileItem = pTreeCtrl->GetRootItem();
	wxTreeItemIdValue ItemIdValue;
	wxTreeItemId YarnItem = pTreeCtrl->GetFirstChild(TextileItem, ItemIdValue);
	for (int i = 0; i < iYarn; i++) {
		YarnItem = pTreeCtrl->GetNextChild(TextileItem, ItemIdValue);
	}
	return YarnItem;
}

wxTreeItemId CModeller::GetOutlinerItem(wxTreeCtrl* pTreeCtrl, int iYarn, int iNode)
{
	wxTreeItemId YarnItem = GetOutlinerItem(pTreeCtrl, iYarn);
	wxTreeItemIdValue ItemIdValue;
	wxTreeItemId NodeItem = pTreeCtrl->GetFirstChild(YarnItem, ItemIdValue);
	for (int i = 0; i < iNode; i++) {
		NodeItem = pTreeCtrl->GetNextChild(YarnItem, ItemIdValue);
	}
	return NodeItem;
}
