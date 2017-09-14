#pragma region Include
#pragma once


#include <Header/Header.hpp>
#include "Header.hpp"


#include <cstdint>


#include <half.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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
	using DVec2			= glm::tvec2<Float64, glm::highp>;
	using DVec3			= glm::tvec3<Float64, glm::highp>;
	using DVec4			= glm::tvec4<Float64, glm::highp>;
	using BVec2			= glm::bvec2;
	using BVec3			= glm::bvec3;
	using BVec4			= glm::bvec4;
	using IVec2			= glm::ivec2;
	using IVec3			= glm::ivec3;
	using IVec4			= glm::ivec4;
	using LVec2			= glm::tvec2<SInt64, glm::highp>;
	using LVec3			= glm::tvec3<SInt64, glm::highp>;
	using LVec4			= glm::tvec4<SInt64, glm::highp>;
	using UVec2			= glm::uvec2;
	using UVec3			= glm::uvec3;
	using UVec4			= glm::uvec4;

	using Mat2			= glm::mat2;
	using Mat3			= glm::mat3;
	using Mat4			= glm::mat4;


	const Float32		PI = 3.1415926535897932384626433832795f;


	class Quaternion:
		Vec4
	{
	public:
		inline Quaternion() = default;
		inline explicit Quaternion(const Vec4& value_):
			Vec4(value_)
		{
		}
		inline Quaternion(const Vec3& dir_, const Float32& angle_):
			Vec4([&dir_, &angle_](){
				auto a = radians(angle_);
				return Vec4(
					sin(a*0.5f)*dir_.x, // -sin(a*0.5f)*dir_.x,
					sin(a*0.5f)*dir_.y, // -sin(a*0.5f)*dir_.y,
					sin(a*0.5f)*dir_.z, // -sin(a*0.5f)*dir_.z,
					cos(a*0.5f));				//+aX +aY -aZ
			}())
		{
		}
		inline explicit Quaternion(const Mat3& source_):
			Vec4([&source_](){
				auto m = transpose(source_);

				/*typedef float mtx_elm[3][3];
				const mtx_elm& m = mtx.m;
				float tr = m[0][0] + m[1][1] + m[2][2]; // trace of martix
				if (tr > 0.0f){     // if trace positive than "w" is biggest component
					set( m[1][2] - m[2][1], m[2][0] - m[0][2], m[0][1] - m[1][0], tr + 1.0f );
				}else                 // Some of vector components is bigger
				if( (m[0][0] > m[1][1] ) && ( m[0][0] > m[2][2]) ) {
					set( 1.0f + m[0][0] - m[1][1] - m[2][2], m[1][0] + m[0][1],
						 m[2][0] + m[0][2], m[1][2] - m[2][1] );
				}else 
				if ( m[1][1] > m[2][2] ){
					set( m[1][0] + m[0][1], 1.0f + m[1][1] - m[0][0] - m[2][2],
						 m[2][1] + m[1][2], m[2][0] - m[0][2] ); 
				}else{
					set( m[2][0] + m[0][2], m[2][1] + m[1][2],
						 1.0f + m[2][2] - m[0][0] - m[1][1], m[0][1] - m[1][0] );
				}*/

				float tr = m[0][0] + m[1][1] + m[2][2]; // trace of martix

				if (tr > 0.0f)
				{     // if trace positive than "w" is biggest component
					return Vec4(m[1][2] - m[2][1], m[2][0] - m[0][2], m[0][1] - m[1][0], tr + 1.0f);
				}
				else
				{	// Some of vector components is bigger
					if((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
					{
						return Vec4(1.0f + m[0][0] - m[1][1] - m[2][2], m[1][0] + m[0][1], m[2][0] + m[0][2], m[1][2] - m[2][1]);
					}
					else
					{
						if(m[1][1] > m[2][2])
						{
							return Vec4(m[1][0] + m[0][1], 1.0f + m[1][1] - m[0][0] - m[2][2], m[2][1] + m[1][2], m[2][0] - m[0][2]);
						}
						else
						{
							return Vec4(m[2][0] + m[0][2], m[2][1] + m[1][2], 1.0f + m[2][2] - m[0][0] - m[1][1], m[0][1] - m[1][0]);
						}
					}
				}
			}())
		{
		}
	public:
		/*inline static Quaternion operator * (const Quaternion& a, const Quaternion& b)
		{
			// Vec4(cross(v,v') + w*v' + w'*v, w*w' - dot(v,v'))
			auto v = cross(VecXYZ(a), VecXYZ(b)) + a.w*VecXYZ(b) + b.w * VecXYZ(a);
			auto w = a.w * b.w - dot(VecXYZ(a), VecXYZ(b));
			return Quaternion(Vec4(v, w));
		}
		inline Quaternion operator - () const
		{
			return Quaternion(Vec4(-x, -y, -z, w) / dot((Vec4)*this, (Vec4)*this));
		}
		inline Vec3 operator * (const Vec3& v)
		{
			auto t = (*this) * Quaternion(Vec4(v, 0.0f)) * -(*this);
			return Vec3(t.x, t.y, t.z);
		}*/
	public:
		operator Mat3() const
		{
			/*
			|       2     2                                |
			| 1 - 2Y  - 2Z    2XY - 2ZW      2XZ + 2YW     |
			|                                              |
			|                       2     2                |
		M = | 2XY + 2ZW       1 - 2X  - 2Z   2YZ - 2XW     |
			|                                              |
			|                                      2     2 |
			| 2XZ - 2YW       2YZ + 2XW      1 - 2X  - 2Y  |
			|                                              |
			*/
			return transpose(Mat3(
				1.0f - 2.0*y*y - 2.0f*z*z,		2.0f*x*y - 2.0f*z*w,		2.0f*x*z + 2.0f*y*w,
				2.0f*x*y + 2.0f*z*w,			1.0f - 2.0f*x*x - 2.0f*z*z,	2.0f*y*z - 2.0f*x*w,
				2.0f*x*z - 2.0f*y*w,			2.0f*y*z + 2.0f*x*w,		1.0f - 2.0f*x*x - 2.0f*y*y));
		}
	};


	inline Vec2 VecXY(const Vec3& vec_)
	{
		return Vec2(vec_.x, vec_.y);
	}
	inline Vec2 VecXZ(const Vec3& vec_)
	{
		return Vec2(vec_.x, vec_.z);
	}
	inline Vec2 VecZX(const Vec3& vec_)
	{
		return Vec2(vec_.z, vec_.x);
	}
	inline Vec2 VecYZ(const Vec3& vec_)
	{
		return Vec2(vec_.y, vec_.z);
	}
	inline Vec2 VecZY(const Vec3& vec_)
	{
		return Vec2(vec_.z, vec_.y);
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

	template <typename Type>
	inline Type Clamp(Type value_, Type minValue_, Type maxValue_)
	{
		return glm::clamp(value_, minValue_, maxValue_);
	}

	inline Float32 Wrap(const Float32& val, const Float32& minVal, const Float32& maxVal)
	{
		return val - floor((val - minVal) / (maxVal - minVal))*(maxVal - minVal);
	}
	inline Float32 Wrap360(const Float32& val)
	{
		return Wrap(val, 0.0f, 360.0f);
	}
	inline Vec2 Wrap360(const Vec2& val)
	{
		return Vec2
			(
			Wrap360(val.x),
			Wrap360(val.y)
			);
	}
	inline Vec3 Wrap360(const Vec3& val)
	{
		return Vec3
			(
			Wrap360(val.x),
			Wrap360(val.y),
			Wrap360(val.z)
			);
	}
	inline Vec4 Wrap360(const Vec4& val)
	{
		return Vec4
			(
			Wrap360(val.x),
			Wrap360(val.y),
			Wrap360(val.z),
			Wrap360(val.w)
			);
	}
	inline Float32 Wrap180(const Float32& val)
	{
		return Wrap(val, -180.0f, +180.0f);
	}
	inline Vec2 Wrap180(const Vec2& val)
	{
		return Vec2
			(
			Wrap180(val.x),
			Wrap180(val.y)
			);
	}
	inline Vec3 Wrap180(const Vec3& val)
	{
		return Vec3
			(
			Wrap180(val.x),
			Wrap180(val.y),
			Wrap180(val.z)
			);
	}
	inline Vec4 Wrap180(const Vec4& val)
	{
		return Vec4
			(
			Wrap180(val.x),
			Wrap180(val.y),
			Wrap180(val.z),
			Wrap180(val.w)
			);
	}

	template<typename T>
	inline Float32 ProjectT(const T& a, const T& b)
	{
		auto l = length(b);
		return dot(a, b) / (l*l);
	}
	template<typename T>
	inline typename T Project(const T& a, const T& b)
	{
		return b * projT(a, b);
	};

	template <typename T>
	inline Float32 Length(const T& t)
	{
		return glm::length(t);
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
	template <typename T>
	inline Float32 Sqr(const T& a)
	{
		return glm::dot(a, a);
	}

	inline Mat2 Rotate2(const Float32& angle_)
	{
		auto a = glm::radians(angle_);
		auto s = glm::sin(a);
		auto c = glm::cos(a);

		return glm::transpose(Mat2(
			c, s,
			-s, c));
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

	inline Mat4 One4()
	{
		return glm::transpose(Mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f));
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
		return GetAngle(Mat3(
			mat[0][0], mat[1][0], mat[2][0],
			mat[0][1], mat[1][1], mat[2][1],
			mat[0][2], mat[1][2], mat[2][2]));
	}
}


#pragma region
#pragma endregion


