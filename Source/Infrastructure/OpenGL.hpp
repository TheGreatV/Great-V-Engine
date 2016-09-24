#pragma region Include
#pragma once


#include "Graphics.hpp"


#include <APIs/OpenGL/Wrapper.hpp>
#include <Utilities/OpenIL/Functions.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace OpenGL
	{
		class Technique;
		class Map;
		class Material;
		class Shape;
		class Model;
		class Object;
		class Scene;

		class Technique:
			public Graphics::Technique
		{
			friend Scene;
			friend Map;
			friend Material;
			friend Model;
			friend Shape;
			friend Object;
		protected:
			const Reference<Context> context;
			Shader*const vertex;
			Shader*const tessellationControl;
			Shader*const tessellationEvaluation;
			Shader*const geometry;
			Shader*const fragment;
			Program*const program;
		public:
			inline Technique(
				Reference<Context> context_,
				const Shader::Source& sourceVertex_,
				const Shader::Source& sourceTessellationControl_,
				const Shader::Source& sourceTessellationEvaluation_,
				const Shader::Source& sourceGeometry_,
				const Shader::Source& sourceFragment_):
				context(context_),
				vertex(!sourceVertex_.empty() ? new Shader(Shader::Type::Vertex, sourceVertex_) : nullptr),
				tessellationControl(!sourceTessellationControl_.empty() ? new Shader(Shader::Type::TessellationControl, sourceTessellationControl_) : nullptr),
				tessellationEvaluation(!sourceTessellationEvaluation_.empty() ? new Shader(Shader::Type::TessellationEvaluation, sourceTessellationEvaluation_) : nullptr),
				geometry(!sourceGeometry_.empty() ? new Shader(Shader::Type::Geometry, sourceGeometry_) : nullptr),
				fragment(!sourceFragment_.empty() ? new Shader(Shader::Type::Fragment, sourceFragment_) : nullptr),
				program(new Program(context.get(), {vertex, tessellationControl, tessellationEvaluation, geometry, fragment}))
			{
			}
			inline ~Technique()
			{
				delete program;

				if(vertex)
				{
					delete vertex;
				}
				if(tessellationControl)
				{
					delete tessellationControl;
				}
				if(tessellationEvaluation)
				{
					delete tessellationEvaluation;
				}
				if(geometry)
				{
					delete geometry;
				}
				if(fragment)
				{
					delete fragment;
				}
			}
		public:
			static Reference<Technique> Load(
				Reference<Context> context_,
				const Filename& base_,
				const Filename& vertex_,
				const Filename& tessellationControl_,
				const Filename& tessellationEvaluation_,
				const Filename& geometry_,
				const Filename& fragment_)
			{
				auto LoadFile = [](const Filename& filename_)
				{
					FILE* file = nullptr;
					if(fopen_s(&file, filename_.c_str(), "rb") != 0)
					{
						throw Exception("failed to load file");
					}
					fseek(file, 0, FILE_END);
					auto size = ftell(file);

					rewind(file);

					Shader::Source result;
					result.resize(size);
					fread((void*)result.data(), 1, size, file);

					fclose(file);

					return std::move(result);
				};

				if(vertex_.size() > 0)
				{
					auto source = LoadFile(base_ + vertex_);
				}

				return MakeReference(new Technique(
					context_,
					vertex_.size() > 0 ? LoadFile(base_ + vertex_) : Shader::Source(),
					tessellationControl_.size() > 0 ? LoadFile(base_ + tessellationControl_) : Shader::Source(),
					tessellationEvaluation_.size() > 0 ? LoadFile(base_ + tessellationEvaluation_) : Shader::Source(),
					geometry_.size() > 0 ? LoadFile(base_ + geometry_) : Shader::Source(),
					fragment_.size() > 0 ? LoadFile(base_ + fragment_) : Shader::Source()));
			}
		};
		class Map:
			public Graphics::Map
		{
			friend Scene;
			friend Technique;
			friend Material;
			friend Model;
			friend Shape;
			friend Object;
		protected:
			Texture*const texture;
		protected:
			inline Map(Texture* texture_):
				texture(texture_)
			{
			}
		public:
			inline ~Map()
			{
				delete texture;
			}
		public:
			inline static Reference<Map> Load2D(Reference<Context> context_, const Filename& filename_)
			{
				auto image = OpenIL::Image::Load2D(filename_);
				
				auto texture = new OpenGL::Texture(
					context_.get(),
					image->GetWidth(), image->GetHeight(), image->GetDepth(),
					OpenGL::Texture::Type::D2, OpenGL::Texture::InternalFormat::RGBA8,
					OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
					OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
					OpenGL::Texture::Wrap::Repeat, OpenGL::Texture::Filter::Linear, image->GetRawData());
				
				return MakeReference(new Map(texture));
			}
		};
		class Material:
			public Graphics::Material
		{
			friend Scene;
			friend Technique;
			friend Map;
			friend Model;
			friend Shape;
			friend Object;
		protected:
			const Reference<Context> context;
			Array<Reference<Technique>, TechniquesCount> techniques;
			Array<Reference<Map>, MapsCount> maps;
		public:
			inline Material(Reference<Context> context_):
				context(context_)
			{
			}
			inline ~Material() = default;
		public:
			inline Reference<Technique>& Technique(const TechniqueType& techniqueType_)
			{
				return techniques[(Size)techniqueType_];
			}
			inline Reference<Map>& Map(const MapType& mapType_)
			{
				return maps[(Size)mapType_];
			}
		protected:
			inline void Set(const Material::TechniqueType& techniqueType_)
			{
				for(Size i = 0; i < MapsCount; ++i)
				{
					auto &map = maps[i];

					if(map)
					{
						map->texture->Set(i);
					}
				}

				if(techniques[(Size)techniqueType_])
				{
					techniques[(Size)techniqueType_]->program->Set();
				}
				else
				{
					throw Exception("No technique available");
				}
			}
		};

		class Shape:
			public Graphics::Shape
		{
			friend Scene;
			friend Technique;
			friend Map;
			friend Material;
			friend Model;
			friend Object;
		public:
			static const Geometry::VertexPackMode vertexPackMode = Geometry::VertexPackMode::Pos32F_TBN32F_Tex32F;
			static const Geometry::IndexPackMode indexPackMode = Geometry::IndexPackMode::UInt32;
		protected:
			const Reference<Context> context;
			Buffers::Array* vertices;
			Buffers::Index* indices;
			Size verticesCount;
			Size indicesCount;
		public:
			inline Shape(Reference<Context> context_, Reference<Geometry> geometry_):
				context(context_),
				vertices(new OpenGL::Buffers::Array(context.get(), *geometry_->GetVertices(vertexPackMode).get())),
				indices(new OpenGL::Buffers::Index(context.get(), *geometry_->GetIndices(indexPackMode).get())),
				verticesCount(geometry_->vertices.size()),
				indicesCount(geometry_->indices.size())
			{
			}
			inline ~Shape()
			{
				delete vertices;
				delete indices;
			}
		protected:
			inline void Set()
			{
				vertices->Set();
				indices->Set();
			}
		};

		class Model:
			public Graphics::Model
		{
			friend Scene;
			friend Technique;
			friend Map;
			friend Material;
			friend Shape;
			friend Object;
		protected:
			const Reference<Context> context;
			const Reference<Shape> shape;
			const Reference<Material> material;
			const Array<Buffers::Attribute*, Material::TechniquesCount> attributes;
		public:
			inline Model(Reference<Context> context_, Reference<Shape> shape_, Reference<Material> material_):
				context(context_),
				shape(shape_),
				material(material_),
				attributes([&]()
				{
					Array<Buffers::Attribute*, Material::TechniquesCount> attributes;

					for(Size i = 0; i < attributes.size(); ++i)
					{
						auto technique = material->Technique((Material::TechniqueType)i);

						if(technique)
						{
							auto vertices = shape->vertices;
							auto indices = shape->indices;
							auto program = technique->program;

							vertices->Set();
							indices->Set();
							program->Set();

							attributes[i] = new Buffers::Attribute(
								context.get(),
								program,
								shape->vertices,
								shape->indices, {
									Buffers::Attribute::Data(
										program->GetAttribute("vPos"),
										Buffers::Attribute::Data::Type::Float32, 3, false,
										Geometry::GetVertexSize(shape->vertexPackMode), 0),
									Buffers::Attribute::Data(
										program->GetAttribute("vTan"),
										Buffers::Attribute::Data::Type::Float32, 3, false,
										Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)*3),
									Buffers::Attribute::Data(
										program->GetAttribute("vBin"),
										Buffers::Attribute::Data::Type::Float32, 3, false,
										Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)* 6),
									Buffers::Attribute::Data(
										program->GetAttribute("vNor"),
										Buffers::Attribute::Data::Type::Float32, 3, false,
										Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)* 9),
									Buffers::Attribute::Data(
										program->GetAttribute("vTex"),
										Buffers::Attribute::Data::Type::Float32, 2, false,
										Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)* 12),
								});
						}
						else
						{
							attributes[i] = nullptr;
						}
					}

					return attributes;
				}())
			{
			}
			inline ~Model()
			{
				for(auto &attribute : attributes)
				{
					delete attribute;
				}
			}
		public:
			inline Reference<Shape> GetShape() const
			{
				return shape;
			}
			inline Reference<Material> GetMaterial() const
			{
				return material;
			}
		protected:
			inline void Set(const Material::TechniqueType& techniqueType_)
			{
				material->Set(techniqueType_);
				attributes[(Size)techniqueType_]->Set();
				shape->Set();
			}
			inline void DrawIndexed()
			{
				glDrawElements(GL_TRIANGLES, shape->indicesCount, GL_UNSIGNED_INT, 0);
			}
		};

		class Object:
			public Graphics::Object
		{
			friend Scene;
			friend Technique;
			friend Map;
			friend Material;
			friend Model;
			friend Shape;
		protected:
			Reference<Model> model = nullptr;
		public:
			inline Reference<Model> GetModel() const
			{
				return model;
			}
			inline void SetModel(Reference<Model> model_)
			{
				onModelChange(model_);
				model = model_;
			}
		};

		class Scene:
			public Graphics::Scene
		{
			friend Technique;
			friend Map;
			friend Material;
			friend Model;
			friend Shape;
			friend Object;
		protected:
			const Reference<Context> context;
			Vector<Reference<Object>> objects;
		public:
			inline Scene(Reference<Context> context_):
				context(context_)
			{
			}
			virtual ~Scene() override = default;
		public:
			inline void Add(Reference<Object> object_);
			inline void Remove(Reference<Object> object_);
			inline void Render(Reference<Graphics::Camera> camera_, const Material::TechniqueType& techniqueType_ = Material::TechniqueType::Basic);
		};
	}
}


