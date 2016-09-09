#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#include <Logic/Logic.hpp>
namespace GVE = GreatVEngine;


namespace ModelMatrix
{
	TEST_CLASS(UnitTest1)
	{
	public:
		const GVE::Float32 e = 0.0001f;
	public:
		TEST_METHOD(IdentityTest)
		{
			GVE::Helper::Transformation::Dimension3::ModelMatrix mm(GVE::Vec3(0.0f), GVE::Vec3(0.0f), GVE::Vec3(1.0f));

			Assert::IsTrue(
				GVE::distance(mm.GetRMat() * GVE::Vec3(0.0f, 0.0f, 1.0f), GVE::Vec3(0.0f, 0.0f, 1.0f)) < e,
				L"Invalid rotation");

			Assert::IsTrue(
				GVE::distance(mm.GetVMat() * GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f), GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f)) < e,
				L"Invalid position");
		}
		TEST_METHOD(RotationTest)
		{
			GVE::Helper::Transformation::Dimension3::ModelMatrix mm(GVE::Vec3(0.0f), GVE::Vec3(0.0f), GVE::Vec3(1.0f));

			mm.SetAngle(GVE::Vec3(90.0f, 0.0f, 0.0f));
			Assert::IsTrue(
				mm.GetAngle() == GVE::Vec3(90.0f, 0.0f, 0.0f),
				L"Invalid angle X");
			Assert::IsTrue(
				GVE::distance(mm.GetRMat() * GVE::Vec3(0.0f, 1.0f, 0.0f), GVE::Vec3(0.0f, 0.0f, 1.0f)) < e,
				L"Invalid rotation X");

			mm.SetAngle(GVE::Vec3(0.0f, 90.0f, 0.0f));
			Assert::IsTrue(
				mm.GetAngle() == GVE::Vec3(0.0f, 90.0f, 0.0f),
				L"Invalid angle Y");
			Assert::IsTrue(
				GVE::distance(mm.GetRMat() * GVE::Vec3(0.0f, 0.0f, 1.0f), GVE::Vec3(1.0f, 0.0f, 0.0f)) < e,
				L"Invalid rotation Y");

			mm.SetAngle(GVE::Vec3(0.0f, 0.0f, 90.0f));
			Assert::IsTrue(
				mm.GetAngle() == GVE::Vec3(0.0f, 0.0f, 90.0f),
				L"Invalid angle X");
			Assert::IsTrue(
				GVE::distance(mm.GetRMat() * GVE::Vec3(0.0f, 1.0f, 0.0f), GVE::Vec3(1.0f, 0.0f, 0.0f)) < e,
				L"Invalid rotation Z");
		}
		TEST_METHOD(PositionTest)
		{
			GVE::Helper::Transformation::Dimension3::ModelMatrix mm(GVE::Vec3(0.0f), GVE::Vec3(0.0f), GVE::Vec3(1.0f));

			mm.SetPosition(GVE::Vec3(10.0f, 0.0f, 0.0f));
			Assert::IsTrue(
				mm.GetPosition() == GVE::Vec3(10.0f, 0.0f, 0.0f),
				L"Invalid position X");
			Assert::IsTrue(
				GVE::distance(mm.GetVMat() * GVE::Vec4(10.0f, 1.0f, 1.0f, 1.0f), GVE::Vec4(0.0f, 1.0f, 1.0f, 1.0f)) < e,
				L"Invalid translation X");

			mm.SetPosition(GVE::Vec3(0.0f, 10.0f, 0.0f));
			Assert::IsTrue(
				mm.GetPosition() == GVE::Vec3(0.0f, 10.0f, 0.0f),
				L"Invalid position Y");
			Assert::IsTrue(
				GVE::distance(mm.GetVMat() * GVE::Vec4(1.0f, 10.0f, 1.0f, 1.0f), GVE::Vec4(1.0f, 0.0f, 1.0f, 1.0f)) < e,
				L"Invalid translation Y");

			mm.SetPosition(GVE::Vec3(0.0f, 0.0f, 10.0f));
			Assert::IsTrue(
				mm.GetPosition() == GVE::Vec3(0.0f, 0.0f, 10.0f),
				L"Invalid position Z");
			Assert::IsTrue(
				GVE::distance(mm.GetVMat() * GVE::Vec4(1.0f, 1.0f, 10.0f, 1.0f), GVE::Vec4(1.0f, 1.0f, 0.0f, 1.0f)) < e,
				L"Invalid translation Z");
		}
		TEST_METHOD(ScaleTest)
		{
			GVE::Helper::Transformation::Dimension3::ModelMatrix mm(GVE::Vec3(0.0f), GVE::Vec3(0.0f), GVE::Vec3(1.0f));

			mm.SetScale(GVE::Vec3(10.0f, 1.0f, 1.0f));
			Assert::IsTrue(
				mm.GetScale() == GVE::Vec3(10.0f, 1.0f, 1.0f),
				L"Invalid scale X");
			Assert::IsTrue(
				GVE::distance(mm.GetMMat() * GVE::Vec4(1.0f, 1.0f, 1.0f, 1.0f), GVE::Vec4(10.0f, 1.0f, 1.0f, 1.0f)) < e,
				L"Invalid scaling X");

			mm.SetScale(GVE::Vec3(1.0f, 10.0f, 1.0f));
			Assert::IsTrue(
				mm.GetScale() == GVE::Vec3(1.0f, 10.0f, 1.0f),
				L"Invalid scale Y");
			Assert::IsTrue(
				GVE::distance(mm.GetMMat() * GVE::Vec4(1.0f, 1.0f, 1.0f, 1.0f), GVE::Vec4(1.0f, 10.0f, 1.0f, 1.0f)) < e,
				L"Invalid scaling Y");

			mm.SetScale(GVE::Vec3(1.0f, 1.0f, 10.0f));
			Assert::IsTrue(
				mm.GetScale() == GVE::Vec3(1.0f, 1.0f, 10.0f),
				L"Invalid scale Z");
			Assert::IsTrue(
				GVE::distance(mm.GetMMat() * GVE::Vec4(1.0f, 1.0f, 1.0f, 1.0f), GVE::Vec4(1.0f, 1.0f, 10.0f, 1.0f)) < e,
				L"Invalid scaling Z");
		}
		TEST_METHOD(ViewTest)
		{
			GVE::Helper::Transformation::Dimension3::ModelMatrix mm(GVE::Vec3(0.0f), GVE::Vec3(0.0f), GVE::Vec3(1.0f));

			mm.SetAngle(GVE::Vec3(0.0f, 90.0f, 0.0f));
			Assert::IsTrue(
				mm.GetAngle() == GVE::Vec3(0.0f, 90.0f, 0.0f),
				L"Invalid angle");

			mm.SetPosition(GVE::Vec3(1.0f, 0.0f, 0.0f));
			Assert::IsTrue(
				mm.GetPosition() == GVE::Vec3(1.0f, 0.0f, 0.0f),
				L"Invalid position");

			Assert::IsTrue(
				GVE::distance(mm.GetVMat() * GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f), GVE::Vec4(-1.0f, 0.0f, -1.0f, 1.0f)) < e,
				L"Invalid view matrix");

			Assert::IsTrue(
				GVE::distance(mm.GetVIMat() * GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f), GVE::Vec4(2.0f, 0.0f, 0.0f, 1.0f)) < e,
				L"Invalid view inverse matrix");
		}
		TEST_METHOD(ModelTest)
		{
			GVE::Helper::Transformation::Dimension3::ModelMatrix mm(GVE::Vec3(0.0f), GVE::Vec3(0.0f), GVE::Vec3(1.0f));

			mm.SetAngle(GVE::Vec3(0.0f, 90.0f, 0.0f));
			Assert::IsTrue(
				mm.GetAngle() == GVE::Vec3(0.0f, 90.0f, 0.0f),
				L"Invalid angle");

			mm.SetPosition(GVE::Vec3(1.0f, 0.0f, 0.0f));
			Assert::IsTrue(
				mm.GetPosition() == GVE::Vec3(1.0f, 0.0f, 0.0f),
				L"Invalid position");

			mm.SetScale(GVE::Vec3(1.0f, 1.0f, 2.0f));
			Assert::IsTrue(
				mm.GetScale() == GVE::Vec3(1.0f, 1.0f, 2.0f),
				L"Invalid scale");

			Assert::IsTrue(
				GVE::distance(mm.GetMMat() * GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f), GVE::Vec4(3.0f, 0.0f, 0.0f, 1.0f)) < e,
				L"Invalid model matrix");

			Assert::IsTrue(
				GVE::distance(mm.GetMIMat() * GVE::Vec4(0.0f, 0.0f, 1.0f, 1.0f), GVE::Vec4(-1.0f, 0.0f, -0.5f, 1.0f)) < e,
				L"Invalid model inverse matrix");
		}
	};
}