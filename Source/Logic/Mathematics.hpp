#pragma region Include
#pragma once


#include <Header/Header.hpp>
#include "Header.hpp"


#include <cstdint>


#include <half.hpp>
#include <glm/glm.hpp>
#pragma endregion


namespace GreatVEngine
{
	using namespace glm;


	using SInt8			= std::int8_t;
	using UInt8			= std::uint8_t;
	using SInt16		= std::int16_t;
	using UInt16		= std::uint16_t;
	using SInt32		= std::int32_t;
	using UInt32		= std::uint32_t;
	using SInt64		= std::int64_t;
	using UInt64		= std::uint64_t;

	using Float16		= half_float::half;
	using Float32		= std::float_t;
	using Float64		= std::double_t;

	using Size2			= glm::tvec2<Size, glm::highp>;
	using Size3			= glm::tvec3<Size, glm::highp>;
	using Size4			= glm::tvec4<Size, glm::highp>;
	using Vec2			= glm::vec2;
	using Vec3			= glm::vec3;
	using Vec4			= glm::vec4;
	using HVec2			= glm::tvec2<Float16, glm::highp>;
	using HVec3			= glm::tvec3<Float16, glm::highp>;
	using HVec4			= glm::tvec4<Float16, glm::highp>;
	using BVec2			= glm::bvec2;
	using BVec3			= glm::bvec3;
	using BVec4			= glm::bvec4;
	using IVec2			= glm::ivec2;
	using IVec3			= glm::ivec3;
	using IVec4			= glm::ivec4;
	using UVec2			= glm::uvec2;
	using UVec3			= glm::uvec3;
	using UVec4			= glm::uvec4;

	using Mat2			= glm::mat2;
	using Mat3			= glm::mat3;
	using Mat4			= glm::mat4;


	inline Vec2 VecXY(const Vec3& vec_)
	{
		return Vec2(vec_.x, vec_.y);
	}
	inline Vec3 VecXYZ(const Vec4& vec_)
	{
		return Vec3(vec_.x, vec_.y, vec_.z);
	}

	inline Float32 Rnd()
	{
		return static_cast <Float32> (rand()) / static_cast <Float32> (RAND_MAX);
	}
	inline Float32 Rnd(const Float32& max_)
	{
		return static_cast <Float32> (rand()) / (static_cast <Float32> (RAND_MAX / max_));
	}
	inline Float32 Rnd(const Float32& min_, const Float32& max_)
	{
		return min_ + static_cast <Float32> (rand()) / (static_cast <Float32> (RAND_MAX / (max_ - min_)));
	}
	inline Vec2 Rnd2()
	{
		return Vec2(Rnd(), Rnd());
	}
	inline Vec2 Rnd2(const Float32& max_)
	{
		return Vec2(Rnd(max_), Rnd(max_));
	}
	inline Vec2 Rnd2(const Float32& min_, const Float32& max_)
	{
		return Vec2(Rnd(min_, max_), Rnd(min_, max_));
	}
	inline Vec3 Rnd3()
	{
		return Vec3(Rnd(), Rnd(), Rnd());
	}
	inline Vec3 Rnd3(const Float32& max_)
	{
		return Vec3(Rnd(max_), Rnd(max_), Rnd(max_));
	}
	inline Vec3 Rnd3(const Float32& min_, const Float32& max_)
	{
		return Vec3(Rnd(min_, max_), Rnd(min_, max_), Rnd(min_, max_));
	}
	inline Vec4 Rnd4()
	{
		return Vec4(Rnd(), Rnd(), Rnd(), Rnd());
	}
	inline Vec4 Rnd4(const Float32& max_)
	{
		return Vec4(Rnd(max_), Rnd(max_), Rnd(max_), Rnd(max_));
	}
	inline Vec4 Rnd4(const Float32& min_, const Float32& max_)
	{
		return Vec4(Rnd(min_, max_), Rnd(min_, max_), Rnd(min_, max_), Rnd(min_, max_));
	}

	template <typename genTypeT, typename genTypeU>
	inline genTypeT Mix(const genTypeT& a, const genTypeT& b, const genTypeU& t)
	{
		return glm::mix(a, b, t);
	}
	template <typename T>
	inline T Normalize(const T& t)
	{
		return glm::normalize(t);
	}

