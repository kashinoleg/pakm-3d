#include "stdafx.h"
#include "Textile.h"
#include "TexGen.h"

using namespace TexGen;

#define TOL 1e-10

CTextile::CTextile(void)
: m_bNeedsBuilding(true) {}

CTextile::CTextile(const vector<CYarn> &Yarns)
: m_Yarns(Yarns.size())
{
	copy(Yarns.begin(), Yarns.end(), m_Yarns.begin());
}

CTextile::CTextile(const CTextile &CopyMe)
: CPropertiesTextile(CopyMe)
{
	*this = CopyMe;
}

CTextile::~CTextile(void) {}

CTextile &CTextile::operator=(const CTextile& CopyMe)
{
	m_Yarns = CopyMe.m_Yarns;
	m_bNeedsBuilding = CopyMe.m_bNeedsBuilding;
	m_pDomain = CopyMe.m_pDomain;
	return *this;
}

CTextile::CTextile(TiXmlElement &Element)
:CPropertiesTextile(Element),m_bNeedsBuilding(true)
{
	TiXmlElement* pDomain = Element.FirstChildElement("Domain");
	if (pDomain)
	{
		const string* pType = pDomain->Attribute(string("type"));
		if (pType && *pType == "CDomainPlanes")
		{
			m_pDomain = CDomainPlanes(*pDomain);
		}
	}
	m_bNeedsBuilding = valueify<bool>(Element.Attribute("NeedsBuilding"));
	FOR_EACH_TIXMLELEMENT(pYarn, Element, "Yarn")
	{
		AddYarn(CYarn(*pYarn));
	}
}

void CTextile::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType)
{
	CPropertiesTextile::PopulateTiXmlElement(Element, OutputType);
	Element.SetAttribute("type", GetType());
	if (m_pDomain)
	{
		TiXmlElement Domain("Domain");
		m_pDomain->PopulateTiXmlElement(Domain, OutputType);
		Element.InsertEndChild(Domain);
	}
	// Output the yarns if minimal output hasn't been selected or
	// if this instance of the textile is not derived from (otherwise
	// the output file will be useless)
	if (OutputType != OUTPUT_MINIMAL || GetType() == "CTextile")
	{
		Element.SetAttribute("NeedsBuilding", 0);
		BuildTextileIfNeeded();
		for (size_t i = 0; i < m_Yarns.size(); i++)
		{
			TiXmlElement Yarn("Yarn");
			Yarn.SetAttribute("index", i);
			m_Yarns[i].PopulateTiXmlElement(Yarn, OutputType);
			Element.InsertEndChild(Yarn);
		}
	}
	else
	{
		Element.SetAttribute("NeedsBuilding", 1);
	}
}

int CTextile::AddYarn(const CYarn &Yarn)
{
	// This call may result in the CYarn copy constructor being invoked
	// This is not effecient but more importantly all the parent pointers
	// are reset to NULL
	m_Yarns.push_back(Yarn);
	return m_Yarns.size()-1;
}

int CTextile::AddYarn(const CYarn &Yarn) const
{
	// This call may result in the CYarn copy constructor being invoked
	// This is not effecient but more importantly all the parent pointers
	// are reset to NULL
	m_Yarns.push_back(Yarn);
	return m_Yarns.size()-1;
}

bool CTextile::DeleteYarn(int iIndex)
{
	if (iIndex < 0 || iIndex >= (int)m_Yarns.size())
	{
		return false;
	}
	m_Yarns.erase(m_Yarns.begin()+iIndex);
	return true;
}

void CTextile::DeleteYarns()
{
	m_Yarns.clear();
}

void CTextile::AddNodesToMesh(CMesh &Mesh)
{
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->AddNodesToMesh(Mesh);
	}
}

void CTextile::AddPathToMesh(CMesh &Mesh)
{
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->AddPathToMesh(Mesh);
	}
}

void CTextile::AddSurfaceToMesh(CMesh &Mesh, bool bTrimToDomain)
{
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		if (bTrimToDomain && m_pDomain)
		{
			yarn->AddSurfaceToMesh(Mesh, *m_pDomain);
		}
		else
		{
			yarn->AddSurfaceToMesh(Mesh);
		}
	}
}

