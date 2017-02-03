#pragma region Include
#pragma once


#include "Header.hpp"


#include <System/System.hpp>
#include <Logic/Mathematics.hpp>
#include <Logic/Helpers.hpp>
#pragma endregion


namespace GreatVEngine
{
	class Cast
	{
	public:
		struct PlaneData
		{
			const Vec3 center;
			const Vec3 normal;
			inline PlaneData() = default;
			inline PlaneData(const Vec3& center_, const Vec3& normal_):
				center(center_),
				normal(normal_)
			{
			}
		};
		struct SphereData
		{
			const Vec3 center;
			const Float32 radius;
		};
		struct QuadData
		{
			Vec3	position;
			Vec3	angle;
			Vec2	size;
		};
		struct BoxData
		{
			Vec3	position;
			Vec3	angle;
			Vec3	size;
		};
	public:
		class Ray // Have begin & direction (no end)
		{
		public:
			struct Data
			{
				const Vec3 begin;
				const Vec3 direction; // must be normalized
				inline Data() = default;
				inline Data(const Vec3 begin_, const Vec3& direction_):
					begin(begin_),
					direction(direction_)
				{
				}
			};
			struct Collide
			{
				Vec3	position	= Vec3(0.0f);
				Vec3	normal		= Vec3(0.0f, 0.0f, 1.0f);
				Float32	distance	= 0.0f;
			};
		protected:
			static Collide collide;
		protected:
			inline static Float32 PlaneDistance(const Data& ray_, const PlaneData& plane_);
			inline static bool IsPlane(const Float32& distance_);
		public:
			inline static const Collide& GetLast()
			{
				return collide;
			}
			inline static bool PlaneCheck(const Data& ray_, const PlaneData& plane_);
			inline static bool PlaneProcess(const Data& ray_, const PlaneData& plane_);
			inline static bool SphereCheck(const Data& ray_, const SphereData& sphere_);
			inline static bool SphereProcess(const Data& ray_, const SphereData& sphere_);
			inline static bool QuadCheck(const Data& ray_, const QuadData& quad_);
			inline static bool QuadProcess(const Data& ray_, const QuadData& quad_);
			inline static bool BoxCheck(const Data& ray_, const BoxData& box_);
			inline static bool BoxProcess(const Data& ray_, const BoxData& box_);
		};
		class Interval // Have begin & end
		{
		public:
			struct Data
			{
				const Vec3 begin;
				const Vec3 end;
			};
			struct Collide
			{
				Vec3	position	= Vec3(0.0f);
				Vec3	normal		= Vec3(0.0f, 0.0f, 1.0f);
				Float32	t			= 0.0f;
				Float32	distance	= 0.0f;
			};
		protected:
			static Collide collide;
		protected:
			inline static Float32 PlaneT(const Data& interval_, const PlaneData& plane_);
			inline static bool IsPlane(const Float32& t_);
		public:
			inline static const Collide& GetLast()
			{
				return collide;
			}
			inline static bool PlaneCheck(const Data& interval_, const PlaneData& plane_);
			inline static bool PlaneProcess(const Data& interval_, const PlaneData& plane_);
			inline static bool SphereCheck(const Data& interval_, const SphereData& sphere_);
			inline static bool SphereProcess(const Data& interval_, const SphereData& sphere_);
			inline static bool QuadCheck(const Data& interval_, const QuadData& quad_);
			inline static bool QuadProcess(const Data& interval_, const QuadData& quad_);
			inline static bool BoxCheck(const Data& interval_, const BoxData& box_);
			inline static bool BoxProcess(const Data& interval_, const BoxData& box_);
		};
	};
}


