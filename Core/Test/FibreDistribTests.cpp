#include "CppUnitTest.h"
#include "TextileFactory.h"
#include <string>
using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(CFibreDistribTests)
	{
		CTextileFactory m_TextileFactory;
	public:
		TEST_METHOD(TestInside)
		{
			CSectionEllipse Ellipse(2, 2);
			//CPPUNIT_ASSERT(PointInsideSection(Ellipse.GetPoints(100), XY(0, 0)));
		}

		TEST_METHOD(TestOutside)
		{
			CSectionEllipse Ellipse(2, 2);
			//CPPUNIT_ASSERT(!PointInsideSection(Ellipse.GetPoints(100), XY(1, 1)));
		}

		TEST_METHOD(TestConst)
		{
			CFibreDistributionConst FibreDist;
			CSectionEllipse Ellipse(4, 1);
			double dAverageVF = GetAverageVF(FibreDist, Ellipse);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 / PI, dAverageVF, 1e-3);
		}

		TEST_METHOD(TestQuad)
		{
			CFibreDistribution1DQuad FibreDist(0.5);
			CSectionEllipse Ellipse(4, 1);
			double dAverageVF = GetAverageVF(FibreDist, Ellipse);
			//CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 / PI, dAverageVF, 1e-3);
		}

		/*void CFibreDistribTests::tearDown()
		{
			TEXGEN.DeleteTextiles();
		}//*/

		double GetAverageVF(CFibreDistribution &FibreDistrib, CSection &Section)
		{
			int i, j;
			int iSizeX = 100, iSizeY = 100;
			const vector<XY> &Points = Section.GetPoints(100);
			XY Min, Max;
			GetBoundingBox(Points, Min, Max);
			XY Point;
			double dVolFrac = 0;
			int iCount = 0;
			for (i = 0; i<iSizeX; ++i)
			{
				Point.x = Min.x + (Max.x - Min.x) * i / double(iSizeX - 1);
				for (j = 0; j<iSizeY; ++j)
				{
					Point.y = Min.y + (Max.y - Min.y) * j / double(iSizeY - 1);
					if (PointInsideSection(Points, Point))
					{
						dVolFrac += FibreDistrib.GetVolumeFraction(Points, 1.0, Point);
						++iCount;
					}
				}
			}
			if (iCount > 0)
				return dVolFrac / iCount;
			else
				return -1;
		}

		void GetBoundingBox(const vector<XY> &Points, XY &Min, XY &Max)
		{
			Min = Max = XY();
			int i;
			for (i = 0; i<(int)Points.size(); ++i)
			{
				Min = ::Min(Points[i], Min);
				Max = ::Max(Points[i], Max);
			}
		}

		bool PointInsideSection(const vector<XY> &Points, XY Point)
		{
			int i;
			for (i = 0; i<(int)Points.size(); ++i)
			{
				XY A, B;
				A = Points[(i + 1) % Points.size()] - Points[i];
				B = Point - Points[i];
				if ((A.x*B.y - A.y*B.x) < 0)
					return false;
			}
			return true;
		}
	};
}