bool CTextile::AddSurfaceToMesh(CMesh &Mesh, vector<CMesh> &DomainMeshes, bool bTrimToDomain) {
	if (!BuildTextileIfNeeded())
	{
		return false;
	}
	if (!m_pDomain)
	{
		TGERROR("Textile has no domain assigned");
		return false;
	}
	CMesh DomainMesh = m_pDomain->GetMesh();
	// For the most part domain will be box in which case want the quad elements not the tris
	DomainMesh.ConvertTriToQuad();

	// Save each domain face as separate mesh & add to DomainMeshes vector
	for ( int i = 0; i < CMesh::NUM_ELEMENT_TYPES; ++i)
	{
		const list<int> &Indices = DomainMesh.GetIndices((CMesh::ELEMENT_TYPE)i);
		int iNumNodes = CMesh::GetNumNodes((CMesh::ELEMENT_TYPE)i);
		for (list<int>::const_iterator itIter = Indices.begin(); itIter != Indices.end(); )
		{
			if (i != CMesh::POLYGON)
			{
				CMesh FaceMesh;
				vector<int> index;
				for ( int j = 0; j < iNumNodes; ++j )
				{
					FaceMesh.AddNode( DomainMesh.GetNode(*(itIter++)));
					index.push_back(j);
				}
				FaceMesh.AddElement( (CMesh::ELEMENT_TYPE)i, index );
				DomainMeshes.push_back(FaceMesh);
			}
		}
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		if (bTrimToDomain && m_pDomain)
		{
			if (!yarn->AddSurfaceToMesh(Mesh, *m_pDomain, DomainMeshes))
			{
				return false;
			}
		}
		else
		{
			yarn->AddSurfaceToMesh(Mesh);
		}
	}
	return true;
}

void CTextile::AddVolumeToMesh(CMesh &Mesh, bool bTrimToDomain) {
	if (!BuildTextileIfNeeded()) {
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		if (bTrimToDomain && m_pDomain)
		{
			yarn->AddVolumeToMesh(Mesh, *m_pDomain);
		}
		else
		{
			yarn->AddVolumeToMesh(Mesh);
		}
	}
}

void CTextile::AddCentrePlaneToMesh(CMesh &Mesh, bool bTrimToDomain) {
	if (!BuildTextileIfNeeded()) {
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		if (bTrimToDomain && m_pDomain)
		{
			yarn->AddCentrePlaneToMesh(Mesh, *m_pDomain);
		}
		else
		{
			yarn->AddCentrePlaneToMesh(Mesh);
		}
	}
}

void CTextile::Rotate(WXYZ Rotation, XYZ Origin) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->Rotate(Rotation, Origin);
	}
}

void CTextile::Translate(XYZ Vector)
{
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->Translate(Vector);
	}
}

string CTextile::GetName() const
{
	return TEXGEN.GetName(this);
}