	inline Mat3 Rotate3(const Float32& angle_)
	{
		auto a = glm::radians(angle_);
		auto s = glm::sin(a);
		auto c = glm::cos(a);

		return glm::transpose(Mat3(
			c,		s,		0.0f,
			-s,		c,		0.0f,
			0.0f,	0.0f,	1.0f));
	}
	inline Mat3 Move3(const Vec2& position_)
	{
		return glm::transpose(Mat3(
			1.0f,	0.0f,	position_.x,
			0.0f,	1.0f,	position_.y,
			0.0f,	0.0f,	1.0f));
	}
	inline Mat3 Scale3(const Vec2& scale_)
	{
		return glm::transpose(Mat3(
			scale_.x,	0.0f,		0.0f,
			0.0f,		scale_.y,	0.0f,
			0.0f,		0.0f,		1.0f));
	}

	inline Mat3 RotateX3(const Float32& angle_)
	{
		auto a = glm::radians(angle_);
		auto s = glm::sin(a);
		auto c = glm::cos(a);

		return glm::transpose(Mat3(
			1.0f,	0.0f,	0.0f,
			0.0f,	c,		-s,
			0.0f,	s,		c));
	}
	inline Mat3 RotateY3(const Float32& angle_)
	{
		auto a = glm::radians(angle_);
		auto s = glm::sin(a);
		auto c = glm::cos(a);

		return glm::transpose(Mat3(
			c,		0.0f,	s,
			0.0f,	1.0f,	0.0f,
			-s,		0.0f,	c));
	}
	inline Mat3 RotateZ3(const Float32& angle_)
	{
		auto a = glm::radians(angle_);
		auto s = glm::sin(a);
		auto c = glm::cos(a);

		return glm::transpose(Mat3(
			c,		s,		0.0f,
			-s,		c,		0.0f,
			0.0f,	0.0f,	1.0f));
	}
	inline Mat3 RotateZXY3(const Vec3& angle_)
	{
		return RotateY3(angle_.y) * RotateX3(angle_.x) * RotateZ3(angle_.z);
	}
	inline Mat3 RotateYXZ3(const Vec3& angle_)
	{
		return RotateZ3(angle_.z) * RotateX3(angle_.x) * RotateY3(angle_.y);
	}

