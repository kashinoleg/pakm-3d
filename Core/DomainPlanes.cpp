#include "DomainPlanes.h"
#include "Yarn.h"

using namespace TexGen;

CDomainPlanes::CDomainPlanes(const vector<PLANE> &Planes)
	: m_Planes(Planes)
{
	BuildMesh();
}

CDomainPlanes::CDomainPlanes(const XYZ &Min, const XYZ &Max)
{
	m_Planes.push_back(PLANE(XYZ(1, 0, 0), Min.x));
	m_Planes.push_back(PLANE(XYZ(-1, 0, 0), -Max.x));
	m_Planes.push_back(PLANE(XYZ(0, 1, 0), Min.y));
	m_Planes.push_back(PLANE(XYZ(0, -1, 0), -Max.y));
	m_Planes.push_back(PLANE(XYZ(0, 0, 1), Min.z));
	m_Planes.push_back(PLANE(XYZ(0, 0, -1), -Max.z));
	BuildMesh();
}

CDomainPlanes::CDomainPlanes(TiXmlElement &Element)
	: CDomain(Element)
{
	PLANE Plane;
	FOR_EACH_TIXMLELEMENT(pPlane, Element, "Plane")
	{
		Plane.Normal = valueify<XYZ>(pPlane->Attribute("Normal"));
		pPlane->Attribute("d", &Plane.d);
		m_Planes.push_back(Plane);
	}
	if (m_Mesh.GetNumNodes() == 0) {
		BuildMesh();
	}
}

void CDomainPlanes::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const {
	CDomain::PopulateTiXmlElement(Element, OutputType);
	for (vector<PLANE>::const_iterator itPlane = m_Planes.begin(); itPlane != m_Planes.end(); itPlane++) {
		TiXmlElement Plane("Plane");
		Plane.SetAttribute("Normal", stringify(itPlane->Normal));
		Plane.SetAttribute("d", stringify(itPlane->d));
		Element.InsertEndChild(Plane);
	}
}

void CDomainPlanes::AddPlane(const PLANE &Plane) {
	m_Planes.push_back(Plane);
	BuildMesh();
}

void CDomainPlanes::Grow(double dDistance) {
	for (vector<PLANE>::iterator itPlane = m_Planes.begin(); itPlane != m_Planes.end(); itPlane++) {
		itPlane->d -= dDistance;
	}
	BuildMesh();
}

void CDomainPlanes::Rotate(WXYZ Rotation) {
	for (vector<PLANE>::iterator itPlane = m_Planes.begin(); itPlane != m_Planes.end(); itPlane++) {
		itPlane->Normal = Rotation * itPlane->Normal;
	}
	BuildMesh();
}

void CDomainPlanes::Translate(XYZ Vector) {
	for (vector<PLANE>::iterator itPlane = m_Planes.begin(); itPlane != m_Planes.end(); itPlane++) {
		itPlane->d += DotProduct(Vector, itPlane->Normal);
	}
	BuildMesh();
}

void CDomainPlanes::Deform(CLinearTransformation Transformation) {
	// See http://www.unknownroad.com/rtfm/graphics/rt_normals.html for details
	// On why we create the NormalTransformation matrix like this
	CMatrix NormalTransformation;
	Transformation.GetInverse(NormalTransformation);
	NormalTransformation = NormalTransformation.GetTranspose();
	for (vector<PLANE>::iterator itPlane = m_Planes.begin(); itPlane != m_Planes.end(); itPlane++) {
		XYZ P = itPlane->Normal * itPlane->d;
		P = Transformation * P;
		itPlane->Normal = NormalTransformation * itPlane->Normal;
		Normalise(itPlane->Normal);
		itPlane->d = DotProduct(itPlane->Normal, P);
	}
	BuildMesh();
}

vector<pair<int, int> > CDomainPlanes::GetRepeatLimits(const CYarn &Yarn) const {
	// Check for all repeats being set to zero
	int j = 0;
	for (XYZ repeat : Yarn.GetRepeats()) {
		if (!repeat) {
			j++;
		}
	}
	if (j == Yarn.GetRepeats().size()) {
		return vector<pair<int, int> >();
	}
	//TGLOG("Calculate repeat");


	vector<pair<double, double> > AllRepeatLimitsPrevious;
	vector<pair<double, double> > AllRepeatLimits;
	AllRepeatLimits.resize(Yarn.GetRepeats().size(), pair<double, double>(0, 0));
	int iIterations = 0;
	int iMaxIterations = 100;
	// Use an iterative method to find the repeat limits
	do
	{
		++iIterations;
		AllRepeatLimitsPrevious = AllRepeatLimits;
		// Loop over all the repeats
		int i = 0;
		for (XYZ repeat : Yarn.GetRepeats()) {
			AllRepeatLimits[i] = pair<double, double>(0, 0);
			CMesh Mesh;	// Create an empty mesh
			// Get a surface mesh of the yarns with the repeats calculated so far
			if (!Yarn.AddAABBToMesh(Mesh, ConvertLimitsToInt(AllRepeatLimits)))
			{
				TGERROR("Unable to calculate repeat limits");
				assert(false);
				return ConvertLimitsToInt(AllRepeatLimits);
			}

			AllRepeatLimits[i] = GetLimits(repeat, Mesh);
			++i;
		}
		// Keep going until the repeats from the previous iteration are the same as the repeats for the current
		// iteration. Or until the operation fails due to too many iterations.
	} while (AllRepeatLimitsPrevious != AllRepeatLimits && iIterations < iMaxIterations);



	if (iIterations >= iMaxIterations)
	{
		TGERROR("Unable to find yarn repeat limits, stopped after " << iIterations << " iterations");
		return vector<pair<int, int> >();
	}
	//TGLOG("Found yarn repeat limits after " << iIterations << " iterations");
	return ConvertLimitsToInt(AllRepeatLimits);
}