void CTextile::GetPointInformation(const vector<XYZ> &Points, vector<POINT_INFO> &PointsInfo, double dTolerance)
{
	if (Points.empty())
	{
		return;
	}
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	PointsInfo.clear();
	PointsInfo.resize(Points.size());
	
	std::vector<XY>::iterator itLoc; 	
	vector<POINT_INFO>::iterator itInfo;
	POINT_INFO Info;

	XYZ Min;
	XYZ Max;
	vector<XYZ>::const_iterator itPoint;
	for (itPoint = Points.begin(); itPoint != Points.end(); ++itPoint)
	{
		if (itPoint == Points.begin())
		{
			Min = *itPoint;
			Max = *itPoint;
		}
		else
		{
			Min = ::Min(Min, *itPoint);
			Max = ::Max(Max, *itPoint);
		}
	}
	XYZ Tangent;
	XY Loc;
	CDomainPlanes DomainPlanes(Min, Max);
	vector<CYarn>::iterator itYarn;
	int iIndex;
	for (itYarn = m_Yarns.begin(), iIndex=0; itYarn != m_Yarns.end(); ++itYarn, ++iIndex)
	{
		vector<XYZ> Translations = DomainPlanes.GetTranslations(*itYarn);
		for (itPoint = Points.begin(), itInfo = PointsInfo.begin(); itPoint != Points.end(); ++itPoint, ++itInfo)
		{
			if (itYarn->PointInsideYarn(*itPoint, Translations, &Info.YarnTangent, 
				&Info.Location, &Info.dVolumeFraction, &Info.dSurfaceDistance, dTolerance, &Info.Orientation, &Info.Up))
			{
				// If the point is inside several yarns, either because the yarns overlap or because
				// the tolerance is set too high, the point is assigned the yarn which it lies deepest
				// within. I.e. the one with the lowest surface distance (note that negative surface
				// distance represents a point lying below the yarn surface).
				if (itInfo->iYarnIndex == -1 || Info.dSurfaceDistance < itInfo->dSurfaceDistance)
				{
					*itInfo = Info;
					itInfo->iYarnIndex = iIndex;
				}
			}
		}
	}
}

void CTextile::GetPointInformation(const vector<XYZ> &Points, vector<POINT_INFO> &PointsInfo, int iYarn, double dTolerance) {
	if (Points.empty())
	{
		return;
	}
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	PointsInfo.clear();
	PointsInfo.resize(Points.size());
	vector<XYZ>::const_iterator itPoint;
	std::vector<XY>::iterator itLoc; 	
	vector<POINT_INFO>::iterator itInfo;
	POINT_INFO Info;

	XYZ Min;
	XYZ Max;
	for (itPoint = Points.begin(); itPoint != Points.end(); ++itPoint)
	{
		if (itPoint == Points.begin())
		{
			Min = *itPoint;
			Max = *itPoint;
		}
		else
		{
			Min = ::Min(Min, *itPoint);
			Max = ::Max(Max, *itPoint);
		}
	}
	XYZ Tangent;
	XY Loc;
	CDomainPlanes DomainPlanes(Min, Max);
	vector<CYarn>::iterator itYarn;
	
	vector<XYZ> Translations = DomainPlanes.GetTranslations(m_Yarns[iYarn]);
	for (itPoint = Points.begin(), itInfo = PointsInfo.begin(); itPoint != Points.end(); ++itPoint, ++itInfo)
	{
		if (m_Yarns[iYarn].PointInsideYarn(*itPoint, Translations, &Info.YarnTangent, 
			&Info.Location, &Info.dVolumeFraction, &Info.dSurfaceDistance, dTolerance, &Info.Orientation, &Info.Up))
		{
			// If the point is inside several yarns, either because the yarns overlap or because
			// the tolerance is set too high, the point is assigned the yarn which it lies deepest
			// within. I.e. the one with the lowest surface distance (note that negative surface
			// distance represents a point lying below the yarn surface).
			if (itInfo->iYarnIndex == -1 )//|| Info.dSurfaceDistance < itInfo->dSurfaceDistance)
			{
				*itInfo = Info;
				itInfo->iYarnIndex = iYarn;
			}
		}
		else
		{
		}
	}
	
}

