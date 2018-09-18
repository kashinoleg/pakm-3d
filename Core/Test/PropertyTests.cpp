#include "CppUnitTest.h"
#include "TextileFactory.h"
#include <string>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(CPropertyTests)
	{
		CTextileFactory m_TextileFactory;
	public:
		/*void CPropertyTests::tearDown()
		{
			TEXGEN.DeleteTextiles();
		}//*/

		TEST_METHOD(TestArealDensityUnits)
		{
			CTextile Textile;
			Textile.SetArealDensity(1, "kg/mm^2");
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(1e6, Textile.GetArealDensity(), 0);
			Textile.SetArealDensity(23, "g/m^2");
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(23e-3, Textile.GetArealDensity(), 0);
			Textile.SetArealDensity(456, "g/mm^2");
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(456e3, Textile.GetArealDensity(), 0);
			Textile.SetArealDensity(1);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.09290, Textile.GetArealDensity("kg/ft^2"), 0.0001);
		}

		TEST_METHOD(TestGeometryScale)
		{
			CTextile Textile;
			//CPPUNIT_ASSERT(Textile.SetGeometryScale("yd"));
			//CPPUNIT_ASSERT(Textile.SetGeometryScale("mm"));
			//CPPUNIT_ASSERT(Textile.SetGeometryScale("in"));
			//CPPUNIT_ASSERT(!Textile.SetGeometryScale("m/s"));
			//CPPUNIT_ASSERT(!Textile.SetGeometryScale("g/m"));
			//CPPUNIT_ASSERT(!Textile.SetGeometryScale("shjdaihsdad"));
		}

		TEST_METHOD(TestLinearDensityUnits)
		{
			CTextile Textile;
			Textile.SetYarnLinearDensity(1, "tex");
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(1e-6, Textile.GetYarnLinearDensity("kg/m"), 1e-12);
			Textile.SetYarnLinearDensity(1, "denier");
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(1.111111e-7, Textile.GetYarnLinearDensity("kg/m"), 1e-12);
			Textile.SetYarnLinearDensity(1);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(9e6, Textile.GetYarnLinearDensity("denier"), 1);
		}

		TEST_METHOD(TestVolumeFraction)
		{
			CTextileWeave2D Textile = m_TextileFactory.CottonWeave();
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(160, Textile.GetArealDensity("g/m^2"), 0.0001);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.085563, Textile.GetYarnVolume("mm^3"), 0.0001);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.18005719647, Textile.GetYarnVolumePerUnitArea("mm^3/mm^2"), 0.0001);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.103896103896, Textile.GetFibreVolumePerUnitArea("mm^3/mm^2"), 0.0001);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.577017225266, Textile.GetFibreYarnVolumeFraction(), 0.001);
		}

		TEST_METHOD(TestVolumeFraction2)
		{
			// Create a textile
			CTextile Textile = m_TextileFactory.StraightYarns();

			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.12732, Textile.GetYarns()[0].GetFibreYarnVolumeFraction(), 0.001);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25465, Textile.GetYarns()[1].GetFibreYarnVolumeFraction(), 0.001);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.190985, Textile.GetFibreYarnVolumeFraction(), 0.001);
		}

		TEST_METHOD(TestVolumeFraction3)
		{
			// Create a textile
			CTextile Textile = m_TextileFactory.StraightYarns();

			vector<XYZ> Points;
			// This point should be inside the first yarn
			Points.push_back(XYZ(5, 0, 0));
			// This point should be inside the second yarn
			Points.push_back(XYZ(5, 5, 0));
			vector<POINT_INFO> PointsInfo;
			Textile.GetPointInformation(Points, PointsInfo);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.12732, PointsInfo[0].dVolumeFraction, 0.001);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25465, PointsInfo[1].dVolumeFraction, 0.001);
		}

		TEST_METHOD(TestVolumeFraction4)
		{
			// Create a textile
			CTextile Textile = m_TextileFactory.StraightYarns();

			int i;
			for (i = 0; i<Textile.GetNumYarns(); ++i)
			{
				Textile.GetYarn(i)->ResetYarnLinearDensity();
				Textile.GetYarn(i)->ResetFibreDensity();
			}

			Textile.SetFibresPerYarn(100);
			Textile.SetFibreDiameter(20, "um");

			vector<XYZ> Points;
			// This point should be inside the first yarn
			Points.push_back(XYZ(5, 0, 0));
			// This point should be inside the second yarn
			Points.push_back(XYZ(5, 5, 0));
			vector<POINT_INFO> PointsInfo;
			Textile.GetPointInformation(Points, PointsInfo);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.04, PointsInfo[0].dVolumeFraction, 0.001);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(0.04, PointsInfo[1].dVolumeFraction, 0.001);
		}
	};
}
