#pragma region Include
#pragma once


#include "Header.hpp"


#include <System/System.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace Graphics
	{
		//	Describe device-API pair, used to render graphics
		class Context
		{
		public:
			inline Context() = default;
			inline Context(const Context&) = delete;
			virtual ~Context() = default;
		public:
			inline Context& operator = (const Context&) = delete;
		public:
			virtual void Set() = 0;
			virtual void Reset() = 0;
		};

		//	Contain objects, decals, lighting, particles
		class Scene
		{
		public:
			inline Scene() = default;
			inline Scene(const Scene&) = delete;
			virtual ~Scene() = default;
		public:
			inline Scene& operator = (const Scene&) = delete;
		};

		//	Logic of material rendering
		class Technique
		{
		public:
			// inline static Reference<Technique> Load(const Filename& filename_);
		public:
			inline Technique() = default;
			inline ~Technique() = default;
		};
		//	Map for material rendering
		class Map
		{
		public:
			// inline static Reference<Map> Load(const Filename& filename_);
		public:
			inline Map() = default;
			inline ~Map() = default;
		};
		//	Describe object's material
		class Material
		{
		public:
			enum class TechniqueType: Size
			{
				Technique0					= 0,
				Technique1					= 1,
				Technique2					= 2,
				Technique3					= 3,
				Technique4					= 4,
				Technique5					= 5,
				Technique6					= 6,
				Technique7					= 7,
				Basic						= Technique0,
				Depth						= Technique1,
				Shadow						= Technique2,
				Reflection					= Technique3,
			};
			enum class MapType: Size
			{
				Map0						= 0,
				Map1						= 1,
				Map2						= 2,
				Map3						= 3,
				Map4						= 4,
				Map5						= 5,
				Map6						= 6,
				Map7						= 7,
				Color						= Map0,
				Topology					= Map1,
				Reflections					= Map2,
			};
		public:
			const static Size TechniquesCount = 8;
			const static Size MapsCount = 8;
		public:
			inline Material() = default;
			inline ~Material() = default;
		public:
			// virtual Reference<Technique>& Technique(const TechniqueType& techniqueType_) = 0;
			// virtual Reference<Map>& Map(const TechniqueType& techniqueType_) = 0;
		};

		class Shape:
			public Helper::Transformation::Dimension3::BoundBox
		{
		public:
			inline Shape() = default;
			inline Shape(const BoundBox::Value min_, const BoundBox::Value max_):
				BoundBox(min_, max_)
			{
			}
			inline ~Shape() = default;
		};
		//	Connect shape with material
		class Model
		{
		public:
			inline Model() = default;
			inline ~Model() = default;
		};

		class Camera:
			public Helper::Transformation::Dimension3::ViewProjectionMatrix
		{
		public:
			inline Camera():
				ViewProjectionMatrix(
					Vec3(0.0f),
					Vec3(0.0f),
					Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, 4.0f / 3.0f, 0.1f, 1000.0f))
			{
			}
			inline Camera(const Camera&) = default;
			inline ~Camera() = default;
		public:
			inline Camera& operator = (const Camera&) = default;
		};

		class Bone:
			public Helper::Logic::Named,
			public Helper::Transformation::Dimension3::HierarchyMatrix
		{
		public:
			ModelMatrix origin = ModelMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f));
		public:
			inline Bone():
				HierarchyMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f), nullptr)
			{
			}
			inline Bone(const Bone&) = delete;
			inline ~Bone() = default;
		};
		//	Contain model & properties for it
		class Object:
			public Bone,
			public Helper::Logic::Visible,
			public Helper::Logic::Color,
			public Helper::Logic::BitGroup<UInt8>,
			public Helper::Subscrption::OnDestructionAction
		{
		protected:
			bool isBonesEnabled = false;
		public:
			inline Object():
				Bone(),
				Visible(true),
				Color(Vec4(1.0f)),
				BitGroup(0),
				OnDestructionAction()
			{
			}
			inline Object(const Object&) = delete;
			inline ~Object() = default;
		public:
			inline void EnableLocalBones()
			{
				isBonesEnabled = true;
			}
			inline void DisableLocalBones()
			{
				isBonesEnabled = false;
			}
			inline void EnableBones()
			{
				EnableLocalBones();
			}
			inline void DisableBones()
			{
				DisableLocalBones();
			}
		};

		class Light:
			public Helper::Transformation::Dimension3::HierarchyMatrix,
			public Helper::Logic::Visible,
			public Helper::Logic::Color
		{
		protected:
			Float32 ambient = 0.0f;
			Float32 fog;
		public:
			inline Light():
				HierarchyMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f)),
				Visible(true),
				Color(Vec4(1.0f))
			{
			}
			inline ~Light() = default;
		public:
			inline Float32 GetAmbient() const
			{
				return ambient;
			}
			inline void SetAmbient(const Float32& ambient_)
			{
				ambient = ambient_;
			}
			inline Float32 GetFog() const
			{
				return fog;
			}
			inline void SetFog(const Float32& fog_)
			{
				fog = fog_;
			}
		};
		namespace Lights
		{
			class Direction:
				public Light
			{
			};
			class Point:
				public Light
			{
			protected:
				Float32 rangeNear = 1.0f;
				Float32 rangeMiddle = 8.0f;
				Float32 rangeFar = 10.0f;
			public:
				inline Float32 GetRangeNear() const
				{
					return rangeNear;
				}
				inline void SetRangeNear(const Float32& rangeNear_)
				{
					rangeNear = rangeNear_;
				}
				inline Float32 GetRangeMiddle() const
				{
					return rangeMiddle;
				}
				inline void SetRangeMiddle(const Float32& rangeMiddle_)
				{
					rangeMiddle = rangeMiddle_;
				}
				inline Float32 GetRangeFar() const
				{
					return rangeFar;
				}
				inline void SetRangeFar(const Float32& rangeFar_)
				{
					rangeFar = rangeFar_;
				}
			};
			class Spot:
				public Light
			{
			protected:
				Float32 rangeNear = 1.0f;
				Float32 rangeMiddle = 8.0f;
				Float32 rangeFar = 10.0f;
				Float32 angleNear = 10.0f;
				Float32 angleMiddle = 40.0f;
				Float32 angleFar = 60.0f;
			public:
				inline Float32 GetRangeNear() const
				{
					return rangeNear;
				}
				inline void SetRangeNear(const Float32& rangeNear_)
				{
					rangeNear = rangeNear_;
				}
				inline Float32 GetRangeMiddle() const
				{
					return rangeMiddle;
				}
				inline void SetRangeMiddle(const Float32& rangeMiddle_)
				{
					rangeMiddle = rangeMiddle_;
				}
				inline Float32 GetRangeFar() const
				{
					return rangeFar;
				}
				inline void SetRangeFar(const Float32& rangeFar_)
				{
					rangeFar = rangeFar_;
				}
				inline Float32 GetAngleNear() const
				{
					return angleNear;
				}
				inline void SetAngleNear(const Float32& angleNear_)
				{
					angleNear = angleNear_;
				}
				inline Float32 GetAngleMiddle() const
				{
					return angleMiddle;
				}
				inline void SetAngle(const Float32& angleMiddle_)
				{
					angleMiddle = angleMiddle_;
				}
				inline Float32 GetAngleFar() const
				{
					return angleFar;
				}
				inline void SetAngleFar(const Float32& angleFar_)
				{
					angleFar = angleFar_;
				}
			};
		}

		class Environment;
		namespace Environments
		{
			class Cubemap:
				public Helper::Transformation::Dimension3::HierarchyMatrix,
				public Helper::Logic::Visible,
				public Helper::Logic::Priority,
				public Helper::Logic::Color
			{
			protected:
				Vec3 rangeNear = Vec3(5.0f);
				Vec3 rangeMiddle = Vec3(8.0f);
				Vec3 rangeFar = Vec3(10.0f);
			public:
				inline Cubemap():
					HierarchyMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f), nullptr),
					Visible(true),
					Priority(0),
					Color(Vec4(1.0f))
				{
				}
				inline ~Cubemap() = default;
			public:
				inline Vec3 GetRangeNear() const
				{
					return rangeNear;
				}
				inline void SetRangeNear(const Vec3& range_)
				{
					rangeNear = range_;
				}
				inline Vec3 GetRangeMiddle() const
				{
					return rangeMiddle;
				}
				inline void SetRangeMiddle(const Vec3& range_)
				{
					rangeMiddle = range_;
				}
				inline Vec3 GetRangeFar() const
				{
					return rangeFar;
				}
				inline void SetRangeFar(const Vec3& range_)
				{
					rangeFar = range_;
				}
				inline void SetRange(const Vec3& range_)
				{
					SetRangeNear(range_ * 0.1f);
					SetRangeMiddle(range_ * 0.2f);
					SetRangeFar(range_);
				}
			};
			class Globalmap:
				public Helper::Logic::Visible,
				public Helper::Logic::Color
			{
			public:
				inline Globalmap():
					Visible(true),
					Color(Vec4(1.0f))
				{
				}
				inline ~Globalmap() = default;
			};
		}

		class Decal:
			public Helper::Logic::Visible,
			public Helper::Logic::Priority,
			public Helper::Logic::Color,
			public Helper::Logic::BitGroup<UInt8>,
			public Helper::Transformation::Dimension3::HierarchyMatrix
		{
		public:
			inline Decal():
				Visible(true),
				Priority(0),
				Color(Vec4(1.0f)),
				BitGroup(0),
				HierarchyMatrix(Vec3(0.0f), Vec3(0.0f), Vec3(1.0f), nullptr)
			{
			}
			inline ~Decal() = default;
		public:
			inline bool operator < (const Decal& source)
			{
				return Priority::operator<(source);
			}
		};

		class Particle;

		namespace UserInterface
		{
			class Item;

			class Item:
				public Helper::Logic::Visible,
				public Helper::Logic::Priority,
				public Helper::Logic::Color,
				public Helper::Transformation::Dimension2::Size,
				public Helper::Transformation::Dimension2::Position,
				public Helper::Transformation::Dimension1::Angle,
				public Helper::Transformation::Dimension2::Scale
			{
			public:
				// using DrawFunction = void(*)(Item*, const Mat3&); //  std::function<void(Item*, const Mat3&)>;
				using Homogeneous = Helper::Transformation::Dimension2::Homogeneous;
			protected:
				Homogeneous center = Homogeneous(Homogeneous::Side::Center); // local (0,0) of item rect
				Homogeneous docking = Homogeneous(Homogeneous::Side::Center); // (0,0) of parent item rect
				Vector<Reference<Item>> items;
			public:
				inline Item():
					Visible(true),
					Priority(0),
					Color(Vec4(1.0f)),
					Size(Vec2(100.0f)),
					Position(Vec2(0.0f)),
					Angle(0.0f),
					Scale(Vec2(1.0f))
				{
				}
			public:
				inline void SetCenter(const Homogeneous::Value& value_)
				{
					center.SetValue(value_);
				}
				inline void SetCenter(const Homogeneous::Side& side_)
				{
					center.SetValue(side_);
				}
				inline Homogeneous::Value GetCenter() const
				{
					return center.GetValue();
				}
				inline void SetDocking(const Homogeneous::Value& value_)
				{
					docking.SetValue(value_);
				}
				inline void SetDocking(const Homogeneous::Side& side_)
				{
					docking.SetValue(side_);
				}
				inline Homogeneous::Value GetDocking() const
				{
					return docking.GetValue();
				}
				inline void Add(Reference<Item> item_)
				{
					items.push_back(item_);
				}
				inline void Remove(Reference<Item> item_)
				{
					items.erase(std::find(items.begin(), items.end(), item_));
				}
			public:
				template<class DrawFunction>
				inline void Draw(DrawFunction drawFunction_, const Mat3& transformation_, const Vec2& canvas_)
				{
					Mat3 transformation =
						transformation_*
						Move3(GetDocking())*
						Scale3(1.0f / canvas_)*

						Move3(2.0f*GetPosition())* // [0,1] => [-1,+1]: *2.0f
						Rotate3(GetAngle())*
						Scale3(GetSize())*
						Move3(-GetCenter())*

						Move3(Vec2(0.0f))
						;
					
					drawFunction_(this, transformation);

					std::sort(items.begin(), items.end(), [](Reference<Item> a, Reference<Item> b) {return a->GetPriority() < b->GetPriority(); });
					
					for(auto &item : items)
					{
						item->Draw(drawFunction_, transformation, GetSize());
					}
				}
			};
		}
	}
}