void CTextile::SavePointInformationToVTK(string Filename, const CMesh &Mesh, double dTolerance) {
	vector<POINT_INFO> PointsInfo;
	GetPointInformation(Mesh.GetNodes(), PointsInfo, dTolerance);

	CMeshData<char> YarnIndex("YarnIndex", CMeshDataBase::ELEMENT);
	CMeshData<XYZ> YarnTangent("YarnTangent", CMeshDataBase::ELEMENT);
	CMeshData<XY> Location("Location", CMeshDataBase::ELEMENT);
	CMeshData<double> VolumeFraction("VolumeFraction", CMeshDataBase::ELEMENT);
	CMeshData<double> SurfaceDistance("SurfaceDistance", CMeshDataBase::ELEMENT);
	CMeshData<XYZ> Orientation("Orientation", CMeshDataBase::ELEMENT);

	for (POINT_INFO pointInfo : PointsInfo) {
		YarnIndex.m_Data.push_back(pointInfo.iYarnIndex);
		YarnTangent.m_Data.push_back(pointInfo.YarnTangent);
		Location.m_Data.push_back(pointInfo.Location);
		VolumeFraction.m_Data.push_back(pointInfo.dVolumeFraction);
		SurfaceDistance.m_Data.push_back(pointInfo.dSurfaceDistance);
		Orientation.m_Data.push_back(pointInfo.Orientation);
	}

	vector<CMeshDataBase*> MeshData;

	MeshData.push_back(&YarnIndex);
	MeshData.push_back(&YarnTangent);
	MeshData.push_back(&Location);
	MeshData.push_back(&VolumeFraction);
	MeshData.push_back(&SurfaceDistance);
	MeshData.push_back(&Orientation);

	Mesh.SaveToVTK(Filename, &MeshData);
}

double CTextile::GetApproximateSize() {
	if (!BuildTextileIfNeeded())
	{
		return 0;
	}
	XYZ Min;
	XYZ Max;
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		for (CNode node : yarn->GetMasterNodes())
		{
			XYZ Pos = node.GetPosition();
			Min = ::Min(Min, Pos);
			Max = ::Max(Max, Pos);
		}
	}
	return GetLength(Min, Max);
}

int CTextile::DetectInterference(vector<float> &DistanceToSurface, vector<int> &YarnIndices,
	bool bTrimToDomain, CMesh *pInterferingPoints ) {
	if (!BuildTextileIfNeeded()) {
		return -1;
	}
	if ( bTrimToDomain && !m_pDomain ) {
		TGERROR( "Cannot trim interference points to domain - no domain specified" );
		return -1;
	}
	if (pInterferingPoints) {
		pInterferingPoints->Clear();
	}
	TGLOGINDENT("Detecting interference between yarns in textile \"" << GetName() << "\"");
	int iIntersections = 0;
	vector<CYarn>::iterator itYarn;
	vector<CYarn>::iterator itYarnCompare;
	
	pair<XYZ, XYZ> AABB1, AABB2;
	// Parameters to send to PointInsideYarn. Only need because default parameters before pDistanceToSurface
	XYZ pTangent;
	XY pLoc;
	double pVolumeFraction;
	double pDistanceToSurface;
	int iIndex;
	for (itYarn = m_Yarns.begin(), iIndex = 0; itYarn != m_Yarns.end(); ++itYarn, ++iIndex)
	{
		for (itYarnCompare = m_Yarns.begin(); itYarnCompare != m_Yarns.end(); ++itYarnCompare)
		{
			if (itYarn == itYarnCompare)
				continue;
			if ( bTrimToDomain )
			{
				CMesh YarnMesh, CompareMesh;
				
				itYarn->AddSurfaceToMesh(YarnMesh, m_pDomain);
				AABB1 = YarnMesh.GetAABB();
				itYarnCompare->AddSurfaceToMesh( CompareMesh, m_pDomain );
				AABB2 = CompareMesh.GetAABB();
			}
			else {
				AABB1 = itYarn->GetAABB();
				AABB2 = itYarnCompare->GetAABB();
			}
			if (!BoundingBoxIntersect(AABB1.first, AABB1.second, AABB2.first, AABB2.second, 1e-9)) {
				continue;
			}
			vector<XYZ> YarnTranslations;
			vector<XYZ> CompareTranslations;
			if( bTrimToDomain ) {
				YarnTranslations = m_pDomain->GetTranslations(*itYarn);
				CompareTranslations = m_pDomain->GetTranslations(*itYarnCompare);
			}
			else {
				XYZ Translation(0,0,0);
				YarnTranslations.push_back(Translation);
				CompareTranslations.push_back(Translation);
			}

			const vector<CSlaveNode> &SlaveNodes = itYarn->GetSlaveNodes(CYarn::SURFACE);
			
			for (CSlaveNode node : SlaveNodes) {
				for (XYZ point : node.GetSectionPoints()) {
					for (XYZ translate : YarnTranslations) {
						XYZ position = point + translate;
						if ( !bTrimToDomain || m_pDomain->PointInDomain(position)) // Don't need to check for intersection if point outside domain
						{
							if (itYarnCompare->PointInsideYarn(position, CompareTranslations, &pTangent, &pLoc, &pVolumeFraction, &pDistanceToSurface))
							{
								if (pInterferingPoints)
									pInterferingPoints->AddNode(position);
								DistanceToSurface.push_back( (float)pDistanceToSurface );
								YarnIndices.push_back( iIndex );
								
								++iIntersections;
							}
						}
					}
				}
			}
		}
	}
	TGLOG("Found " << iIntersections << " intersections between yarns in textile \"" << GetName() << "\"");
	return iIntersections;
}

