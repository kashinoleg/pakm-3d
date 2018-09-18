#include "CppUnitTest.h"
#include "TextileFactory.h"
#include "TestUtilities.h"
#include <string>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(CXMLTests)
	{
		CTextileFactory m_TextileFactory;
	public:
		/*void CXMLTests::tearDown()
		{
			TEXGEN.DeleteTextiles();
		}//*/

		/*bool TestOutput(string Prefix, OUTPUT_TYPE Type)
		{
			// Set up some file names
			string OriginalFile = Prefix + "1.tg3";
			string CompareFile = Prefix + "2.tg3";

			// Save to xml file
			TEXGEN.SaveToXML(OriginalFile, "", Type);
			TEXGEN.DeleteTextiles();

			// Read it back in
			TEXGEN.ReadFromXML(OriginalFile);

			// Save it back to disk again
			TEXGEN.SaveToXML(CompareFile, "", Type);

			// Check the two files are the same
			return CompareFiles(OriginalFile, CompareFile);
		}

		TEST_METHOD(TestTextile2DWeaveMin)
		{
			TEXGEN.AddTextile(m_TextileFactory.SatinWeave());
			//CPPUNIT_ASSERT(TestOutput("2dweavemin", OUTPUT_MINIMAL));
		}

		TEST_METHOD(TestTextile2DWeaveStandard)
		{
			TEXGEN.AddTextile(m_TextileFactory.SatinWeave());
			//CPPUNIT_ASSERT(TestOutput("2dweavestan", OUTPUT_STANDARD));
		}//*/

		/*TEST_METHOD(TestTextile2DWeaveFull)
		{
			TEXGEN.AddTextile(m_TextileFactory.SatinWeave());
			//CPPUNIT_ASSERT(TestOutput("2dweavefull", OUTPUT_FULL));
		}

		TEST_METHOD(TestTextile3DWeaveMin)
		{
			TEXGEN.AddTextile(m_TextileFactory.Weave3D8x4());
			//CPPUNIT_ASSERT(TestOutput("3dweavemin", OUTPUT_MINIMAL));
		}

		TEST_METHOD(TestTextile3DWeaveStandard)
		{
			TEXGEN.AddTextile(m_TextileFactory.Weave3D8x4());
			//CPPUNIT_ASSERT(TestOutput("3dweavestan", OUTPUT_STANDARD));
		}

		TEST_METHOD(TestTextile3DWeaveFull)
		{
			TEXGEN.AddTextile(m_TextileFactory.Weave3D8x4());
			//CPPUNIT_ASSERT(TestOutput("3dweavefull", OUTPUT_FULL));
		}//not used

		TEST_METHOD(TestYarnMin)
		{
			TEXGEN.AddTextile(m_TextileFactory.InterpTest());
			//CPPUNIT_ASSERT(TestOutput("yarnmin", OUTPUT_MINIMAL));
		}

		TEST_METHOD(TestYarnStandard)
		{
			TEXGEN.AddTextile(m_TextileFactory.InterpTest());
			//CPPUNIT_ASSERT(TestOutput("yarnstan", OUTPUT_STANDARD));
		}

		TEST_METHOD(TestYarnFull)
		{
			TEXGEN.AddTextile(m_TextileFactory.InterpTest());
			//CPPUNIT_ASSERT(TestOutput("yarnfull", OUTPUT_FULL));
		}

		TEST_METHOD(TestSectionsMin)
		{
			TEXGEN.AddTextile(m_TextileFactory.SectionsTest());
			//CPPUNIT_ASSERT(TestOutput("sectionsmin", OUTPUT_MINIMAL));
		}

		TEST_METHOD(TestSectionsStandard)
		{
			TEXGEN.AddTextile(m_TextileFactory.SectionsTest());
			//CPPUNIT_ASSERT(TestOutput("sectionsstan", OUTPUT_STANDARD));
		}

		TEST_METHOD(TestSectionsFull)
		{
			TEXGEN.AddTextile(m_TextileFactory.SectionsTest());
			//CPPUNIT_ASSERT(TestOutput("sectionsfull", OUTPUT_FULL));
		}

		TEST_METHOD(TestDomain)
		{
			CTextile Textile;
			CDomainPlanes Domain(XYZ(0, 0, 0), XYZ(1, 2, 3));
			Textile.AssignDomain(Domain);
			TEXGEN.AddTextile(Textile);
			//CPPUNIT_ASSERT(TestOutput("domain", OUTPUT_FULL));
		}//*/
		/*
		void CXMLTests::TestMeshing()
		{
		CTextileWeave2D Text = m_TextileFactory.MeshedWeave();
		Text.SetResolution(8);
		//	Text.OutputAbaqus("test.inp");
		//	TEXGEN.AddTextile(m_TextileFactory.MeshedWeave());
		//	CPPUNIT_ASSERT(TestOutput("meshedweave", OUTPUT_FULL));
		}*/
	};
}