#pragma region Scene
inline void GreatVEngine::OpenGL::Scene::Add(Reference<OpenGL::Object> object_)
{
	struct T
	{
		Scene* scene;
		Reference<OpenGL::Object> object;
	};

	object_->Subscribe_OnModelChange(new T{this, object_}, [](void* data, Reference<Graphics::Model> model_){
		auto t = (T*)data;
		auto $this = t->scene;

		$this->Remove(t->object);
		$this->Add(t->object);

		return true;
	});

	objects.push_back(object_);
}
inline void GreatVEngine::OpenGL::Scene::Remove(Reference<OpenGL::Object> object_)
{
	objects.erase(std::find(objects.begin(), objects.end(), object_));
}
inline void GreatVEngine::OpenGL::Scene::Render(Reference<Graphics::Camera> camera_, const Material::TechniqueType& techniqueType_)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);

	glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 800, 600);

	for(auto &object : objects)
	{
		auto &model = object->model;

		auto shape = model->shape;
		auto material = model->material;
		auto program = model->material->techniques[(Size)techniqueType_]->program;

		model->Set(techniqueType_);

		program->SetInt("textureColor", 0);
		program->SetInt("textureNormals", 1);
		program->SetInt("textureSpecular", 2);

		program->SetMat4("modelViewProjectionMatrix", camera_->GetVPMat() * object->GetMMat());
		program->SetMat4("modelMatrix", Move4(-camera_->GetPosition()) * object->GetMMat());
		program->SetMat3("rotateMatrix", object->GetRMat());

		model->DrawIndexed();
	}

	glFlush();
}
#pragma endregion

#pragma region
#pragma endregion