#pragma region Cast
#pragma region Ray
inline GreatVEngine::Float32 GreatVEngine::Cast::Ray::PlaneDistance(const Data& ray_, const PlaneData& plane_)
{
	return
		dot(plane_.normal, plane_.center - ray_.begin) /
		dot(plane_.normal, ray_.direction);
}
inline bool GreatVEngine::Cast::Ray::IsPlane(const Float32& distance_)
{
	return distance_ >= 0.0f;
}
inline bool GreatVEngine::Cast::Ray::PlaneCheck(const Data& ray_, const PlaneData& plane_)
{
	auto distance = PlaneDistance(ray_, plane_);
	return IsPlane(distance);
}
inline bool GreatVEngine::Cast::Ray::PlaneProcess(const Data& ray_, const PlaneData& plane_)
{
	auto distance = PlaneDistance(ray_, plane_);
	if(IsPlane(distance))
	{
		collide.distance = distance;
		collide.position = ray_.begin + ray_.direction * collide.distance;
		collide.normal = dot(plane_.normal, ray_.direction) < 0.0f ? plane_.normal : -plane_.normal;

		return true;
	}

	return false;
}
inline bool GreatVEngine::Cast::Ray::SphereCheck(const Data& ray_, const SphereData& sphere_)
{
	auto projectedDistance = ProjectT(sphere_.center - ray_.begin, ray_.direction);
	auto closestPoint = ray_.begin + ray_.direction * projectedDistance;
	auto offsetDistance = distance(sphere_.center, closestPoint);

	return offsetDistance < sphere_.radius;
}
inline bool GreatVEngine::Cast::Ray::SphereProcess(const Data& ray_, const SphereData& sphere_)
{
	auto a = Sqr(ray_.direction);
	auto b = 2.0f * dot(ray_.begin - sphere_.center, ray_.direction);
	auto c = Sqr(ray_.begin - sphere_.center) - Sqr(sphere_.radius);

	auto d2 = b*b - 4.0f * a * c;
	if(d2 > 0.0f)
	{
		auto d = sqrt(d2);

		auto t1 = (-b + d) / (2.0f * a);
		auto t2 = (-b - d) / (2.0f * a);

		auto t = min(t1, t2);
		
		collide.distance = t;
		collide.position = ray_.begin + ray_.direction * collide.distance;
		collide.normal = normalize(collide.position - sphere_.center);

		return true;
	}

	return false;
}
inline bool GreatVEngine::Cast::Ray::QuadCheck(const Data& ray_, const QuadData& quad_)
{
	auto normal = RotateZXY3(quad_.angle) * Vec3(0.0f, 0.0f, -1.0f);

	auto distance = PlaneDistance(ray_, {quad_.position, normal});
	if(IsPlane(distance))
	{
		auto position = ray_.begin + ray_.direction * distance;

		auto localPosition = VecXYZ((
			RotateYXZ4(-quad_.angle) *
			Move4(-quad_.position)) *
			Vec4(position, 1.0f));

		if(
			localPosition.x >= -quad_.size.x*0.5f && localPosition.x <= +quad_.size.x*0.5f &&
			localPosition.y >= -quad_.size.y*0.5f && localPosition.y <= +quad_.size.y*0.5f)
		{
			return true;
		}
	}

	return false;
}
inline bool GreatVEngine::Cast::Ray::QuadProcess(const Data& ray_, const QuadData& quad_)
{
	auto normal = RotateZXY3(quad_.angle) * Vec3(0.0f, 0.0f, -1.0f);

	auto distance = PlaneDistance(ray_, {quad_.position, normal});
	if(IsPlane(distance))
	{
		auto position = ray_.begin + ray_.direction * distance;

		auto localPosition = VecXYZ((
			RotateYXZ4(-quad_.angle) *
			Move4(-quad_.position)) *
			Vec4(position, 1.0f));

		if(
			localPosition.x >= -quad_.size.x*0.5f && localPosition.x <= +quad_.size.x*0.5f &&
			localPosition.y >= -quad_.size.y*0.5f && localPosition.y <= +quad_.size.y*0.5f)
		{
			collide.distance = distance;
			collide.position = position;
			collide.normal = dot(normal, ray_.direction) < 0.0f ? normal : -normal;

			return true;
		}
	}

	return false;
}
inline bool GreatVEngine::Cast::Ray::BoxCheck(const Data& ray_, const BoxData& box_)
{
	auto oldCollide = collide;
	Float32 distance = FLT_MAX;

	auto rMat = RotateZXY3(box_.angle);
	auto aY = GetAngle(rMat * RotateY3(90.0f));
	auto aX = GetAngle(rMat * RotateX3(90.0f));

	if(QuadProcess(ray_, {box_.position + rMat * Vec3(-box_.size.x*0.5f, 0.0f, 0.0f), aY, VecZY(box_.size)}) && collide.distance < distance)
	{
		distance = collide.distance;
	}
	if(QuadProcess(ray_, {box_.position + rMat * Vec3(+box_.size.x*0.5f, 0.0f, 0.0f), aY, VecZY(box_.size)}) && collide.distance < distance)
	{
		distance = collide.distance;
	}

	if(QuadProcess(ray_, {box_.position + rMat * Vec3(0.0f, -box_.size.y*0.5f, 0.0f), aX, VecXZ(box_.size)}) && collide.distance < distance)
	{
		distance = collide.distance;
	}
	if(QuadProcess(ray_, {box_.position + rMat * Vec3(0.0f, +box_.size.y*0.5f, 0.0f), aX, VecXZ(box_.size)}) && collide.distance < distance)
	{
		distance = collide.distance;
	}

	if(QuadProcess(ray_, {box_.position + rMat * Vec3(0.0f, 0.0f, -box_.size.z*0.5f), box_.angle, VecXY(box_.size)}) && collide.distance < distance)
	{
		distance = collide.distance;
	}
	if(QuadProcess(ray_, {box_.position + rMat * Vec3(0.0f, 0.0f, +box_.size.z*0.5f), box_.angle, VecXY(box_.size)}) && collide.distance < distance)
	{
		distance = collide.distance;
	}

	collide = oldCollide;

	return distance < FLT_MAX;
}
inline bool GreatVEngine::Cast::Ray::BoxProcess(const Data& ray_, const BoxData& box_)
{
	auto oldCollide = collide;
	Collide tmpCollide;
	tmpCollide.distance = FLT_MAX;

	auto rMat = RotateZXY3(box_.angle);
	auto aY = GetAngle(rMat * RotateY3(90.0f));
	auto aX = GetAngle(rMat * RotateX3(90.0f));

	if(QuadProcess(ray_, {box_.position + rMat * Vec3(-box_.size.x*0.5f, 0.0f, 0.0f), aY, VecZY(box_.size)}) && collide.distance < tmpCollide.distance)
	{
		tmpCollide = collide;
	}
	if(QuadProcess(ray_, {box_.position + rMat * Vec3(+box_.size.x*0.5f, 0.0f, 0.0f), aY, VecZY(box_.size)}) && collide.distance < tmpCollide.distance)
	{
		tmpCollide = collide;
	}

	if(QuadProcess(ray_, {box_.position + rMat * Vec3(0.0f, -box_.size.y*0.5f, 0.0f), aX, VecXZ(box_.size)}) && collide.distance < tmpCollide.distance)
	{
		tmpCollide = collide;
	}
	if(QuadProcess(ray_, {box_.position + rMat * Vec3(0.0f, +box_.size.y*0.5f, 0.0f), aX, VecXZ(box_.size)}) && collide.distance < tmpCollide.distance)
	{
		tmpCollide = collide;
	}

	if(QuadProcess(ray_, {box_.position + rMat * Vec3(0.0f, 0.0f, -box_.size.z*0.5f), box_.angle, VecXY(box_.size)}) && collide.distance < tmpCollide.distance)
	{
		tmpCollide = collide;
	}
	if(QuadProcess(ray_, {box_.position + rMat * Vec3(0.0f, 0.0f, +box_.size.z*0.5f), box_.angle, VecXY(box_.size)}) && collide.distance < tmpCollide.distance)
	{
		tmpCollide = collide;
	}

	if(tmpCollide.distance < FLT_MAX)
	{
		collide = tmpCollide;
		return true;
	}

	collide = oldCollide;
	return false;
}
#pragma endregion
#pragma region Interval
inline GreatVEngine::Float32 GreatVEngine::Cast::Interval::PlaneT(const Data& interval_, const PlaneData& plane_)
{
	return
		dot(plane_.normal, plane_.center - interval_.begin) /
		dot(plane_.normal, interval_.end - interval_.begin);
}
inline bool GreatVEngine::Cast::Interval::IsPlane(const Float32& t_)
{
	return t_ >= 0.0f && t_ <= 1.0f;
}
inline bool GreatVEngine::Cast::Interval::PlaneCheck(const Data& interval_, const PlaneData& plane_)
{
	auto t = PlaneT(interval_, plane_);
	return IsPlane(t);
}
inline bool GreatVEngine::Cast::Interval::PlaneProcess(const Data& interval_, const PlaneData& plane_)
{
	auto t = PlaneT(interval_, plane_);
	if(IsPlane(t))
	{
		collide.t = t;
		collide.position = Mix(interval_.begin, interval_.end, collide.t);
		collide.distance = distance(interval_.begin, collide.position);
		collide.normal = dot(plane_.normal, interval_.begin - interval_.end) > 0.0f ? plane_.normal : -plane_.normal;

		return true;
	}

	return false;
}
inline bool GreatVEngine::Cast::Interval::SphereCheck(const Data& interval_, const SphereData& sphere_)
{
	auto projectedT = ProjectT(sphere_.center - interval_.begin, interval_.end - interval_.begin);

	if(projectedT >= 0.0f && projectedT <= 1.0f)
	{
		auto closestPoint = Mix(interval_.begin, interval_.end, projectedT);
		auto offsetDistance = distance(sphere_.center, closestPoint);

		return offsetDistance < sphere_.radius;
	}

	return false;
}
inline bool GreatVEngine::Cast::Interval::SphereProcess(const Data& interval_, const SphereData& sphere_)
{
	auto direction = interval_.end - interval_.begin;

	auto a = Sqr(direction);
	auto b = 2.0f * dot(interval_.begin - sphere_.center, direction);
	auto c = Sqr(interval_.begin - sphere_.center) - Sqr(sphere_.radius);

	auto d2 = b*b - 4.0f * a * c;
	if(d2 > 0.0f)
	{
		auto d = sqrt(d2);

		auto t1 = (-b + d) / (2.0f * a);
		auto t2 = (-b - d) / (2.0f * a);

		auto t = [&]()
		{
			if(t1 >= 0.0f && t1 <= 1.0f)
			{
				if(t2 >= 0.0f && t2 <= 1.0f)
				{
					return min(t1, t2);
				}
				else
				{
					return t1;
				}
			}
			else
			{
				if(t2 >= 0.0f && t2 <= 1.0f)
				{
					return t2;
				}
			}

			return FLT_MAX;
		}();

		if(t != FLT_MAX)
		{
			collide.t = t;
			collide.distance = length(direction) * collide.t;
			collide.position = interval_.begin + direction * collide.t;
			collide.normal = normalize(collide.position - sphere_.center);

			return true;
		}
	}

	return false;
}
inline bool GreatVEngine::Cast::Interval::QuadCheck(const Data& interval_, const QuadData& quad_)
{
	auto normal = RotateZXY3(quad_.angle) * Vec3(0.0f, 0.0f, -1.0f);

	auto t = PlaneT(interval_, {quad_.position, normal});
	if(IsPlane(t))
	{
		auto position = Mix(interval_.begin, interval_.end, t);

		auto localPosition = VecXYZ((
			RotateYXZ4(-quad_.angle) *
			Move4(-quad_.position)) *
			Vec4(position, 1.0f));

		if(
			localPosition.x >= -quad_.size.x*0.5f && localPosition.x <= +quad_.size.x*0.5f &&
			localPosition.y >= -quad_.size.y*0.5f && localPosition.y <= +quad_.size.y*0.5f)
		{
			return true;
		}
	}

	return false;
}
inline bool GreatVEngine::Cast::Interval::QuadProcess(const Data& interval_, const QuadData& quad_)
{
	auto normal = RotateZXY3(quad_.angle) * Vec3(0.0f, 0.0f, -1.0f);
	auto direction = interval_.end - interval_.begin;

	auto t = PlaneT(interval_, {quad_.position, normal});
	if(IsPlane(t))
	{
		auto position = Mix(interval_.begin, interval_.end, t);

		auto localPosition = VecXYZ((
			RotateYXZ4(-quad_.angle) *
			Move4(-quad_.position)) *
			Vec4(position, 1.0f));

		if(
			localPosition.x >= -quad_.size.x*0.5f && localPosition.x <= +quad_.size.x*0.5f &&
			localPosition.y >= -quad_.size.y*0.5f && localPosition.y <= +quad_.size.y*0.5f)
		{
			collide.t = t;
			collide.distance = length(direction) * collide.t;
			collide.position = position;
			collide.normal = dot(normal, direction) < 0.0f ? normal : -normal;

			return true;
		}
	}

	return false;
}
inline bool GreatVEngine::Cast::Interval::BoxCheck(const Data& interval_, const BoxData& box_)
{
	auto oldCollide = collide;
	Float32 t = FLT_MAX;

	auto rMat = RotateZXY3(box_.angle);
	auto aY = GetAngle(rMat * RotateY3(90.0f));
	auto aX = GetAngle(rMat * RotateX3(90.0f));

	if(QuadProcess(interval_, {box_.position + rMat * Vec3(-box_.size.x*0.5f, 0.0f, 0.0f), aY, VecZY(box_.size)}) && collide.t < t)
	{
		t = collide.t;
	}
	if(QuadProcess(interval_, {box_.position + rMat * Vec3(+box_.size.x*0.5f, 0.0f, 0.0f), aY, VecZY(box_.size)}) && collide.t < t)
	{
		t = collide.t;
	}

	if(QuadProcess(interval_, {box_.position + rMat * Vec3(0.0f, -box_.size.y*0.5f, 0.0f), aX, VecXZ(box_.size)}) && collide.t < t)
	{
		t = collide.t;
	}
	if(QuadProcess(interval_, {box_.position + rMat * Vec3(0.0f, +box_.size.y*0.5f, 0.0f), aX, VecXZ(box_.size)}) && collide.t < t)
	{
		t = collide.t;
	}

	if(QuadProcess(interval_, {box_.position + rMat * Vec3(0.0f, 0.0f, -box_.size.z*0.5f), box_.angle, VecXY(box_.size)}) && collide.t < t)
	{
		t = collide.t;
	}
	if(QuadProcess(interval_, {box_.position + rMat * Vec3(0.0f, 0.0f, +box_.size.z*0.5f), box_.angle, VecXY(box_.size)}) && collide.t < t)
	{
		t = collide.t;
	}

	collide = oldCollide;
	return t < FLT_MAX;
}
inline bool GreatVEngine::Cast::Interval::BoxProcess(const Data& interval_, const BoxData& box_)
{
	auto oldCollide = collide;
	Collide tmpCollide;
	tmpCollide.t = FLT_MAX;

	auto rMat = RotateZXY3(box_.angle);
	auto aY = GetAngle(rMat * RotateY3(90.0f));
	auto aX = GetAngle(rMat * RotateX3(90.0f));

	if(QuadProcess(interval_, {box_.position + rMat * Vec3(-box_.size.x*0.5f, 0.0f, 0.0f), aY, VecZY(box_.size)}) && collide.t < tmpCollide.t)
	{
		tmpCollide = collide;
	}
	if(QuadProcess(interval_, {box_.position + rMat * Vec3(+box_.size.x*0.5f, 0.0f, 0.0f), aY, VecZY(box_.size)}) && collide.t < tmpCollide.t)
	{
		tmpCollide = collide;
	}

	if(QuadProcess(interval_, {box_.position + rMat * Vec3(0.0f, -box_.size.y*0.5f, 0.0f), aX, VecXZ(box_.size)}) && collide.t < tmpCollide.t)
	{
		tmpCollide = collide;
	}
	if(QuadProcess(interval_, {box_.position + rMat * Vec3(0.0f, +box_.size.y*0.5f, 0.0f), aX, VecXZ(box_.size)}) && collide.t < tmpCollide.t)
	{
		tmpCollide = collide;
	}

	if(QuadProcess(interval_, {box_.position + rMat * Vec3(0.0f, 0.0f, -box_.size.z*0.5f), box_.angle, VecXY(box_.size)}) && collide.t < tmpCollide.t)
	{
		tmpCollide = collide;
	}
	if(QuadProcess(interval_, {box_.position + rMat * Vec3(0.0f, 0.0f, +box_.size.z*0.5f), box_.angle, VecXY(box_.size)}) && collide.t < tmpCollide.t)
	{
		tmpCollide = collide;
	}

	if(tmpCollide.t < FLT_MAX)
	{
		collide = tmpCollide;
		return true;
	}

	collide = oldCollide;
	return false;
}
#pragma endregion
#pragma endregion


#pragma region
#pragma endregion


