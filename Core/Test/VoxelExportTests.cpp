#include "CppUnitTest.h"
#include "TextileFactory.h"
#include "TestUtilities.h"
#include <string>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(CVoxelExportTests)
	{
		CTextileFactory m_TextileFactory;
	public:
		/*void CVoxelExportTests::tearDown()
		{
			TEXGEN.DeleteTextiles();
		}//*/

		TEST_METHOD(TestContinuumExport)
		{
			CTextile Textile = m_TextileFactory.GetSingleYarn(3, 20);
			CRectangularVoxelMesh Vox("CPeriodicBoundaries");
			// Save yarns and matrix
			Vox.SaveVoxelMesh(Textile, "VoxelContinuumTest", 10, 10, 10, true, true, MATERIAL_CONTINUUM);
			// Compare to template file
			//CPPUNIT_ASSERT(CompareFiles("VoxelContinuumTest.inp", "..\\..\\UnitTests\\VoxelContinuumTest.inp"));
		}
	};
}
