#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#include <Logic/Logic.hpp>
namespace GVE = GreatVEngine;


namespace RotateMatrix
{
	TEST_CLASS(UnitTest1)
	{
	public:
		const GVE::Float32 e = 0.0001f;
	public:
		TEST_METHOD(RotationTest)
		{
			GVE::Helper::Transformation::Dimension3::RotateMatrix rm(GVE::Vec3(0.0f, 0.0f, 0.0f));

			Assert::IsTrue(
				GVE::distance(rm.GetRMat() * GVE::Vec3(0.0f, 0.0f, 1.0f), GVE::Vec3(0.0f, 0.0f, 1.0f)) < e,
				L"Invalid rotation");

			rm.SetAngle(GVE::Vec3(90.0f, 0.0f, 0.0f));
			Assert::IsTrue(
				rm.GetAngle() == GVE::Vec3(90.0f, 0.0f, 0.0f),
				L"Invalid angle X");
			Assert::IsTrue(
				GVE::distance(rm.GetRMat() * GVE::Vec3(0.0f, 1.0f, 0.0f), GVE::Vec3(0.0f, 0.0f, 1.0f)) < e,
				L"Invalid rotation X");

			rm.SetAngle(GVE::Vec3(0.0f, 90.0f, 0.0f));
			Assert::IsTrue(
				rm.GetAngle() == GVE::Vec3(0.0f, 90.0f, 0.0f),
				L"Invalid angle Y");
			Assert::IsTrue(
				GVE::distance(rm.GetRMat() * GVE::Vec3(0.0f, 0.0f, 1.0f), GVE::Vec3(1.0f, 0.0f, 0.0f)) < e,
				L"Invalid rotation Y");

			rm.SetAngle(GVE::Vec3(0.0f, 0.0f, 90.0f));
			Assert::IsTrue(
				rm.GetAngle() == GVE::Vec3(0.0f, 0.0f, 90.0f),
				L"Invalid angle X");
			Assert::IsTrue(
				GVE::distance(rm.GetRMat() * GVE::Vec3(0.0f, 1.0f, 0.0f), GVE::Vec3(1.0f, 0.0f, 0.0f)) < e,
				L"Invalid rotation Z");
		}
	};
}