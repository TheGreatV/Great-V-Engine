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


	inline Vec3 VecXYZ(const Vec4& vec_)
	{
		return Vec3(vec_.x, vec_.y, vec_.z);
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

		return glm::transpose(Mat4(
			f / aspect_,	0.0f,	0.0f,	0.0f,
			0.0f,			f,		0.0f,	0.0f,
			0.0f,			0.0f,	b,		a,
			0.0f,			0.0f,	1.0f,	0.0f));
	}
	inline Mat4 PerspectiveInverse(const Float32& fov_, const Float32& aspect_, const Float32& near_, const Float32& far_)
	{
		auto g = radians(fov_);
		float32 f = 1.0f / tanf(g / 2.0f);
		float32 a = (far_ + near_) / (near_ - far_);
		float32 b = (2.0f * far_ * near_) / (near_ - far_);

		return glm::transpose(Mat4(
			aspect_ / f,	0.0f,		0.0f,		0.0f,
			0.0f,			1.0f / f,	0.0f,		0.0f,
			0.0f,			0.0f,		0.0f,		1.0f,
			0.0f,			0.0f,		1.0f / b,	a / b));
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
}


#pragma region
#pragma endregion