vector<CYarn> &CTextile::GetYarns()
{
	BuildTextileIfNeeded();
	return m_Yarns;
}

const vector<CYarn> &CTextile::GetYarns() const {
	BuildTextileIfNeeded();
	return m_Yarns;
}

CYarn *CTextile::GetYarn(int iIndex) {
	BuildTextileIfNeeded();
	if (iIndex < 0 || iIndex >= (int)m_Yarns.size()) {
		TGERROR("Unable to get yarn, invalid index: " << iIndex);
		return NULL;
	}
	return &m_Yarns[iIndex];
}

const CYarn *CTextile::GetYarn(int iIndex) const {
	BuildTextileIfNeeded();
	if (iIndex < 0 || iIndex >= (int)m_Yarns.size()) {
		TGERROR("Unable to get yarn, invalid index: " << iIndex);
		return NULL;
	}
	return &m_Yarns[iIndex];
}

int CTextile::GetNumYarns() const {
	BuildTextileIfNeeded();
	return (int)m_Yarns.size();
}

bool CTextile::BuildTextileIfNeeded() const {
	if (!m_bNeedsBuilding) {
		return true;
	}
	else {
		// Even if the build fails, we set this flag to false because if nothing happens
		// which changes the textile between this call and the next call. The next call
		// will also fail, thus there is no point to call it again.
		m_bNeedsBuilding = false;
		return BuildTextile();
	}
}

void CTextile::AssignDomain(const CDomain &Domain)
{
	m_pDomain = Domain;
}

void CTextile::RemoveDomain()
{
	m_pDomain.Clear();
}

double CTextile::GetYarnLength(string Units)
{
	if (!BuildTextileIfNeeded())
	{
		return 0;
	}
	double dYarnLength = 0;
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		dYarnLength += yarn->GetRealYarnLength(Units);
	}
	return dYarnLength;
}

double CTextile::GetYarnLengthPerUnitArea(string Units)
{
	if (!BuildTextileIfNeeded())
	{
		return 0;
	}
	double dYarnLength = 0;
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		// If one of the yarns doesn't have correct repeat vectors then we
		// cannot calculate an accurate value of length per unit area for the
		// whole textile
		if (yarn->GetRawRepeatArea() == 0)
		{
			return 0;
		}
		dYarnLength += yarn->GetYarnLengthPerUnitArea(Units);
	}
	return dYarnLength;
}

double CTextile::GetYarnVolume(string Units) {
	if (!BuildTextileIfNeeded())
	{
		return 0;
	}
	double dVolume = 0;
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		dVolume += yarn->GetRealYarnVolume(Units);
	}
	return dVolume;
}

double CTextile::GetYarnVolumePerUnitArea(string Units) {
	if (!BuildTextileIfNeeded())
	{
		return 0;
	}
	double dVolPerUnitArea = 0;
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		// If one of the yarns doesn't have correct repeat vectors then we
		// cannot calculate an accurate value of volume per unit area for the
		// whole textile
		if (yarn->GetRawRepeatArea() == 0)
		{
			return 0;
		}
		dVolPerUnitArea += yarn->GetYarnVolumePerUnitArea(Units);
	}
	return dVolPerUnitArea;
}