#pragma region Technique
#pragma endregion

#pragma region Map
#pragma endregion

#pragma region Material
#pragma endregion

#pragma region Model
#pragma endregion

#pragma region Object
#pragma endregion

#pragma region Scene
/*inline void GreatVEngine::Graphics::Scene::Add(Reference<Object> object_)
{
	auto objectModel = object_->GetModel();
	auto objectGeometry = objectModel->GetGeometry();
	auto objectMaterial = objectModel->GetMaterial();

	auto &models = materials[objectMaterial];
	auto &geometries = models[objectGeometry];
	geometries.insert(geometries.begin(), object_);
}
inline void GreatVEngine::Graphics::Scene::Add(Reference<Light> light_)
{
	lights.push_back(light_);
}
inline void GreatVEngine::Graphics::Scene::Remove(Reference<Object> object_)
{
	auto &modelsIt = materials.begin();
	while(modelsIt != materials.end())
	{
		auto &models = (*modelsIt).second;

		auto &geometriesIt = models.begin();
		while(geometriesIt != models.end())
		{
			auto &geometry = (*geometriesIt).second;

			geometry.erase(std::find(geometry.begin(), geometry.end(), object_));

			if(geometry.empty())
			{
				auto t = geometriesIt;
				++geometriesIt;
				models.erase(t);
			}
			else
			{
				++geometriesIt;
			}
		}

		if(models.empty())
		{
			auto t = modelsIt;
			++modelsIt;
			materials.erase(modelsIt);
		}
		else
		{
			++modelsIt;
		}
	}

	// auto objectModel = object_->GetModel();
	// auto objectGeometry = objectModel->GetGeometry();
	// auto objectMaterial = objectModel->GetMaterial();
	// 
	// auto modelsIt = materials.find(objectMaterial);
	// auto models = (*modelsIt).second;
	// auto geometriesIt = models.find(objectGeometry);
	// auto geometries = (*geometriesIt).second;
	// geometries.erase(std::find(geometries.begin(), geometries.end(), object_));
	// if(geometries.empty())
	// {
	// models.erase(geometriesIt);
	// }
	// if(models.empty())
	// {
	// materials.erase(modelsIt);
	// }
}
inline void GreatVEngine::Graphics::Scene::Remove(Reference<Light> light_)
{
	lights.erase(std::find(lights.begin(), lights.end(), light_));
}
inline void GreatVEngine::Graphics::Scene::Update(Reference<Object> object_)
{
	Remove(object_);
	Add(object_);
}
inline void GreatVEngine::Graphics::Scene::Build()
{
	for(auto &models : materials)
	{
		for(auto &geometries : models.second)
		{
			for(auto &object : geometries.second)
			{
				object;
			}
		}
	}
}
inline void GreatVEngine::Graphics::Scene::Render(Reference<Camera> camera_, Material::Techniques::Type techniqueType_)
{
	Build();
}*/
#pragma endregion


#pragma region
#pragma endregion