vector<XYZ> CDomainPlanes::GetRepeats(const CYarn &Yarn) const {
	const vector<XYZ> &YarnRepeats = Yarn.GetRepeats();
	vector<pair<int, int> > RepeatLimits = GetRepeatLimits(Yarn);  // How many times to repeat from original yarn


	vector<XYZ> AllRepeats;
	AllRepeats.push_back(XYZ());


	vector<pair<int, int> >::const_iterator itLimits = RepeatLimits.begin();
	for (vector<XYZ>::const_iterator itRepeat = YarnRepeats.begin();
		itRepeat != YarnRepeats.end() && itLimits != RepeatLimits.end(); ++itRepeat, ++itLimits)
	{
		vector<XYZ> OriginalOffsets = AllRepeats;
		AllRepeats.clear();
		for (int i = itLimits->first; i <= itLimits->second; i++) {
			for (XYZ offset : OriginalOffsets) {
				AllRepeats.push_back(offset + i * *itRepeat);
			}
		}
	}
	return AllRepeats;
}

vector<XYZ> CDomainPlanes::GetTranslations(const CYarn &Yarn) const {
	CMesh Mesh;	// Create an empty mesh
	Yarn.AddAABBToMesh(Mesh); // Adds bounding box of yarn surfaces to mesh (?)

	vector<XYZ> FiniteRepeats;
	for (XYZ repeat : GetRepeats(Yarn)) {
		CMesh RepeatedMesh = Mesh;
		RepeatedMesh.Translate(repeat);
		if (MeshIntersectsDomain(RepeatedMesh)) {
			FiniteRepeats.push_back(repeat);
		}
	}
	return FiniteRepeats;
}

pair<double, double> CDomainPlanes::GetLimits(XYZ RepeatVector, const CMesh &Mesh) const
{
	pair<double, double> DomainLimits = make_pair(0.0, 0.0);
	pair<double, double> MeshLimits = make_pair(0.0, 0.0);
	pair<double, double> Limits = make_pair(0.0, 0.0);
	double dPointDist;//, dFurthestPointDist = 0;
	vector<XYZ>::const_iterator itNode;
	const vector<XYZ> MeshNodes = m_Mesh.GetNodes();

	double dRepeatLength = GetLength(RepeatVector);
	RepeatVector /= dRepeatLength;
	//for (itNode = m_Mesh.NodesBegin(); itNode != m_Mesh.NodesEnd(); ++itNode)
	for (itNode = MeshNodes.begin(); itNode != MeshNodes.end(); ++itNode)
	{
		dPointDist = DotProduct(*itNode, RepeatVector);

		if (itNode == MeshNodes.begin())
			DomainLimits = make_pair(dPointDist, dPointDist);
		else if (DomainLimits.first > dPointDist)
			DomainLimits.first = dPointDist;
		else if (DomainLimits.second < dPointDist)
			DomainLimits.second = dPointDist;
	}

	for (itNode = Mesh.NodesBegin(); itNode != Mesh.NodesEnd(); ++itNode)
	{
		dPointDist = DotProduct(*itNode, RepeatVector);

		if (itNode == Mesh.NodesBegin())
			MeshLimits = make_pair(dPointDist, dPointDist);
		else if (MeshLimits.first > dPointDist)
			MeshLimits.first = dPointDist;
		else if (MeshLimits.second < dPointDist)
			MeshLimits.second = dPointDist;
	}
	Limits.first = (DomainLimits.first - MeshLimits.second) / dRepeatLength;
	Limits.second = (DomainLimits.second - MeshLimits.first) / dRepeatLength;
	return Limits;
}

bool CDomainPlanes::MeshIntersectsDomain(const CMesh &Mesh) const {
	pair<XYZ, XYZ> DomainAABB = m_Mesh.GetAABB();
	pair<XYZ, XYZ> MeshAABB = Mesh.GetAABB();
	return BoundingBoxIntersect(DomainAABB.first, DomainAABB.second, MeshAABB.first, MeshAABB.second);
}

bool CDomainPlanes::GetBoxLimits(XYZ &Min, XYZ &Max)
{
	if (m_Planes.size() != 6)
		return false;
	int i;
	bool bLimitsSet[6] = { false,false,false,false,false,false };
	for (i = 0; i < 6; ++i)
	{
		if (m_Planes[i].Normal == XYZ(1, 0, 0))
		{
			Min.x = m_Planes[i].d;
			bLimitsSet[0] = true;
		}
		if (m_Planes[i].Normal == XYZ(0, 1, 0))
		{
			Min.y = m_Planes[i].d;
			bLimitsSet[1] = true;
		}
		if (m_Planes[i].Normal == XYZ(0, 0, 1))
		{
			Min.z = m_Planes[i].d;
			bLimitsSet[2] = true;
		}
		if (m_Planes[i].Normal == XYZ(-1, 0, 0))
		{
			Max.x = -m_Planes[i].d;
			bLimitsSet[3] = true;
		}
		if (m_Planes[i].Normal == XYZ(0, -1, 0))
		{
			Max.y = -m_Planes[i].d;
			bLimitsSet[4] = true;
		}
		if (m_Planes[i].Normal == XYZ(0, 0, -1))
		{
			Max.z = -m_Planes[i].d;
			bLimitsSet[5] = true;
		}
	}
	for (i = 0; i < 6; ++i)
	{
		if (bLimitsSet[i] == false)
			return false;
	}
	return true;
}

