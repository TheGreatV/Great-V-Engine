#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#include <Logic/Mathematics.hpp>
namespace GVE = GreatVEngine;


namespace Mat3
{
	TEST_CLASS(UnitTest1)
	{
	public:
		const GVE::Float32 e = 0.0001f;
	public:

		TEST_METHOD(RotationDirectionTest)
		{
			Assert::IsTrue(
				GVE::distance(GVE::RotateX3(90.0f) * GVE::Vec3(0.0f, 1.0f, 0.0f), GVE::Vec3(0.0f, 0.0f, 1.0f)) < e,
				L"Wrong X rotation");
			Assert::IsTrue(
				GVE::distance(GVE::RotateX3(-90.0f) * GVE::Vec3(0.0f, 1.0f, 0.0f), GVE::Vec3(0.0f, 0.0f, -1.0f)) < e,
				L"Wrong X rotation");

			Assert::IsTrue(
				GVE::distance(GVE::RotateY3(90.0f) * GVE::Vec3(0.0f, 0.0f, 1.0f), GVE::Vec3(1.0f, 0.0f, 0.0f)) < e,
				L"Wrong Y rotation");
			Assert::IsTrue(
				GVE::distance(GVE::RotateY3(-90.0f) * GVE::Vec3(0.0f, 0.0f, 1.0f), GVE::Vec3(-1.0f, 0.0f, 0.0f)) < e,
				L"Wrong Y rotation");

			Assert::IsTrue(
				GVE::distance(GVE::RotateZ3(90.0f) * GVE::Vec3(1.0f, 0.0f, 0.0f), GVE::Vec3(0.0f, -1.0f, 0.0f)) < e,
				L"Wrong Z rotation");
			Assert::IsTrue(
				GVE::distance(GVE::RotateZ3(-90.0f) * GVE::Vec3(1.0f, 0.0f, 0.0f), GVE::Vec3(0.0f, 1.0f, 0.0f)) < e,
				L"Wrong Z rotation");
		}

	};
}