	inline Mat4 Move4(const Vec3& position_)
	{
		return glm::transpose(Mat4(
			1.0f, 0.0f, 0.0f, position_.x,
			0.0f, 1.0f, 0.0f, position_.y,
			0.0f, 0.0f, 1.0f, position_.z,
			0.0f, 0.0f, 0.0f, 1.0f));
	}
	inline Mat4 Scale4(const Vec3& scale_)
	{
		return glm::transpose(Mat4(
			scale_.x,	0.0f,		0.0f,		0.0f,
			0.0f,		scale_.y,	0.0f,		0.0f,
			0.0f,		0.0f,		scale_.z,	0.0f,
			0.0f,		0.0f,		0.0f,		1.0f));
	}
	inline Mat4 Scale4(const Vec4& scale_)
	{
		return glm::transpose(Mat4(
			scale_.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale_.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale_.z, 0.0f,
			0.0f, 0.0f, 0.0f, scale_.w));
	}
	inline Mat4 RotateX4(const Float32& angle_)
	{
		auto a = glm::radians(angle_);
		auto s = glm::sin(a);
		auto c = glm::cos(a);

		return glm::transpose(Mat4(
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	c,		-s,		0.0f,
			0.0f,	s,		c,		0.0f,
			0.0f,	0.0f,	0.0f,	1.0f));
	}
	inline Mat4 RotateY4(const Float32& angle_)
	{
		auto a = glm::radians(angle_);
		auto s = glm::sin(a);
		auto c = glm::cos(a);

		return glm::transpose(Mat4(
			c,		0.0f,	s,		0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			-s,		0.0f,	c,		0.0f,
			0.0f,	0.0f,	0.0f,	1.0f));
	}
	inline Mat4 RotateZ4(const Float32& angle_)
	{
		auto a = glm::radians(angle_);
		auto s = glm::sin(a);
		auto c = glm::cos(a);

		return glm::transpose(Mat4(
			c,		s,		0.0f,	0.0f,
			-s,		c,		0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f));
	}
	inline Mat4 RotateZXY4(const Vec3& angle_)
	{
		return  RotateY4(angle_.y) * RotateX4(angle_.x) * RotateZ4(angle_.z);
	}
	inline Mat4 RotateYXZ4(const Vec3& angle_)
	{
		return  RotateZ4(angle_.z) * RotateX4(angle_.x) * RotateY4(angle_.y);
	}
	inline Mat4 Perspective(const Float32& fov_, const Float32& aspect_, const Float32& near_, const Float32& far_)
	{
		auto g = radians(fov_);
		float32 f = 1.0f / tanf(g / 2.0f);
		float32 a = (far_ + near_) / (near_ - far_);
		float32 b = (2.0f * far_ * near_) / (near_ - far_);

		// return glm::transpose(Mat4( // original
		// 	f / aspect_,	0.0f,	0.0f,	0.0f,
		// 	0.0f,			f,		0.0f,	0.0f,
		// 	0.0f,			0.0f,	a,		b,
		// 	0.0f,			0.0f,	-1.0f,	0.0f));
		return glm::transpose(Mat4( // corrected by *(1,1,-1)
			f / aspect_,	0.0f,	0.0f,	0.0f,
			0.0f,			f,		0.0f,	0.0f,
			0.0f,			0.0f,	-a,		b,
			0.0f,			0.0f,	1.0f,	0.0f));
	}
	inline Mat4 PerspectiveInverse(const Float32& fov_, const Float32& aspect_, const Float32& near_, const Float32& far_)
	{
		auto g = radians(fov_);
		float32 f = 1.0f / tanf(g / 2.0f);
		float32 a = (far_ + near_) / (near_ - far_);
		float32 b = (2.0f * far_ * near_) / (near_ - far_);

		// return glm::transpose(Mat4( // original
		// 	aspect_ / f,	0.0f,		0.0f,		0.0f,
		// 	0.0f,			1.0f / f,	0.0f,		0.0f,
		// 	0.0f,			0.0f,		0.0f,		-1.0f,
		// 	0.0f,			0.0f,		1.0f / b,	a / b));
		return glm::transpose(Mat4( // corrected by *(1,1,-1)
			aspect_ / f,	0.0f,		0.0f,		0.0f,
			0.0f,			1.0f / f,	0.0f,		0.0f,
			0.0f,			0.0f,		0.0f,		1.0f,
			0.0f,			0.0f,		-1.0f / b,	a / b));
	}
	inline Mat4 Orthogonal(const Float32& left_, const Float32& right_, const Float32& bottom_, const Float32& top_, const Float32& back_, const Float32& front_)
	{
		float32
			dx = 1.0f / (right_ - left_),
			dy = 1.0f / (top_ - bottom_),
			dz = 1.0f / (front_ - back_);

		return glm::transpose(Mat4(
			2*dx,	0.0f,	0.0f,	-(right_ + left_) * dx,
			0.0f,	2*dy,	0.0f,	-(top_ + bottom_)*dy,
			0.0f,	0.0f,	-2*dz,	-(front_ + back_)*dz,
			0.0f,	0.0f,	0.0f,	1.0f));
	}
	inline Mat4 OrthogonalInverse(const Float32& left_, const Float32& right_, const Float32& bottom_, const Float32& top_, const Float32& back_, const Float32& front_)
	{
		return inverse(Orthogonal(left_, right_, bottom_, top_, back_, front_));
	}
	inline Mat4 To4(const Mat3& mat_)
	{
		return Mat4(
			mat_[0].x,	mat_[0].y,	mat_[0].z,	0.0f,
			mat_[1].x,	mat_[1].y,	mat_[1].z,	0.0f,
			mat_[2].x,	mat_[2].y,	mat_[2].z,	0.0f,
			0.0f,		0.0f,		0.0f,		1.0f);
	}

	inline Float32 GetAngle(const Vec2& a)
	{
		return degrees(atan2(a.x, a.y));
	}
	inline Vec2 GetAngle(const Vec3& a)
	{
		Vec2 out;
		out.x = GetAngle(Vec2(-a.y, sqrt(a.x*a.x + a.z*a.z)));
		out.y = GetAngle(Vec2(a.x, a.z));
		return out;
	}
	inline Vec3 GetAngle(const Mat3& mat)
	{
		Vec3	result(0.0f), v_xz(0.0f), v_y(0.0f);

		v_xz = mat * Vec3(0.0f, 0.0f, 1.0f);

		auto t = GetAngle(v_xz);

		result.x = t.x;
		result.y = t.y;

		v_y = (RotateYXZ3(Vec3(-result.x, -result.y, 0.0f)) * mat) * Vec3(0.0f, 1.0f, 0.0f);

		result.z = degrees(atan2(v_y.x, v_y.y));

		return result;
	}
	inline Vec3 GetAngle(const Mat4& mat)
	{
		// return GetAngle(transpose(Mat3(
		// 	mat[0][0], mat[0][1], mat[0][2],
		// 	mat[1][0], mat[1][1], mat[1][2],
		// 	mat[2][0], mat[2][1], mat[2][2])));
		return GetAngle(transpose(Mat3(
			mat[0][0], mat[1][0], mat[2][0],
			mat[0][1], mat[1][1], mat[2][1],
			mat[0][2], mat[1][2], mat[2][2])));
	}
}


#pragma region
#pragma endregion