void CDomainPlanes::ClipMeshToDomain(CMesh &Mesh, bool bFillGaps) const
{
	const double TOL = 1e-9;

	TGLOGINDENT("Clipping mesh to domain");

	vector<XYZ> NewTriangles;
	vector<bool> NewTrianglesFlipped;
	vector<XYZ>::iterator itXYZ;
	vector<XYZ> NewQuads;

	vector<PLANE>::const_iterator itPlane;
	list<int>::iterator itStart;
	list<int>::iterator itInt;
	const XYZ *p1, *p2, *p3, *p4;
	double d1, d2, d3, d4;	// d represents the distance of the point to the plane (i.e. +ve inside, -ve outside, 0 on top)
	double dTemp;
	const XYZ *pTemp;
	double u;
	int i;
	int iLastNodeIndex;
	bool bFlipped;

	// Deal with surface elements
	for (itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane)
	{
		list<int> &QuadIndices = Mesh.GetIndices(CMesh::QUAD);
		for (itInt = QuadIndices.begin(); itInt != QuadIndices.end(); )
		{
			itStart = itInt;

			p1 = &Mesh.GetNode(*(itInt++));
			p2 = &Mesh.GetNode(*(itInt++));
			p3 = &Mesh.GetNode(*(itInt++));
			p4 = &Mesh.GetNode(*(itInt++));

			d1 = DotProduct(itPlane->Normal, *p1) - itPlane->d;
			d2 = DotProduct(itPlane->Normal, *p2) - itPlane->d;
			d3 = DotProduct(itPlane->Normal, *p3) - itPlane->d;
			d4 = DotProduct(itPlane->Normal, *p4) - itPlane->d;

			if (d1 <= TOL && d2 <= TOL && d3 <= TOL && d4 <= TOL) // The quad lies completely on or outside the plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
			}
			else if (d1 >= -TOL && d2 >= -TOL && d3 >= -TOL && d4 >= -TOL) // The quad lies completely inside the plane
			{
				// Do nothing
			}
			else if (d1 < TOL && d2 < TOL && d3 > TOL && d4 > TOL) // Points 1 & 2 outside plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
				u = d4 / (d4 - d1);
				NewQuads.push_back(*p4 + (*p1 - *p4) * u);
				u = d3 / (d3 - d2);
				NewQuads.push_back(*p3 + (*p2 - *p3) * u);
				NewQuads.push_back(*p3);
				NewQuads.push_back(*p4);
			}
			else if (d2 < TOL && d3 < TOL && d4 > TOL && d1 > TOL) // Points 2 & 3 outside plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
				NewQuads.push_back(*p1);
				u = d1 / (d1 - d2);
				NewQuads.push_back(*p1 + (*p2 - *p1) * u);
				u = d4 / (d4 - d3);
				NewQuads.push_back(*p4 + (*p3 - *p4) * u);
				NewQuads.push_back(*p4);
			}
			else if (d3 < TOL && d4 < TOL && d1 > TOL && d2 > TOL)  // Points 3 & 4 outside plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
				NewQuads.push_back(*p1);
				NewQuads.push_back(*p2);
				u = d2 / (d2 - d3);
				NewQuads.push_back(*p2 + (*p3 - *p2) * u);
				u = d1 / (d1 - d4);
				NewQuads.push_back(*p1 + (*p4 - *p1) * u);
			}
			else if (d4 < TOL && d1 < TOL && d2 > TOL && d3 > TOL)  // Points 4 & 1 outside plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
				u = d2 / (d2 - d1);
				NewQuads.push_back(*p2 + (*p1 - *p2) * u);
				NewQuads.push_back(*p2);
				NewQuads.push_back(*p3);
				u = d3 / (d3 - d4);
				NewQuads.push_back(*p3 + (*p4 - *p3) * u);

			}
			else // Convert the quad to a triangle for trimming if 1 or 3 points inside plane
			{
				itInt = Mesh.ConvertQuadtoTriangles(itStart);
			}
		}

		// Add the new quads to the mesh, and clear the new quad list
		iLastNodeIndex = int(Mesh.GetNumNodes());
		for (i = 0; i < int(NewQuads.size() / 4); ++i)
		{

			QuadIndices.push_back(4 * i + iLastNodeIndex);
			QuadIndices.push_back(4 * i + 1 + iLastNodeIndex);
			QuadIndices.push_back(4 * i + 2 + iLastNodeIndex);
			QuadIndices.push_back(4 * i + 3 + iLastNodeIndex);
		}
		for (itXYZ = NewQuads.begin(); itXYZ != NewQuads.end(); ++itXYZ)
		{
			Mesh.AddNode(*itXYZ);
		}
		NewQuads.clear();

		list<int> &TriIndices = Mesh.GetIndices(CMesh::TRI);
		for (itInt = TriIndices.begin(); itInt != TriIndices.end(); )
		{
			itStart = itInt;

			p1 = &Mesh.GetNode(*(itInt++));
			p2 = &Mesh.GetNode(*(itInt++));
			p3 = &Mesh.GetNode(*(itInt++));

			d1 = DotProduct(itPlane->Normal, *p1) - itPlane->d;
			d2 = DotProduct(itPlane->Normal, *p2) - itPlane->d;
			d3 = DotProduct(itPlane->Normal, *p3) - itPlane->d;

			if (d1 <= TOL && d2 <= TOL && d3 <= TOL) // The triangle lies completely outside or on the plane
			{
				itInt = TriIndices.erase(itStart, itInt); // Delete the triangle
			}
			else if (d1 >= -TOL && d2 >= -TOL && d3 >= -TOL) // The triangle lies completely inside the plane
			{
				// Do nothing
			}
			else
			{
				itInt = TriIndices.erase(itStart, itInt); // Delete the triangle, will need to be seperated into smaller ones
				// Order points such that d1 >= d2 >= d3
				bFlipped = false; // Keep track of whether the triangle is flipped or not
				if (d2 > d1)
				{
					dTemp = d2; d2 = d1; d1 = dTemp;
					pTemp = p2; p2 = p1; p1 = pTemp;
					bFlipped = !bFlipped;
				}
				if (d3 > d2)
				{
					dTemp = d3; d3 = d2; d2 = dTemp;
					pTemp = p3; p3 = p2; p2 = pTemp;
					bFlipped = !bFlipped;
					if (d2 > d1)
					{
						dTemp = d2; d2 = d1; d1 = dTemp;
						pTemp = p2; p2 = p1; p1 = pTemp;
						bFlipped = !bFlipped;
					}
				}

				if (d2 <= TOL) // One point of the triangle lies inside the plane, the other two are outside or on the plane
				{
					NewTriangles.push_back(*p1);
					u = d1 / (d1 - d2);
					NewTriangles.push_back(*p1 + (*p2 - *p1) * u);
					u = d1 / (d1 - d3);
					NewTriangles.push_back(*p1 + (*p3 - *p1) * u);
					NewTrianglesFlipped.push_back(bFlipped);
				}
				else if (d3 <= TOL) // Two points of the triangle lie inside the plane, the other is outside or on the plane
				{
					NewTriangles.push_back(*p1);
					NewTriangles.push_back(*p2);
					u = d2 / (d2 - d3);
					NewTriangles.push_back(*p2 + (*p3 - *p2) * u);
					NewTrianglesFlipped.push_back(bFlipped);

					NewTriangles.push_back(*p2 + (*p3 - *p2) * u);
					u = d1 / (d1 - d3);
					NewTriangles.push_back(*p1 + (*p3 - *p1) * u);
					NewTriangles.push_back(*p1);
					NewTrianglesFlipped.push_back(bFlipped);
				}
			}
		}

		// Add the new triangles to the mesh, and clear the new triangles list
		iLastNodeIndex = int(Mesh.GetNumNodes());
		for (i = 0; i < int(NewTriangles.size() / 3); ++i)
		{
			// The order of the vertices determines the direction of the normal,
			// the normal should always point outwards from the yarn. Thus if the
			// normal was flipped we must make sure the indices are swapped so
			// the normal is flipped back to its original state.
			if (!NewTrianglesFlipped[i])
			{
				TriIndices.push_back(3 * i + iLastNodeIndex);
				TriIndices.push_back(3 * i + 1 + iLastNodeIndex);
				TriIndices.push_back(3 * i + 2 + iLastNodeIndex);
			}
			else
			{
				TriIndices.push_back(3 * i + iLastNodeIndex);
				TriIndices.push_back(3 * i + 2 + iLastNodeIndex);
				TriIndices.push_back(3 * i + 1 + iLastNodeIndex);
			}
		}
		for (itXYZ = NewTriangles.begin(); itXYZ != NewTriangles.end(); ++itXYZ)
		{
			Mesh.AddNode(*itXYZ);
		}
		NewTriangles.clear();
		NewTrianglesFlipped.clear();

		vector<int> ClosedLoop;
		if (bFillGaps)
			FillGaps(Mesh, *itPlane, ClosedLoop);
	}

	// Deal with volume elements
	int iNumNodes;
	double d;
	vector<CMesh::ELEMENT_TYPE> VolumeElements;
	vector<CMesh::ELEMENT_TYPE>::iterator itElementType;
	VolumeElements.push_back(CMesh::TET);
	VolumeElements.push_back(CMesh::PYRAMID);
	VolumeElements.push_back(CMesh::WEDGE);
	VolumeElements.push_back(CMesh::HEX);
	VolumeElements.push_back(CMesh::QUADRATIC_TET);
	for (itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane)
	{
		for (itElementType = VolumeElements.begin(); itElementType != VolumeElements.end(); ++itElementType)
		{
			list<int> &Indices = Mesh.GetIndices(*itElementType);
			for (itInt = Indices.begin(); itInt != Indices.end(); )
			{
				iNumNodes = CMesh::GetNumNodes(*itElementType);
				itStart = itInt;

				XYZ Center;
				for (i = 0; i < iNumNodes; ++i)
				{
					Center += Mesh.GetNode(*(itInt++));
				}
				Center /= iNumNodes;

				d = DotProduct(itPlane->Normal, Center) - itPlane->d;
				if (d < 0)
					itInt = Indices.erase(itStart, itInt); // Delete the volume element
			}
		}
	}

	for (itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane)
	{
		list<int> &Indices = Mesh.GetIndices(CMesh::POLYGON);
		list<int>::iterator itIndices;
		int StartIndex, NewStartIndex;
		bool bResetStart = true;
		list<int>::iterator itStartIndex;
		bool bDelete = true;

		for (itIndices = Indices.begin(); itIndices != Indices.end(); )
		{
			bDelete = true;
			StartIndex = NewStartIndex = (*(itIndices));
			itStartIndex = itIndices;
			vector<int> PolygonIndex;
			XYZ p;
			bResetStart = true;
			int i = 0, iPoints = 0;
			do {
				p = Mesh.GetNode(*(itIndices));
				d = DotProduct(itPlane->Normal, p) - itPlane->d;
				if (d < TOL)
				{
					//if ( *itIndices == NewStartIndex )
						//bResetStart = false;  // Keeping 1st so don't need to reset
					//bDelete = false;
					++iPoints;
				}
				++i;
				++itIndices;
			} while ((*itIndices) != StartIndex);
			++itIndices;
			//if ( bDelete )
			if (iPoints == i) // All points outside plane
			{
				itIndices = Indices.erase(itStartIndex, itIndices);
			}
			/*if ( NewStartIndex != StartIndex ) // Must have deleted start so reset to new start value
				*(itIndices) = NewStartIndex;
			if ( NewStartIndex == StartIndex && bResetStart )  // Deleted whole loop so need to delete repeated start index
				itIndices = Indices.erase(itIndices);
			else
				itIndices++;*/
		}
	}

	Mesh.RemoveUnreferencedNodes();
}

