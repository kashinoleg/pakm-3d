#include "CppUnitTest.h"
#include "OctreeDimensions.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hxa;

namespace UnitTest
{
	TEST_CLASS(OctreeDimensionsTest)
	{
	public:
		TEST_METHOD(OctreeDimensionsZeroTest)
		{
			// arrange
			OctreeDimensions dimension;

			// act
			dimension = OctreeDimensions();

			// assert
			Assert::AreEqual(dimension.getPosition().x_m, 0.0f);
			Assert::AreEqual(dimension.getPosition().y_m, 0.0f);
			Assert::AreEqual(dimension.getPosition().z_m, 0.0f);

			Assert::AreEqual(dimension.getSize(), 0.0f);
			Assert::AreEqual(dimension.getMaxItemCountPerCell(), 0);
		}
		TEST_METHOD(OctreeDimensionsOperatorSetTest)
		{
			// arrange
			OctreeDimensions dimensionA;
			OctreeDimensions dimensionB;
			OctreeVector position = OctreeVector(1.0f, 2.0f, 3.0f);

			// act
			dimensionA.setPosition(position);
			position.setX(4.0f);
			position.setY(5.0f);
			position.setZ(6.0f);

			dimensionA.setSize(10);
			dimensionB.setSize(-10);

			dimensionA.setMaxItemCountPerCell(10);
			dimensionB.setMaxItemCountPerCell(-10);

			dimensionA.setMaxLevel(10);
			dimensionB.setMaxLevel(-10);

			// assert
			Assert::AreEqual(dimensionA.getPosition().x_m, 1.0f);
			Assert::AreEqual(dimensionA.getPosition().y_m, 2.0f);
			Assert::AreEqual(dimensionA.getPosition().z_m, 3.0f);

			Assert::AreEqual(dimensionA.getSize(), 10.0f);
			Assert::AreEqual(dimensionB.getSize(), 10.0f);

			Assert::AreEqual(dimensionA.getMaxItemCountPerCell(), 10);
			Assert::AreEqual(dimensionB.getMaxItemCountPerCell(), 1);

			Assert::AreEqual(dimensionA.getMaxLevel(), 9);
			Assert::AreEqual(dimensionB.getMaxLevel(), 0);
		}
		TEST_METHOD(OctreeDimensionsIsSubdivideTest)
		{
			// arrange
			OctreeDimensions dimensionA;

			// act
			dimensionA.setMaxItemCountPerCell(2);
			dimensionA.setMaxLevel(5);//4

			// assert
			Assert::AreEqual(dimensionA.isSubdivide(4, 3), true);
			Assert::AreEqual(dimensionA.isSubdivide(4, 4), false);
			Assert::AreEqual(dimensionA.isSubdivide(1, 3), false);
			Assert::AreEqual(dimensionA.isSubdivide(1, 4), false);
		}
	};
}