double CTextile::GetFibreVolume(string Units)
{
	if (!BuildTextileIfNeeded())
	{
		return 0;
	}
	double dVolume = 0;
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		dVolume += yarn->GetFibreVolume(Units);
	}
	return dVolume;
}

double CTextile::GetFibreVolumePerUnitArea(string Units)
{
	if (!BuildTextileIfNeeded())
	{
		return 0;
	}
	// Try to calculate the fibre volume per unit area based on areal density
	if (m_ArealDensity.IsSet() && m_FibreDensity.IsSet())
	{
		double dVolPerUnitArea = m_ArealDensity.GetSIValue()/m_FibreDensity.GetSIValue();
		return ConvertUnits(dVolPerUnitArea, "m", Units);
	}
	// If areal density is missing then calculate it based on linear density of the yarns
	else {
		double dVolPerUnitArea = 0;
		for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
		{
			// If one of the yarns doesn't have correct repeat vectors then we
			// cannot calculate an accurate value fibre volume per unit area for the
			// whole textile
			if (yarn->GetRawRepeatArea() == 0) {
				return 0;
			}
			dVolPerUnitArea += yarn->GetFibreVolumePerUnitArea(Units);
		}
		return dVolPerUnitArea;
	}
}

double CTextile::GetFibreYarnVolumeFraction() {
	if (!BuildTextileIfNeeded()) {
		return 0;
	}
	double dYarnVolumePerUnitArea = GetYarnVolumePerUnitArea();
	if (dYarnVolumePerUnitArea) {
		return GetFibreVolumePerUnitArea() / dYarnVolumePerUnitArea;
	}
	else if (double dYarnVolume = GetYarnVolume()) {
		return GetFibreVolume()/dYarnVolume;
	}
	return 0;
}

CTextileWeave* CTextile::GetWeave() {
	return dynamic_cast<CTextileWeave*>(this);
}

CTextileLayered* CTextile::GetLayeredTextile() {
	return dynamic_cast<CTextileLayered*>(this);
}

CTextile3DWeave* CTextile::Get3DWeave() {
	return dynamic_cast<CTextile3DWeave*>(this);
}

CTextileOrthogonal* CTextile::GetOrthogonalWeave() {
	return dynamic_cast<CTextileOrthogonal*>(this);
}

CTextileLayerToLayer* CTextile::GetLayerToLayerWeave() {
	return dynamic_cast<CTextileLayerToLayer*>(this);
}

void CTextile::SetAllYarnsYoungsModulusX(double dValue, string Units) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetYoungsModulusX(dValue, Units);
	}
}

void CTextile::SetAllYarnsYoungsModulusY(double dValue, string Units) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetYoungsModulusY(dValue, Units);
	}
}

void CTextile::SetAllYarnsYoungsModulusZ( double dValue, string Units) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetYoungsModulusZ(dValue, Units);
	}
}

void CTextile::SetAllYarnsShearModulusXY(double dValue, string Units) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetShearModulusXY(dValue, Units);
	}
}

void CTextile::SetAllYarnsShearModulusXZ(double dValue, string Units)
{
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetShearModulusXZ(dValue, Units);
	}
}

void CTextile::SetAllYarnsShearModulusYZ(double dValue, string Units) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetShearModulusYZ(dValue, Units);
	}
}

void CTextile::SetAllYarnsAlphaX(double dValue, string Units) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetAlphaX(dValue, Units);
	}
}

void CTextile::SetAllYarnsAlphaY(double dValue, string Units) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetAlphaY( dValue, Units );
	}
}

void CTextile::SetAllYarnsAlphaZ(double dValue, string Units) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetAlphaZ(dValue, Units);
	}
}

void CTextile::SetAllYarnsPoissonsRatioX(double dValue) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetPoissonsRatioX( dValue );
	}
}

void CTextile::SetAllYarnsPoissonsRatioY(double dValue) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetPoissonsRatioY(dValue);
	}
}

void CTextile::SetAllYarnsPoissonsRatioZ(double dValue) {
	if (!BuildTextileIfNeeded())
	{
		return;
	}
	for (auto yarn = m_Yarns.begin(); yarn != m_Yarns.end(); yarn++)
	{
		yarn->SetPoissonsRatioZ( dValue );
	}
}