bool CDomainPlanes::ClipMeshToDomain(CMesh &Mesh, vector<CMesh> &DomainMeshes, bool bFillGaps) const
{
	const double TOL = 1e-9;

	TGLOGINDENT("Clipping mesh to domain");

	vector<XYZ> NewTriangles;
	vector<bool> NewTrianglesFlipped;
	vector<XYZ>::iterator itXYZ;
	vector<XYZ> NewQuads;

	vector<PLANE>::const_iterator itPlane;
	list<int>::iterator itStart;
	list<int>::iterator itInt;
	const XYZ *p1, *p2, *p3, *p4;
	double d1, d2, d3, d4;	// d represents the distance of the point to the plane (i.e. +ve inside, -ve outside, 0 on top)
	double dTemp;
	const XYZ *pTemp;
	double u;
	int i;
	int iLastNodeIndex;
	bool bFlipped;
	vector<CMesh>::iterator itDomainMeshes;
	bool bSaveDomainMeshes = false;
	if (DomainMeshes.size() != 0)
		bSaveDomainMeshes = true;
	/*if( DomainMeshes.size() != 0 && DomainMeshes.size() == m_Planes.size() )  // If these aren't equal then assumption that DomainMeshes match plains isn't sound
	{
		itDomainMeshes = DomainMeshes.begin();
		bSaveDomainMeshes = true;
	}*/
	// Deal with surface elements
	for (itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane)
	{
		list<int> &QuadIndices = Mesh.GetIndices(CMesh::QUAD);
		for (itInt = QuadIndices.begin(); itInt != QuadIndices.end(); )
		{
			itStart = itInt;

			p1 = &Mesh.GetNode(*(itInt++));
			p2 = &Mesh.GetNode(*(itInt++));
			p3 = &Mesh.GetNode(*(itInt++));
			p4 = &Mesh.GetNode(*(itInt++));

			d1 = DotProduct(itPlane->Normal, *p1) - itPlane->d;
			d2 = DotProduct(itPlane->Normal, *p2) - itPlane->d;
			d3 = DotProduct(itPlane->Normal, *p3) - itPlane->d;
			d4 = DotProduct(itPlane->Normal, *p4) - itPlane->d;

			if (d1 <= TOL && d2 <= TOL && d3 <= TOL && d4 <= TOL) // The quad lies completely on or outside the plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
			}
			else if (d1 >= -TOL && d2 >= -TOL && d3 >= -TOL && d4 >= -TOL) // The quad lies completely inside the plane
			{
				// Do nothing
			}
			else if (d1 < TOL && d2 < TOL && d3 > TOL && d4 > TOL) // Points 1 & 2 outside plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
				u = d4 / (d4 - d1);
				NewQuads.push_back(*p4 + (*p1 - *p4) * u);
				u = d3 / (d3 - d2);
				NewQuads.push_back(*p3 + (*p2 - *p3) * u);
				NewQuads.push_back(*p3);
				NewQuads.push_back(*p4);
			}
			else if (d2 < TOL && d3 < TOL && d4 > TOL && d1 > TOL) // Points 2 & 3 outside plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
				NewQuads.push_back(*p1);
				u = d1 / (d1 - d2);
				NewQuads.push_back(*p1 + (*p2 - *p1) * u);
				u = d4 / (d4 - d3);
				NewQuads.push_back(*p4 + (*p3 - *p4) * u);
				NewQuads.push_back(*p4);
			}
			else if (d3 < TOL && d4 < TOL && d1 > TOL && d2 > TOL)  // Points 3 & 4 outside plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
				NewQuads.push_back(*p1);
				NewQuads.push_back(*p2);
				u = d2 / (d2 - d3);
				NewQuads.push_back(*p2 + (*p3 - *p2) * u);
				u = d1 / (d1 - d4);
				NewQuads.push_back(*p1 + (*p4 - *p1) * u);
			}
			else if (d4 < TOL && d1 < TOL && d2 > TOL && d3 > TOL)  // Points 4 & 1 outside plane
			{
				itInt = QuadIndices.erase(itStart, itInt); // Delete the quad
				u = d2 / (d2 - d1);
				NewQuads.push_back(*p2 + (*p1 - *p2) * u);
				NewQuads.push_back(*p2);
				NewQuads.push_back(*p3);
				u = d3 / (d3 - d4);
				NewQuads.push_back(*p3 + (*p4 - *p3) * u);

			}
			else // Convert the quad to a triangle for trimming if 1 or 3 points inside plane
			{
				itInt = Mesh.ConvertQuadtoTriangles(itStart);
			}
		}

		// Add the new quads to the mesh, and clear the new quad list
		iLastNodeIndex = int(Mesh.GetNumNodes());
		for (i = 0; i < int(NewQuads.size() / 4); ++i)
		{

			QuadIndices.push_back(4 * i + iLastNodeIndex);
			QuadIndices.push_back(4 * i + 1 + iLastNodeIndex);
			QuadIndices.push_back(4 * i + 2 + iLastNodeIndex);
			QuadIndices.push_back(4 * i + 3 + iLastNodeIndex);
		}
		for (itXYZ = NewQuads.begin(); itXYZ != NewQuads.end(); ++itXYZ)
		{
			Mesh.AddNode(*itXYZ);
		}
		NewQuads.clear();

		list<int> &TriIndices = Mesh.GetIndices(CMesh::TRI);
		for (itInt = TriIndices.begin(); itInt != TriIndices.end(); )
		{
			itStart = itInt;

			p1 = &Mesh.GetNode(*(itInt++));
			p2 = &Mesh.GetNode(*(itInt++));
			p3 = &Mesh.GetNode(*(itInt++));

			d1 = DotProduct(itPlane->Normal, *p1) - itPlane->d;
			d2 = DotProduct(itPlane->Normal, *p2) - itPlane->d;
			d3 = DotProduct(itPlane->Normal, *p3) - itPlane->d;

			if (d1 <= TOL && d2 <= TOL && d3 <= TOL) // The triangle lies completely outside or on the plane
			{
				itInt = TriIndices.erase(itStart, itInt); // Delete the triangle
			}
			else if (d1 >= -TOL && d2 >= -TOL && d3 >= -TOL) // The triangle lies completely inside the plane
			{
				// Do nothing
			}
			else
			{
				itInt = TriIndices.erase(itStart, itInt); // Delete the triangle, will need to be seperated into smaller ones
				// Order points such that d1 >= d2 >= d3
				bFlipped = false; // Keep track of whether the triangle is flipped or not
				if (d2 > d1)
				{
					dTemp = d2; d2 = d1; d1 = dTemp;
					pTemp = p2; p2 = p1; p1 = pTemp;
					bFlipped = !bFlipped;
				}
				if (d3 > d2)
				{
					dTemp = d3; d3 = d2; d2 = dTemp;
					pTemp = p3; p3 = p2; p2 = pTemp;
					bFlipped = !bFlipped;
					if (d2 > d1)
					{
						dTemp = d2; d2 = d1; d1 = dTemp;
						pTemp = p2; p2 = p1; p1 = pTemp;
						bFlipped = !bFlipped;
					}
				}

				if (d2 <= TOL) // One point of the triangle lies inside the plane, the other two are outside or on the plane
				{
					NewTriangles.push_back(*p1);
					u = d1 / (d1 - d2);
					NewTriangles.push_back(*p1 + (*p2 - *p1) * u);
					u = d1 / (d1 - d3);
					NewTriangles.push_back(*p1 + (*p3 - *p1) * u);
					NewTrianglesFlipped.push_back(bFlipped);
				}
				else if (d3 <= TOL) // Two points of the triangle lie inside the plane, the other is outside or on the plane
				{
					NewTriangles.push_back(*p1);
					NewTriangles.push_back(*p2);
					u = d2 / (d2 - d3);
					NewTriangles.push_back(*p2 + (*p3 - *p2) * u);
					NewTrianglesFlipped.push_back(bFlipped);

					NewTriangles.push_back(*p2 + (*p3 - *p2) * u);
					u = d1 / (d1 - d3);
					NewTriangles.push_back(*p1 + (*p3 - *p1) * u);
					NewTriangles.push_back(*p1);
					NewTrianglesFlipped.push_back(bFlipped);
				}
			}
		}

		// Add the new triangles to the mesh, and clear the new triangles list
		iLastNodeIndex = int(Mesh.GetNumNodes());
		for (i = 0; i < int(NewTriangles.size() / 3); ++i)
		{
			// The order of the vertices determines the direction of the normal,
			// the normal should always point outwards from the yarn. Thus if the
			// normal was flipped we must make sure the indices are swapped so
			// the normal is flipped back to its original state.
			if (!NewTrianglesFlipped[i])
			{
				TriIndices.push_back(3 * i + iLastNodeIndex);
				TriIndices.push_back(3 * i + 1 + iLastNodeIndex);
				TriIndices.push_back(3 * i + 2 + iLastNodeIndex);
			}
			else
			{
				TriIndices.push_back(3 * i + iLastNodeIndex);
				TriIndices.push_back(3 * i + 2 + iLastNodeIndex);
				TriIndices.push_back(3 * i + 1 + iLastNodeIndex);
			}
		}
		for (itXYZ = NewTriangles.begin(); itXYZ != NewTriangles.end(); ++itXYZ)
		{
			Mesh.AddNode(*itXYZ);
		}
		NewTriangles.clear();
		NewTrianglesFlipped.clear();

		vector<int> ClosedLoop;
		if (bFillGaps)
		{
			if (!FillGaps(Mesh, *itPlane, ClosedLoop, false))
				return false;
		}
		if (bSaveDomainMeshes)
		{
			if (ClosedLoop.size() > 0)
			{
				for (itDomainMeshes = DomainMeshes.begin(); itDomainMeshes != DomainMeshes.end(); itDomainMeshes++)
				{
					XYZ Points[3];
					XYZ Normal;
					double dPlane;
					for (int i = 0; i < 3; i++)
					{
						Points[i] = (*itDomainMeshes).GetNode(i);
					}
					Normal = CrossProduct((Points[0] - Points[1]), Points[2] - Points[1]);
					dPlane = DotProduct(Points[2], Normal);
					if (fabs(DotProduct(Mesh.GetNode(ClosedLoop[0]), Normal) - dPlane) < 0.000001)
					{
						int iIndex = (*itDomainMeshes).GetNumNodes();
						int iPolyStart = iIndex;
						vector<int>::iterator itClosedLoop;
						vector<int> Indices;
						int iStartInd = ClosedLoop[0];
						for (itClosedLoop = ClosedLoop.begin(); itClosedLoop != ClosedLoop.end(); itClosedLoop++)
						{
							if (*itClosedLoop == iStartInd && iIndex > iPolyStart)  // Back to start of loop
							{
								Indices.push_back(iPolyStart);
							}
							else
							{
								Indices.push_back(iIndex++);
								(*itDomainMeshes).AddNode(Mesh.GetNode(*itClosedLoop));
							}
						}
						if (ClosedLoop[ClosedLoop.size() - 1] != iStartInd) // Close loop if not already closed
							Indices.push_back(iPolyStart);
						(*itDomainMeshes).AddElement(CMesh::POLYGON, Indices);
						break;
					}
				}
			}
			//itDomainMeshes++;
		}
	}

	// Deal with volume elements
	int iNumNodes;
	double d;
	vector<CMesh::ELEMENT_TYPE> VolumeElements;
	vector<CMesh::ELEMENT_TYPE>::iterator itElementType;
	VolumeElements.push_back(CMesh::TET);
	VolumeElements.push_back(CMesh::PYRAMID);
	VolumeElements.push_back(CMesh::WEDGE);
	VolumeElements.push_back(CMesh::HEX);
	VolumeElements.push_back(CMesh::QUADRATIC_TET);
	for (itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane)
	{
		for (itElementType = VolumeElements.begin(); itElementType != VolumeElements.end(); ++itElementType)
		{
			list<int> &Indices = Mesh.GetIndices(*itElementType);
			for (itInt = Indices.begin(); itInt != Indices.end(); )
			{
				iNumNodes = CMesh::GetNumNodes(*itElementType);
				itStart = itInt;

				XYZ Center;
				for (i = 0; i < iNumNodes; ++i)
				{
					Center += Mesh.GetNode(*(itInt++));
				}
				Center /= iNumNodes;

				d = DotProduct(itPlane->Normal, Center) - itPlane->d;
				if (d < 0)
					itInt = Indices.erase(itStart, itInt); // Delete the volume element
			}
		}
	}

	// Deal with polygon elements
