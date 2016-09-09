#pragma region Include
#pragma once


#include "Header.hpp"
#include "OpenGL.hpp"
#include "Wrapper.hpp"


#include <Infrastructure/Graphics.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace OpenGL
	{
		class Scene:
			public Graphics::Scene
		{
		protected:
			const Link<Context> context;
		public:
			inline Scene(Link<Context> context_):
				context(context_)
			{
			}
			virtual ~Scene() override = default;
		public:
			virtual Link<Graphics::Context> GetContext() const;
		public:
			inline void Add(Reference<Graphics::Object> object_);
			inline void Add(Reference<Graphics::Light> light_);
		};

		class Technique:
			public Graphics::Technique
		{
		};
		class Map:
			public Graphics::Map
		{
		};
		class Material:
			public Graphics::Material
		{
		protected:
			Array<Reference<Technique>, TechniquesCount> techniques;
			Array<Reference<Map>, MapsCount> maps;
		public:
			inline Material() = default;
			inline ~Material() = default;
		public:
			inline Reference<Technique>& technique(const TechniqueType& techniqueType_)
			{
				return techniques[(Size)techniqueType_];
			}
			inline Reference<Map>& map(const MapType& mapType_)
			{
				return maps[(Size)mapType_];
			}
		};

		class Shape:
			public Graphics::Shape
		{
		public:
			inline Shape(Link<Geometry> geometry_)
			{
			}
			inline ~Shape() = default;
		};

		class Model:
			public Graphics::Model
		{
		protected:
			const Reference<Shape> shape;
			const Reference<Material> material;
		public:
			inline Model(Reference<Shape> shape_, Reference<Material> material_):
				shape(shape_),
				material(material_)
			{
			}
			inline ~Model() = default;
		public:
			inline Reference<Shape> GetShape() const
			{
				return shape;
			}
			inline Reference<Material> GetMaterial() const
			{
				return material;
			}
		};

		class Object:
			public Graphics::Object
		{
		protected:
			Reference<Model> model = nullptr;
		public:
			inline Reference<Model> GetModel() const
			{
				return model;
			}
			inline void SetModel(Reference<Model> model_)
			{
				model = model_;
			}
		};
	}
}


#pragma region Scene
inline void GreatVEngine::OpenGL::Scene::Add(Reference<Graphics::Object> object_)
{

}
inline void GreatVEngine::OpenGL::Scene::Add(Reference<Graphics::Light> light_)
{

}
#pragma endregion


#pragma region
#pragma endregion

