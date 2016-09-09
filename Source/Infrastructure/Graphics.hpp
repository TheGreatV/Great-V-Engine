#pragma region Include
#pragma once


#include "Header.hpp"
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
		public:
			virtual Link<Context> GetContext() const = 0;
		};

		//	Logic of material rendering
		class Technique
		{
		public:
			inline static Reference<Technique> Load(const Filename& filename_);
		public:
			inline Technique() = default;
			inline ~Technique() = default;
		};
		//	Map for material rendering
		class Map
		{
		public:
			inline static Reference<Map> Load(const Filename& filename_);
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

		class Shape
		{
		public:
			inline Shape() = default;
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
			public Helper::Transformation::Dimension3::ViewMatrix
		{
		public:
			inline Camera() = default;
			inline Camera(const Camera&) = default;
			inline ~Camera() = default;
		public:
			inline Camera& operator = (const Camera&) = default;
		};

		//	Contain model & properties for it
		class Object:
			public Helper::Logic::Visible,
			public Helper::Logic::Color,
			public Helper::Transformation::Dimension3::HierarchyMatrix,
			public Helper::Subscrption::Destruction
		{
		public:
			inline Object() = default;
			inline ~Object() = default;
		};

		class Light
		{
		public:
			inline Light() = default;
			inline ~Light() = default;
		};

		class Environment;

		class Particle;
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


