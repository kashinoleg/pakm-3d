#include "stdafx.h"
#include "SectionMeshTriangulate.h"
#include "Section.h"
#include "triangulate.h"

using namespace trg;
using namespace TexGen;

CSectionMeshTriangulate::CSectionMeshTriangulate(double dMinAngle, double dMaxArea)
{
	m_dMinAngle = dMinAngle;
	m_dMaxArea = dMaxArea;
}

CSectionMeshTriangulate::CSectionMeshTriangulate(TiXmlElement &Element)
{
	Element.Attribute("MinAngle", &m_dMinAngle);
	Element.Attribute("MaxArea", &m_dMaxArea);
}

void CSectionMeshTriangulate::PopulateTiXmlElement(TiXmlElement &Element, OUTPUT_TYPE OutputType) const
{
	CSectionMesh::PopulateTiXmlElement(Element, OutputType);
	Element.SetAttribute("MinAngle", stringify(m_dMinAngle));
	Element.SetAttribute("MaxArea", stringify(m_dMaxArea));
}

bool CSectionMeshTriangulate::CreateMesh(const vector<XY> &Section) const
{
	char szSwitches[128];
	double dSectionArea = CSection::GetArea(Section);
	double dMaxArea = dSectionArea*m_dMaxArea;
	sprintf_s(szSwitches, "pzQPBq%fa%f", m_dMinAngle, dMaxArea);
	CTriangulateIO TriangleInput;
	CTriangulateIO TriangleOutput;
	memset(&TriangleInput, 0, sizeof(TriangleInput));
	memset(&TriangleOutput, 0, sizeof(TriangleOutput));

	TriangleInput.pointlist = new double[Section.size()*2];
	TriangleInput.numberofpoints = (int)Section.size();

	for (size_t i = 0; i < Section.size(); i++)
	{
		TriangleInput.pointlist[i*2] = Section[i].x;
		TriangleInput.pointlist[i*2+1] = Section[i].y;
	}

	TriangleInput.segmentlist = new int [Section.size()*2];
	TriangleInput.numberofsegments = (int)Section.size();

	for (size_t i = 0; i < Section.size(); i++)
	{
		TriangleInput.segmentlist[i*2] = i;
		TriangleInput.segmentlist[i*2+1] = (i+1)%Section.size();
	}

	CTriangulate::triangulate(szSwitches, &TriangleInput, &TriangleOutput, NULL);

	delete [] TriangleInput.pointlist;
	delete [] TriangleInput.segmentlist;

	m_Mesh.Clear();

	XYZ Point;
	for (int i = 0; i < TriangleOutput.numberofpoints; i++)
	{
		Point.x = TriangleOutput.pointlist[i*2];
		Point.y = TriangleOutput.pointlist[i*2+1];
		m_Mesh.AddNode(Point);
	}

	for (int i = 0; i < TriangleOutput.numberoftriangles; i++)
	{
		m_Mesh.GetIndices(CMesh::TRI).push_back(TriangleOutput.trianglelist[i*3]);
		m_Mesh.GetIndices(CMesh::TRI).push_back(TriangleOutput.trianglelist[i*3+1]);
		m_Mesh.GetIndices(CMesh::TRI).push_back(TriangleOutput.trianglelist[i*3+2]);
	}

	CTriangulate::trifree(TriangleOutput.pointlist);
	CTriangulate::trifree(TriangleOutput.trianglelist);
	return true;
}

CMesh CSectionMeshTriangulate::GetSimpleMesh(const vector<XY> &Section)
{
	CMesh Mesh;
	vector<int> ClosedLoop;
	int i = 0;
	for (vector<XY>::const_iterator itPoint = Section.begin(); itPoint != Section.end(); ++itPoint)
	{
		Mesh.AddNode(XYZ(itPoint->x, itPoint->y, 0));
		ClosedLoop.push_back(i);
		i++;
	}
	Mesh.MeshClosedLoop(XYZ(0, 0, -1), ClosedLoop);
	return Mesh;
}
