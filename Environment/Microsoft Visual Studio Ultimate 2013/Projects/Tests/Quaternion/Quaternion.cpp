#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


#include <Logic/Logic.hpp>
namespace GVE = GreatVEngine;


namespace Quaternion
{
	TEST_CLASS(UnitTest1)
	{
	public:
		const GVE::Float32 e = 0.0001f;
	public:
		TEST_METHOD(IdentityTest)
		{
			auto origin = GVE::Vec3(0.0f, 0.0f, 1.0f);

			auto dX = GVE::Vec3(1.0f, 0.0f, 0.0f);
			auto dY = GVE::Vec3(0.0f, 1.0f, 0.0f);
			auto dZ = GVE::Vec3(0.0f, 0.0f, 1.0f);

			auto angle = 0.0f;

			GVE::Quaternion qX(dX, angle);
			GVE::Quaternion qY(dY, angle);
			GVE::Quaternion qZ(dZ, angle);

			Assert::IsTrue(
				GVE::distance((GVE::Mat3)qX*origin, origin) < e,
				L"Invalid origin X");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)qY*origin, origin) < e,
				L"Invalid origin Y");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)qZ*origin, origin) < e,
				L"Invalid origin Z");
		}
		TEST_METHOD(RotationXTest)
		{
			auto origin = GVE::Vec3(0.0f, 1.0f, 0.0f);

			auto d = GVE::Vec3(1.0f, 0.0f, 0.0f);

			auto angle0 = 0.0f;
			auto angle45 = 45.0f;
			auto angle90 = 90.0f;
			auto angle180 = 180.0f;
			auto angle270 = 270.0f;

			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle0)*origin, GVE::RotateX3(angle0)*origin) < e,
				L"Invalid rotate on 0");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle45)*origin, GVE::RotateX3(angle45)*origin) < e,
				L"Invalid rotate on 45");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle90)*origin, GVE::RotateX3(angle90)*origin) < e,
				L"Invalid rotate on 90");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle180)*origin, GVE::RotateX3(angle180)*origin) < e,
				L"Invalid rotate on 180");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle270)*origin, GVE::RotateX3(angle270)*origin) < e,
				L"Invalid rotate on 270");
		}
		TEST_METHOD(RotationYTest)
		{
			auto origin = GVE::Vec3(0.0f, 0.0f, 1.0f);

			auto d = GVE::Vec3(0.0f, 1.0f, 0.0f);

			auto angle0 = 0.0f;
			auto angle45 = 45.0f;
			auto angle90 = 90.0f;
			auto angle180 = 180.0f;
			auto angle270 = 270.0f;

			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle0)*origin, GVE::RotateY3(angle0)*origin) < e,
				L"Invalid rotate on 0");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle45)*origin, GVE::RotateY3(angle45)*origin) < e,
				L"Invalid rotate on 45");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle90)*origin, GVE::RotateY3(angle90)*origin) < e,
				L"Invalid rotate on 90");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle180)*origin, GVE::RotateY3(angle180)*origin) < e,
				L"Invalid rotate on 180");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle270)*origin, GVE::RotateY3(angle270)*origin) < e,
				L"Invalid rotate on 270");
		}
		TEST_METHOD(RotationZTest)
		{
			auto origin = GVE::Vec3(1.0f, 0.0f, 0.0f);

			auto d = GVE::Vec3(0.0f, 0.0f, 1.0f);

			auto angle0 = 0.0f;
			auto angle45 = 45.0f;
			auto angle90 = 90.0f;
			auto angle180 = 180.0f;
			auto angle270 = 270.0f;

			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle0)*origin, GVE::RotateZ3(angle0)*origin) < e,
				L"Invalid rotate on 0");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle45)*origin, GVE::RotateZ3(angle45)*origin) < e,
				L"Invalid rotate on 45");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle90)*origin, GVE::RotateZ3(angle90)*origin) < e,
				L"Invalid rotate on 90");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle180)*origin, GVE::RotateZ3(angle180)*origin) < e,
				L"Invalid rotate on 180");
			Assert::IsTrue(
				GVE::distance((GVE::Mat3)GVE::Quaternion(d, angle270)*origin, GVE::RotateZ3(angle270)*origin) < e,
				L"Invalid rotate on 270");
		}
		TEST_METHOD(RotationYXTest)
		{
			// rotate origin on Y by 45 and then rotate on X by 30
			auto angY = 90.0f;
			auto angX = 30.0f;

			auto matY = GVE::RotateY3(angY);
			auto matX = GVE::RotateX3(angX);
			auto mat = matX * matY;

			auto o = matX * GVE::Vec3(0.0f, 0.0f, 1.0f);
			auto d = matX * GVE::Vec3(0.0f, 1.0f, 0.0f);


			auto t1 = (GVE::Mat3)GVE::Quaternion(d, angY) * o;
			auto t2 = mat * GVE::Vec3(0.0f, 0.0f, 1.0f);

			Assert::IsTrue(
				GVE::distance(
					(GVE::Mat3)GVE::Quaternion(d, angY) * o,
					mat * GVE::Vec3(0.0f, 0.0f, 1.0f)) < e,
				L"Invalid rotate");
			/*
			qq' = [ vv' + wv' + w'v, ww' Ц vХv' ]

			где vv' Ч векторное произведение, vХv' Ч скал€рное произведение векторов.

			q = v + w
			q' = v' + w'

			Vec4(cross(v,v') + w*v' + w'*v, w*w' - dot(v,v'))

			*/
		}
	};
}