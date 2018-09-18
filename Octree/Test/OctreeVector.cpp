#include "CppUnitTest.h"
#include "OctreeVector.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hxa;

namespace UnitTest
{
	TEST_CLASS(OctreeVectorTest)
	{
	public:
		TEST_METHOD(OctreeVectorZeroTest)
		{
			// arrange
			OctreeVector vector;
			
			// act
			vector = OctreeVector();

			// assert
			Assert::AreEqual(vector.x_m, 0.0f);
			Assert::AreEqual(vector.y_m, 0.0f);
			Assert::AreEqual(vector.z_m, 0.0f);
		}

		TEST_METHOD(OctreeVectorThreeFloatTest)
		{
			// arrange
			OctreeVector vector(1.0f, 2.0f, 3.0f);

			// assert
			Assert::AreEqual(vector.x_m, 1.0f);
			Assert::AreEqual(vector.y_m, 2.0f);
			Assert::AreEqual(vector.z_m, 3.0f);
		}

		TEST_METHOD(OctreeVectorCopyTest)
		{
			// arrange
			OctreeVector vectorA(1.0f, 2.0f, 3.0f);
			OctreeVector vectorB(vectorA);

			// act
			vectorA.x_m = 4.0f;
			vectorA.y_m = 5.0f;
			vectorA.z_m = 6.0f;

			// assert
			Assert::AreEqual(vectorB.x_m, 1.0f);
			Assert::AreEqual(vectorB.y_m, 2.0f);
			Assert::AreEqual(vectorB.z_m, 3.0f);
		}

		TEST_METHOD(OctreeVectorOperatorEquallyTest)
		{
			// arrange
			OctreeVector vectorA(1.0f, 2.0f, 3.0f);
			OctreeVector vectorB;

			// act
			vectorB = vectorA;
			vectorA.x_m = 4.0f;
			vectorA.y_m = 5.0f;
			vectorA.z_m = 6.0f;

			// assert
			Assert::AreEqual(vectorB.x_m, 1.0f);
			Assert::AreEqual(vectorB.y_m, 2.0f);
			Assert::AreEqual(vectorB.z_m, 3.0f);
		}

		TEST_METHOD(OctreeVectorOperatorSetTest)
		{
			// arrange
			OctreeVector vectorA(1.0f, 2.0f, 3.0f);

			// act
			vectorA.setX(4.0f);
			vectorA.setY(5.0f);
			vectorA.setZ(6.0f);

			// assert
			Assert::AreEqual(vectorA.x_m, 4.0f);
			Assert::AreEqual(vectorA.y_m, 5.0f);
			Assert::AreEqual(vectorA.z_m, 6.0f);
		}

		TEST_METHOD(OctreeVectorOperatorGetTest)
		{
			// arrange
			OctreeVector vectorA(1.0f, 2.0f, 3.0f);

			// act
			float x = vectorA.getX();
			float y = vectorA.getY();
			float z = vectorA.getZ();

			x = 4.0f;
			y = 5.0f;
			z = 6.0f;

			// assert
			Assert::AreEqual(vectorA.x_m, 1.0f);
			Assert::AreEqual(vectorA.y_m, 2.0f);
			Assert::AreEqual(vectorA.z_m, 3.0f);
		}

		TEST_METHOD(OctreeVectorOperatorLengthTest)
		{
			// arrange
			OctreeVector vectorA(2.0f, 4.0f, 4.0f);
			OctreeVector vectorB(4.0f, 2.0f, 4.0f);
			OctreeVector vectorC(4.0f, 4.0f, 2.0f);

			// act
			float lengthA = vectorA.length();
			float lengthB = vectorB.length();
			float lengthC = vectorC.length();

			// assert
			Assert::AreEqual(lengthA, 6.0f);
			Assert::AreEqual(lengthB, 6.0f);
			Assert::AreEqual(lengthC, 6.0f);
		}
	};
}