/*	for (itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane)
	{
		list<int> &Indices = Mesh.GetIndices(CMesh::POLYGON);
		list<int>::iterator itIndices;
		int StartIndex, NewStartIndex;
		bool bResetStart = true;
		list<int>::iterator itStartIndex;

		for ( itIndices = Indices.begin(); itIndices != Indices.end(); )
		{
			StartIndex = NewStartIndex = (*itIndices);
			itStartIndex = itIndices;
			vector<int> PolygonIndex;
			XYZ p;
			bResetStart = true;
			do {
				p = Mesh.GetNode(*itIndices);
				d = DotProduct(itPlane->Normal, p) - itPlane->d;
				if ( d < TOL )
				{
					//if ( *itIndices == NewStartIndex )
					//	bResetStart = true;
					itIndices = Indices.erase(itIndices);
					if ( bResetStart )
						NewStartIndex = *itIndices;
				}
				else
				{
					//if ( *itIndices == NewStartIndex )
						bResetStart = false;  // Keeping 1st so don't need to reset
					++itIndices;
				}
			} while ( (*itIndices) != StartIndex );

			if ( NewStartIndex != StartIndex ) // Must have deleted start so reset to new start value
				*(itIndices) = NewStartIndex;
			if ( NewStartIndex == StartIndex && bResetStart )  // Deleted whole loop so need to delete repeated start index
				itIndices = Indices.erase(itIndices);
			else
				itIndices++;
		}
	}*/

	// Deal with polygon elements
	for (itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane)
	{
		list<int> &Indices = Mesh.GetIndices(CMesh::POLYGON);
		list<int>::iterator itIndices;
		int StartIndex, NewStartIndex;
		bool bResetStart = true;
		list<int>::iterator itStartIndex;
		bool bDelete = true;

		for (itIndices = Indices.begin(); itIndices != Indices.end(); )
		{
			bDelete = true;
			StartIndex = NewStartIndex = (*(itIndices));
			itStartIndex = itIndices;
			vector<int> PolygonIndex;
			XYZ p;
			bResetStart = true;
			int i = 0, iPoints = 0;
			do {
				p = Mesh.GetNode(*(itIndices));
				d = DotProduct(itPlane->Normal, p) - itPlane->d;
				if (d < TOL)
				{
					//if ( *itIndices == NewStartIndex )
						//bResetStart = false;  // Keeping 1st so don't need to reset
					//bDelete = false;
					++iPoints;
				}
				++i;
				++itIndices;
			} while ((*itIndices) != StartIndex);
			++itIndices;
			//if ( bDelete )
			if (iPoints == i) // All points outside plane
			{
				itIndices = Indices.erase(itStartIndex, itIndices);
			}
			/*if ( NewStartIndex != StartIndex ) // Must have deleted start so reset to new start value
				*(itIndices) = NewStartIndex;
			if ( NewStartIndex == StartIndex && bResetStart )  // Deleted whole loop so need to delete repeated start index
				itIndices = Indices.erase(itIndices);
			else
				itIndices++;*/
		}
	}

	Mesh.RemoveUnreferencedNodes();
	return true;
}


