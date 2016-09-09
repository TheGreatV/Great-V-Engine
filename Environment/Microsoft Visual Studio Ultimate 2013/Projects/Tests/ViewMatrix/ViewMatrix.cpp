#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#include <Logic/Logic.hpp>
namespace GVE = GreatVEngine;


namespace ViewMatrix
{
	TEST_CLASS(UnitTest1)
	{
	public:
		const GVE::Float32 e = 0.0001f;
	public:
		TEST_METHOD(IdentityTest)
		{
			GVE::Helper::Transformation::Dimension3::ViewMatrix vm(GVE::Vec3(0.0f), GVE::Vec3(0.0f));

			Assert::IsTrue(
				GVE::distance(vm.GetRMat() * GVE::Vec3(0.0f, 0.0f, 1.0f), GVE::Vec3(0.0f, 0.0f, 1.0f)) < e,
				L"Invalid rotation");

			Assert::IsTrue(
				GVE::distance(vm.GetVMat() * GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f), GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f)) < e,
				L"Invalid position");
		}
		TEST_METHOD(RotationTest)
		{
			GVE::Helper::Transformation::Dimension3::ViewMatrix vm(GVE::Vec3(0.0f), GVE::Vec3(0.0f));

			vm.SetAngle(GVE::Vec3(90.0f, 0.0f, 0.0f));
			Assert::IsTrue(
				vm.GetAngle() == GVE::Vec3(90.0f, 0.0f, 0.0f),
				L"Invalid angle X");
			Assert::IsTrue(
				GVE::distance(vm.GetRMat() * GVE::Vec3(0.0f, 1.0f, 0.0f), GVE::Vec3(0.0f, 0.0f, 1.0f)) < e,
				L"Invalid rotation X");

			vm.SetAngle(GVE::Vec3(0.0f, 90.0f, 0.0f));
			Assert::IsTrue(
				vm.GetAngle() == GVE::Vec3(0.0f, 90.0f, 0.0f),
				L"Invalid angle Y");
			Assert::IsTrue(
				GVE::distance(vm.GetRMat() * GVE::Vec3(0.0f, 0.0f, 1.0f), GVE::Vec3(1.0f, 0.0f, 0.0f)) < e,
				L"Invalid rotation Y");

			vm.SetAngle(GVE::Vec3(0.0f, 0.0f, 90.0f));
			Assert::IsTrue(
				vm.GetAngle() == GVE::Vec3(0.0f, 0.0f, 90.0f),
				L"Invalid angle X");
			Assert::IsTrue(
				GVE::distance(vm.GetRMat() * GVE::Vec3(0.0f, 1.0f, 0.0f), GVE::Vec3(1.0f, 0.0f, 0.0f)) < e,
				L"Invalid rotation Z");
		}
		TEST_METHOD(PositionTest)
		{
			GVE::Helper::Transformation::Dimension3::ViewMatrix vm(GVE::Vec3(0.0f), GVE::Vec3(0.0f));

			vm.SetPosition(GVE::Vec3(10.0f, 0.0f, 0.0f));
			Assert::IsTrue(
				vm.GetPosition() == GVE::Vec3(10.0f, 0.0f, 0.0f),
				L"Invalid position X");
			Assert::IsTrue(
				GVE::distance(vm.GetVMat() * GVE::Vec4(10.0f, 1.0f, 1.0f, 1.0f), GVE::Vec4(0.0f, 1.0f, 1.0f, 1.0f)) < e,
				L"Invalid translation X");

			vm.SetPosition(GVE::Vec3(0.0f, 10.0f, 0.0f));
			Assert::IsTrue(
				vm.GetPosition() == GVE::Vec3(0.0f, 10.0f, 0.0f),
				L"Invalid position Y");
			Assert::IsTrue(
				GVE::distance(vm.GetVMat() * GVE::Vec4(1.0f, 10.0f, 1.0f, 1.0f), GVE::Vec4(1.0f, 0.0f, 1.0f, 1.0f)) < e,
				L"Invalid translation Y");

			vm.SetPosition(GVE::Vec3(0.0f, 0.0f, 10.0f));
			Assert::IsTrue(
				vm.GetPosition() == GVE::Vec3(0.0f, 0.0f, 10.0f),
				L"Invalid position Z");
			Assert::IsTrue(
				GVE::distance(vm.GetVMat() * GVE::Vec4(1.0f, 1.0f, 10.0f, 1.0f), GVE::Vec4(1.0f, 1.0f, 0.0f, 1.0f)) < e,
				L"Invalid translation Z");
		}
		TEST_METHOD(ViewTest)
		{
			GVE::Helper::Transformation::Dimension3::ViewMatrix vm(GVE::Vec3(0.0f), GVE::Vec3(0.0f));

			vm.SetAngle(GVE::Vec3(0.0f, 90.0f, 0.0f));
			Assert::IsTrue(
				vm.GetAngle() == GVE::Vec3(0.0f, 90.0f, 0.0f),
				L"Invalid angle");

			vm.SetPosition(GVE::Vec3(1.0f, 0.0f, 0.0f));
			Assert::IsTrue(
				vm.GetPosition() == GVE::Vec3(1.0f, 0.0f, 0.0f),
				L"Invalid position");

			Assert::IsTrue(
				GVE::distance(vm.GetVMat() * GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f), GVE::Vec4(-1.0f, 0.0f, -1.0f, 1.0f)) < e,
				L"Invalid view matrix");

			Assert::IsTrue(
				GVE::distance(vm.GetVIMat() * GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f), GVE::Vec4(2.0f, 0.0f, 0.0f, 1.0f)) < e,
				L"Invalid view inverse matrix");
		}
	};
}