double CTextile::GetQuickDomainVolumeFraction() {
	if( !m_pDomain ) {
		TGERROR("Cannot calculate volume fraction. No domain specified");
		return 0.0;
	}
	if (!BuildTextileIfNeeded()) {
		return 0.0;
	}

	
	double FibreVolume = 0.0;
	
	int i = 0;

	vector<CYarn>::iterator itYarn;
	for ( itYarn = m_Yarns.begin(); itYarn != m_Yarns.end(); ++itYarn, ++i ) {
		CMesh Mesh;
		double YarnVolume, YarnFibreVf;
		// Calculate volume of yarn within the domain
		itYarn->AddSurfaceToMesh( Mesh, m_pDomain );
		YarnVolume = Mesh.CalculateVolume();
		
		YarnFibreVf = itYarn->GetFibreYarnVolumeFraction(); //Based on whole yarn
		if ( YarnFibreVf < TOL ) {
			stringstream Message;
			Message << "No fibre data specified for yarn " << i << ". Using Vf = 1.0\n";
			TGERROR( Message.str() );
			YarnFibreVf = 1.0;
		}
		FibreVolume += YarnFibreVf * YarnVolume;
	}
	return FibreVolume/m_pDomain->GetVolume();
}

double CTextile::GetDomainVolumeFraction() {
	if(!m_pDomain) {
		TGERROR("Cannot calculate volume fraction. No domain specified");
		return 0.0;
	}
	if (!BuildTextileIfNeeded()) {
		return 0.0;
	}
	vector<CYarn>::iterator itYarn;
	double FibreVolume = 0.0;
	int i = 0;

	for ( itYarn = m_Yarns.begin(); itYarn != m_Yarns.end(); ++itYarn, ++i )
	{
		CMesh Mesh;
		double YarnFibreVf;
		vector<XYZ> Centres;
		vector<XYZ>::iterator  itCentres;
		XYZ Tangent;
		XY Loc;
		int i1, i2, i3, i4;
		const XYZ *p1, *p2, *p3, *p4;
		bool bDefaultVf = false;

		// Create volume mesh for domain
		itYarn->AddVolumeToMesh( Mesh, *m_pDomain );
		Mesh.ConvertToTetMesh();

		vector<XYZ> Translations = m_pDomain->GetTranslations(*itYarn);
		Centres = Mesh.GetElementCenters();
		list<int> &TetIndices = Mesh.GetIndices(CMesh::TET);
		list<int>::iterator itTetInd;

		YarnFibreVf = itYarn->GetFibreYarnVolumeFraction();
		if ( YarnFibreVf < TOL )
		{
			stringstream Message;
			Message << "No fibre data specified for yarn " << i << ". Using Vf = 1.0\n";
			TGERROR( Message.str() );
			YarnFibreVf = 1.0;
			bDefaultVf = true;
		}

		// Get fibre volume for each tet based on its area and local volume fraction
		for ( itCentres = Centres.begin(), itTetInd = TetIndices.begin(); itCentres != Centres.end(); ++itCentres )
		{
			XYZ Centre = *itCentres;
			if ( !bDefaultVf )
			{
				itYarn->PointInsideYarn( *itCentres, Translations, &Tangent, &Loc, &YarnFibreVf );
			}
			i1 = *(itTetInd++);
			i2 = *(itTetInd++);
			i3 = *(itTetInd++);
			i4 = *(itTetInd++);

			p1 = &Mesh.GetNode(i1);
			p2 = &Mesh.GetNode(i2);
			p3 = &Mesh.GetNode(i3);
			p4 = &Mesh.GetNode(i4);
			double TetVolume = fabs( DotProduct(CrossProduct(((*p1)-(*p2)),((*p1)-(*p3))),((*p1)-(*p4))))/6.0;
			FibreVolume += YarnFibreVf * TetVolume;
		}
	}
	return FibreVolume/m_pDomain->GetVolume();
}