void CDomainPlanes::BuildMesh() {
	{
		// http://paulbourke.net/geometry/pointlineplane/
		m_PlaneIntersections.clear();
		m_PlaneIntersections.resize(m_Planes.size());
		int i = 0;
		for (vector<PLANE>::const_iterator itPlane1 = m_Planes.begin(); itPlane1 != m_Planes.end(); ++itPlane1, ++i)
		{
			for (vector<PLANE>::const_iterator itPlane2 = m_Planes.begin(); itPlane2 != m_Planes.end(); itPlane2++) {
				if (itPlane1 == itPlane2) {
					continue;
				}
				double N1dotN2 = DotProduct(itPlane1->Normal, itPlane2->Normal);
				double dDeterminant = GetLengthSquared(itPlane1->Normal)*GetLengthSquared(itPlane2->Normal) - N1dotN2 * N1dotN2;
				if (abs(dDeterminant) < 1e-6) {
					continue;
				}
				double c1 = (itPlane1->d*GetLengthSquared(itPlane2->Normal) - itPlane2->d*N1dotN2) / dDeterminant;
				double c2 = (itPlane2->d*GetLengthSquared(itPlane1->Normal) - itPlane1->d*N1dotN2) / dDeterminant;

				// Points to the left of the line are considered inside when looking along the second vector
				m_PlaneIntersections[i].push_back(make_pair(c1 * itPlane1->Normal + c2 * itPlane2->Normal, CrossProduct(itPlane1->Normal, itPlane2->Normal)));
			}
		}
	}
	m_Mesh.Clear();
	for (vector<pair<XYZ, XYZ> > planeIntersect : m_PlaneIntersections) {
		for (pair<XYZ, XYZ> intersect : planeIntersect) {
			for (PLANE plane : m_Planes) {
				double dDenom = DotProduct(plane.Normal, intersect.second);
				if (abs(dDenom) < 1e-6) {
					continue;
				}
				double u = (plane.d - DotProduct(plane.Normal, intersect.first)) / dDenom;
				m_Mesh.AddNode(intersect.first + u * intersect.second);
			}
		}
	}
	for (vector<XYZ>::iterator itPoint = m_Mesh.GetNodes().begin(); itPoint != m_Mesh.GetNodes().end(); ) {
		vector<PLANE>::const_iterator itPlane;
		for (itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane) {
			double u = DotProduct(*itPoint, itPlane->Normal) - itPlane->d;
			if (u < -1e-6) {
				itPoint = m_Mesh.DeleteNode(itPoint);
				break;
			}
		}
		if (itPlane == m_Planes.end()) {
			++itPoint;
		}
	}
	m_Mesh.MeshConvexHull();
}

