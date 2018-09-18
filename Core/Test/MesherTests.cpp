#include "CppUnitTest.h"
#include "TextileFactory.h"
#include <string>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(CMesherTests)
	{
		CTextileFactory m_TextileFactory;
	public:
		TEST_METHOD(TestSimpleMesh)
		{
			CTextileWeave2D Textile = m_TextileFactory.CottonWeave();
			Textile.SetFibreArea(0.01, "mm^2");
			//	Textile.SetGapSize(0.02);

			CMesher Mesher;
			//	Mesher.SetHybrid(true);
			//	Mesher.SetQuadratic(true);
			Mesher.SetPeriodic(true);
			Mesher.SetSeed(0.05);
			Mesher.SetMergeTolerance(0.02);
			//CPPUNIT_ASSERT(Mesher.CreateMesh(Textile));
			/*	CMesh Mesh = Mesher.GetMesh();
			Mesh.ConvertToSurfaceMesh();
			Mesh.SaveToVTK("mesh-validated");
			CMesh SurfaceMesh;
			Textile.AddSurfaceToMesh(SurfaceMesh, true);
			SurfaceMesh.SaveToVTK("mesh-surface");*/
			Mesher.SaveVolumeMeshToVTK("vmesh.vtu");
			Mesher.SaveVolumeMeshToABAQUS("vmesh.inp", Textile.GetName());
		}

		TEST_METHOD(TestInvertedElements)
		{
			CTextileWeave2D Textile = m_TextileFactory.PlainWeave();

			CMesher Mesher;
			Mesher.SetHybrid(true);
			Mesher.SetSeed(0.1);
			Mesher.SetMergeTolerance(0.1);
			Mesher.CreateMesh(Textile);
			//CPPUNIT_ASSERT_EQUAL_MESSAGE("Some tetrahedral elements are inverted", 0, Mesher.GetMesh().CountInvertedElements(CMesh::TET));
			//CPPUNIT_ASSERT_EQUAL_MESSAGE("Some pyramid elements are inverted", 0, Mesher.GetMesh().CountInvertedElements(CMesh::PYRAMID));
			//CPPUNIT_ASSERT_EQUAL_MESSAGE("Some wedge elements are inverted", 0, Mesher.GetMesh().CountInvertedElements(CMesh::WEDGE));
		}//*/not used

		/*void CMesherTests::TestMatchingFaces()
		{
		CTextileWeave2D Textile = m_TextileFactory.PlainWeave();
		CMesher Mesher;
		Mesher.SetHybrid(true);
		Mesher.SetSeed(0.1);
		Mesher.SetMergeTolerance(0.1);
		Mesher.CreateMesh(Textile);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Some faces are not matched up correctly", 0, Mesher.GetMesh().CountMismatchedFaces());
		}not used//*/

		/*void CMesherTests::tearDown()
		{
			TEXGEN.DeleteTextiles();
		}//*/
	};
}
