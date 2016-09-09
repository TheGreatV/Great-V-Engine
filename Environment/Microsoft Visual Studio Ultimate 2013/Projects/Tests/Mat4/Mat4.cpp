#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#include <Logic/Mathematics.hpp>
namespace GVE = GreatVEngine;


namespace Mat4
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		const GVE::Float32 e = 0.0001f;
	public:
		TEST_METHOD(RotationDirectionTest)
		{
			Assert::IsTrue(
				GVE::distance(GVE::RotateX4(90.0f) * GVE::Vec4(0.0f, 1.0f, 0.0f, 0.0f), GVE::Vec4(0.0f, 0.0f, 1.0f, 0.0f)) < e,
				L"Wrong X rotation");
			Assert::IsTrue(
				GVE::distance(GVE::RotateX4(-90.0f) * GVE::Vec4(0.0f, 1.0f, 0.0f, 0.0f), GVE::Vec4(0.0f, 0.0f, -1.0f, 0.0f)) < e,
				L"Wrong X rotation");

			Assert::IsTrue(
				GVE::distance(GVE::RotateY4(90.0f) * GVE::Vec4(0.0f, 0.0f, 1.0f, 0.0f), GVE::Vec4(1.0f, 0.0f, 0.0f, 0.0f)) < e,
				L"Wrong Y rotation");
			Assert::IsTrue(
				GVE::distance(GVE::RotateY4(-90.0f) * GVE::Vec4(0.0f, 0.0f, 1.0f, 0.0f), GVE::Vec4(-1.0f, 0.0f, 0.0f, 0.0f)) < e,
				L"Wrong Y rotation");

			Assert::IsTrue(
				GVE::distance(GVE::RotateZ4(90.0f) * GVE::Vec4(1.0f, 0.0f, 0.0f, 0.0f), GVE::Vec4(0.0f, -1.0f, 0.0f, 0.0f)) < e,
				L"Wrong Z rotation");
			Assert::IsTrue(
				GVE::distance(GVE::RotateZ4(-90.0f) * GVE::Vec4(1.0f, 0.0f, 0.0f, 0.0f), GVE::Vec4(0.0f, 1.0f, 0.0f, 0.0f)) < e,
				L"Wrong Z rotation");
		}
		TEST_METHOD(MakeFromMat3Test)
		{
			Assert::IsTrue(
				GVE::distance(GVE::To4(GVE::Mat3(0, 1, 2, 3, 4, 5, 6, 7, 8)) * GVE::Vec4(1, 2, 3, 4), GVE::Mat4(0, 1, 2, 0, 3, 4, 5, 0, 6, 7, 8, 0, 0, 0, 0, 1) * GVE::Vec4(1, 2, 3, 4)) < e,
				L"Mat3 to Mat4 invalid");
		}
		TEST_METHOD(SequentialTransformationTest)
		{
			auto m1 = GVE::RotateY4(90.0f);
			auto m2 = GVE::Move4(GVE::Vec3(0.0f, 0.0f, 1.0f));

			auto v = GVE::Vec4(1.0f, 0.0f, 0.0f, 1.0f);

			Assert::IsTrue(
				GVE::distance(m2 * (m1 * v), (m2 * m1) * v) < e,
				L"Invalid multiple order");

			Assert::IsTrue(
				GVE::distance((m2 * m1) * v, GVE::Vec4(0.0f, 0.0f, 0.0f, 1.0f)) < e,
				L"Invalid multiple order");
		}
	};
}