bool CDomainPlanes::FillGaps(CMesh &Mesh, const PLANE &Plane, vector<int> &Polygon, bool bMeshGaps)
{
	const double TOL = 1e-6;

	int i1, i2, i3, i4;
	const XYZ *p1, *p2, *p3, *p4;
	double d1, d2, d3, d4;	// d represents the distance of the point to the plane (i.e. +ve inside, -ve outside, 0 on top)

	vector<pair<int, int> > Segments;

	// Merge the nodes together and remove degenerate triangles
	Mesh.MergeNodes();
	Mesh.RemoveDegenerateTriangles();

	// Build a list of segments which lie on the plane
	list<int>::iterator itInt;
	// Check each quad to see if any of the edges lie on the plane
	list<int> &QuadIndices = Mesh.GetIndices(CMesh::QUAD);
	for (itInt = QuadIndices.begin(); itInt != QuadIndices.end(); )
	{
		i1 = *(itInt++);
		i2 = *(itInt++);
		i3 = *(itInt++);
		i4 = *(itInt++);

		p1 = &Mesh.GetNode(i1);
		p2 = &Mesh.GetNode(i2);
		p3 = &Mesh.GetNode(i3);
		p4 = &Mesh.GetNode(i4);

		d1 = DotProduct(Plane.Normal, *p1) - Plane.d;
		d2 = DotProduct(Plane.Normal, *p2) - Plane.d;
		d3 = DotProduct(Plane.Normal, *p3) - Plane.d;
		d4 = DotProduct(Plane.Normal, *p4) - Plane.d;

		d1 = abs(d1);
		d2 = abs(d2);
		d3 = abs(d3);
		d4 = abs(d4);

		// Add the segments which lie on the plane
		// The order of the segment indices is important, it tells us
		// which side is inside and which side is outside
		// Here the segments are added clockwise when viewed along the plane normal
		if (d1 <= TOL && d2 <= TOL)
		{
			Segments.push_back(pair<int, int>(i1, i2));
		}
		if (d2 <= TOL && d3 <= TOL)
		{
			Segments.push_back(pair<int, int>(i2, i3));
		}
		if (d3 <= TOL && d4 <= TOL)
		{
			Segments.push_back(pair<int, int>(i3, i4));
		}
		if (d4 <= TOL && d1 <= TOL)
		{
			Segments.push_back(pair<int, int>(i4, i1));
		}
	}
	// Check each triangle to find edges that lie on the plane
	list<int> &TriIndices = Mesh.GetIndices(CMesh::TRI);
	for (itInt = TriIndices.begin(); itInt != TriIndices.end(); )
	{
		i1 = *(itInt++);
		i2 = *(itInt++);
		i3 = *(itInt++);

		p1 = &Mesh.GetNode(i1);
		p2 = &Mesh.GetNode(i2);
		p3 = &Mesh.GetNode(i3);

		d1 = DotProduct(Plane.Normal, *p1) - Plane.d;
		d2 = DotProduct(Plane.Normal, *p2) - Plane.d;
		d3 = DotProduct(Plane.Normal, *p3) - Plane.d;

		d1 = abs(d1);
		d2 = abs(d2);
		d3 = abs(d3);

		// Add the segments which lie on the plane
		// The order of the segment indices is important, it tells us
		// which side is inside and which side is outside
		// Here the segments are added clockwise when viewed along the plane normal
		if (d1 <= TOL && d2 <= TOL)
		{
			Segments.push_back(pair<int, int>(i1, i2));
		}
		if (d2 <= TOL && d3 <= TOL)
		{
			Segments.push_back(pair<int, int>(i2, i3));
		}
		if (d3 <= TOL && d1 <= TOL)
		{
			Segments.push_back(pair<int, int>(i3, i1));
		}
	}

	vector<pair<int, int> >::iterator itSegment;

	// Find closed loops
	int iIndex;
	int iFirstIndex;
	bool bFound;

	while (Segments.size())
	{
		vector<int> ClosedLoop;
		// Start at a random index and go round counterclockwise until a full circle is done
		// Indices are added to the ClosedLoop list with each index specified only once.
		// As segments are followed they are removed from the segments list.
		iFirstIndex = iIndex = Segments.begin()->first;
		do
		{
			bFound = false;
			for (itSegment = Segments.begin(); itSegment != Segments.end(); ++itSegment)
			{
				// Follow segments counter-clockwise only
				if (itSegment->second == iIndex)
				{
					// Adjust the index to go follow the segment
					iIndex = itSegment->first;
					// Delete the segment, it is no longer needed
					itSegment = Segments.erase(itSegment);
					// Found the segment, now stop searching
					bFound = true;
					break;
				}
			}
			if (bFound)
			{
				// Add the index to the loop
				ClosedLoop.push_back(iIndex);
				// If the index is the same as the first index then a full circle
				// has been made, its time to exit the loop
				if (iFirstIndex == iIndex)
					break;
			}
		} while (bFound);

		// If a dead end was reached the bFound will be false. This means that the segments
		// do not form a fully closed loop. This can occur if nodes are not merged together
		// correctly (two nodes may occupy the same position where each one is only referenced
		// once which will cause a break in the loop).
		if (!bFound)
		{
			// Report the error
			TGERROR("Unable to fill gaps satisfactorily");
			return false;
		}

		Polygon.insert(Polygon.begin(), ClosedLoop.begin(), ClosedLoop.end());
		// Mesh the closed loop
		if (bMeshGaps)  // If just saving closed loop don't want to fill in end
			Mesh.MeshClosedLoop(Plane.Normal, ClosedLoop);
	};

	return true;

}

bool CDomainPlanes::PointInDomain(const XYZ &Point) const {
	const double TOL = 1e-9;
	for (vector<PLANE>::const_iterator itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane) {
		double d = DotProduct(itPlane->Normal, Point) - itPlane->d;
		if (d < TOL) {
			return false;
		}
	}
	return true;
}

int CDomainPlanes::GetPlane(XYZ &Normal, PLANE &Plane) {
	int i = 0;
	for (vector<PLANE>::iterator itPlane = m_Planes.begin(); itPlane != m_Planes.end(); ++itPlane, ++i) {
		if (itPlane->Normal == Normal) {
			Plane = *itPlane;
			return i;
		}
	}
	return -1;
}

void CDomainPlanes::SetPlane(int index, PLANE &Plane) {
	if (index >= m_Planes.size()) {
		return;
	}
	else {
		m_Planes[index] = Plane;
		BuildMesh();
	}
}
