#pragma region Include
#pragma once


#include "Graphics.hpp"


#include <APIs/OpenGL/Wrapper.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Utilities/OpenIL/Functions.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace OpenGL
	{
		class Engine;
		class Technique;
		class Map;
		class Material;
		class Shape;
		class Model;
		class Object;
		class Scene;
		class Light;
		namespace Lights
		{
			class Direction;
			class Point;
			class Spot;
		}
		namespace Environments
		{
			class Cubemap;
			class Globalmap;
		};
		class Decal;

		class Engine
		{
			friend Technique;
			friend Map;
			friend Material;
			friend Shape;
			friend Model;
			friend Object;
			friend Scene;
		public: // protected:
			Context*const context;
		protected:
			inline Texture* LoadTexture(const Filename& filename_)
			{
				auto image = OpenIL::Image::Load2D(filename_);

				if(image->GetOriginMode() != OpenIL::Image::Origin::LowerLeft)
				{
					image->Flip();
				}
				
				auto texture = new OpenGL::Texture(
					context,
					image->GetWidth(), image->GetHeight(), image->GetDepth(),
					OpenGL::Texture::Type::D2, OpenGL::Texture::InternalFormat::RGBA8,
					OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
					OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
					OpenGL::Texture::Wrap::Repeat, OpenGL::Texture::Filter::Linear, image->GetRawData());

				return texture;
			}
		public:
			inline Engine(Reference<OpenGL::WinAPI::Context::DeviceContext> deviceContext_):
				context(new OpenGL::WinAPI::ExtendedContext(deviceContext_))
			{
				context->Set();
			}
			inline ~Engine()
			{
				delete context;
			}
		};

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
			const Reference<Engine> engine;
			Shader*const vertex;
			Shader*const tessellationControl;
			Shader*const tessellationEvaluation;
			Shader*const geometry;
			Shader*const fragment;
			Program*const program;
		public:
			inline Technique(
				Reference<Engine> engine_,
				const Shader::Source& sourceVertex_,
				const Shader::Source& sourceTessellationControl_,
				const Shader::Source& sourceTessellationEvaluation_,
				const Shader::Source& sourceGeometry_,
				const Shader::Source& sourceFragment_):
				engine(engine_),
				vertex(!sourceVertex_.empty() ? new Shader(Shader::Type::Vertex, sourceVertex_) : nullptr),
				tessellationControl(!sourceTessellationControl_.empty() ? new Shader(Shader::Type::TessellationControl, sourceTessellationControl_) : nullptr),
				tessellationEvaluation(!sourceTessellationEvaluation_.empty() ? new Shader(Shader::Type::TessellationEvaluation, sourceTessellationEvaluation_) : nullptr),
				geometry(!sourceGeometry_.empty() ? new Shader(Shader::Type::Geometry, sourceGeometry_) : nullptr),
				fragment(!sourceFragment_.empty() ? new Shader(Shader::Type::Fragment, sourceFragment_) : nullptr),
				program(new Program(engine->context, {vertex, tessellationControl, tessellationEvaluation, geometry, fragment}))
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
				Reference<Engine> engine_,
				const Filename& base_,
				const Filename& vertex_,
				const Filename& tessellationControl_,
				const Filename& tessellationEvaluation_,
				const Filename& geometry_,
				const Filename& fragment_)
			{
				return MakeReference(new Technique(
					engine_,
					vertex_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + vertex_) : Shader::Source(),
					tessellationControl_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + tessellationControl_) : Shader::Source(),
					tessellationEvaluation_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + tessellationEvaluation_) : Shader::Source(),
					geometry_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + geometry_) : Shader::Source(),
					fragment_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + fragment_) : Shader::Source()));
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
			inline static Reference<Map> Load2D(Reference<Engine> engine_, const Filename& filename_)
			{
				/*auto image = OpenIL::Image::Load2D(filename_);
				
				auto texture = new OpenGL::Texture(
					engine_->context,
					image->GetWidth(), image->GetHeight(), image->GetDepth(),
					OpenGL::Texture::Type::D2, OpenGL::Texture::InternalFormat::RGBA8,
					OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
					OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
					OpenGL::Texture::Wrap::Repeat, OpenGL::Texture::Filter::Linear, image->GetRawData());*/
				
				return MakeReference(new Map(engine_->LoadTexture(filename_))); // texture));
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
			const Reference<Engine> engine;
			Array<Reference<Technique>, TechniquesCount> techniques;
			Array<Reference<Map>, MapsCount> maps;
		public:
			inline Material(Reference<Engine> engine_):
				engine(engine_)
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
		public:
			inline void SetValue(const String& name_, const Float32& value_)
			{
				for(auto &technique : techniques)
				{
					if(technique)
					{
						technique->program->Set();
						technique->program->SetFloat(name_, value_);
					}
				}
			}
			inline void SetValue(const String& name_, const Vec3& value_)
			{
				for(auto &technique : techniques)
				{
					if(technique)
					{
						technique->program->Set();
						technique->program->SetVec3(name_, value_);
					}
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
			const Reference<Engine> engine;
			Buffers::Array* vertices;
			Buffers::Index* indices;
			Size verticesCount;
			Size indicesCount;
		public:
			inline Shape(Reference<Engine> engine_, Reference<Geometry> geometry_):
				engine(engine_),
				vertices(new OpenGL::Buffers::Array(engine->context, *geometry_->GetVertices(vertexPackMode).get())),
				indices(new OpenGL::Buffers::Index(engine->context, *geometry_->GetIndices(indexPackMode).get())),
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
			const Reference<Engine> engine;
			const Reference<Shape> shape;
			const Reference<Material> material;
			const Array<Buffers::Attribute*, Material::TechniquesCount> attributes;
		public:
			inline Model(Reference<Engine> engine_, Reference<Shape> shape_, Reference<Material> material_):
				engine(engine_),
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
								engine->context,
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
		public:
			inline bool operator < (const Model& source_)
			{
				if(material.get() == source_.material.get())
				{
					return shape.get() < source_.shape.get();
				}

				return material.get() < source_.material.get();
			}
		};

		class Object:
			public Shared<Object>,
			public Graphics::Object
		{
			friend Scene;
			friend Technique;
			friend Map;
			friend Material;
			friend Model;
			friend Shape;
		protected:
			inline static Reference<Object> Load(Reference<System::BinaryFileReader> reader_, Vector<Reference<Geometry>>& meshes_, Reference<Material> material_);
		public:
			inline static Reference<Object> Load(const Filename& filename_, Reference<Material> material_);
		protected:
			Reference<Model> model = nullptr;
			Reference<Object> parent;
			Vector<Reference<Object>> childs;
		protected:
			inline void AddChild(Reference<Object> object_)
			{
				childs.push_back(object_);
			}
			inline void RemoveChild(Reference<Object> object_)
			{
				childs.erase(std::find(childs.begin(), childs.end(), object_));
			}
		public:
			inline Reference<Object> operator [] (const Name& name_)
			{
				for(auto &child : childs)
				{
					if(child->GetName() == name_)
					{
						return child;
					}
				}

				return nullptr;
			}
			inline Reference<Object> GetParentObject() const
			{
				return parent;
			}
			inline void SetParent(HierarchyMatrix::Parent parent_)
			{
				if(parent)
				{
					parent->RemoveChild(shared_from_this());
				}

				HierarchyMatrix::SetParent(parent_);
			}
			inline void SetParent(Reference<Object> parent_)
			{
				SetParent((HierarchyMatrix::Parent)parent_.get());

				parent = parent_;

				if(parent)
				{
					parent->AddChild(shared_from_this());
				}
			}
			inline Reference<Model> GetModel() const
			{
				return model;
			}
			inline void SetModel(Reference<Model> model_)
			{
				onModelChange(model_);

				model = model_;
			}
		public:
			inline bool operator < (const Object& source_)
			{
				if(model != nullptr)
				{
					if(source_.model != nullptr)
					{
						return *model < *source_.model;
					}

					return true;
				}

				return false;
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
		public:
			using Viewport = Size2;
		protected:
			struct Target
			{
			public:
				Texture*const diffuse;
				Texture*const specular;
				Buffers::Frame*const frameComplete;
				Buffers::Frame*const frameSpecular;
			public:
				inline Target(Context* context_, const Viewport& viewport_):
					diffuse(new Texture( // RGB - diffuse total color, A - diffuse intensity (for environment calculation)
						context_, viewport_.x, viewport_.y, 1, Texture::Type::D2,
						Texture::InternalFormat::RGBA16F, Texture::Format::RGBA, Texture::ComponentType::SFloat16,
						Texture::Wrap::Clamp, Texture::Filter::Off, nullptr)),
					specular(new Texture( // RGB - specular total color, A - environment intensity (for composition)
						context_, viewport_.x, viewport_.y, 1, Texture::Type::D2,
						Texture::InternalFormat::RGBA16F, Texture::Format::RGBA, Texture::ComponentType::SFloat16,
						Texture::Wrap::Clamp, Texture::Filter::Off, nullptr)),
					frameComplete(new Buffers::Frame(context_, {diffuse, specular}, nullptr, nullptr)),
					frameSpecular(new Buffers::Frame(context_, {specular}, nullptr, nullptr))
				{
				}
			};
			struct Presenter
			{
			public:
				Program*const program;
				Buffers::Array*const vertices;
				Buffers::Attribute*const attributes;
			public:
				inline Presenter(Context* context_):
					program([&](){
						auto program_ = new Program(context_, {
							new Shader(Shader::Type::Vertex, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Screen Quad/Drawer 2D/1.vs"))),
							new Shader(Shader::Type::Geometry, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Screen Quad/Drawer 2D/1.gs"))),
							new Shader(Shader::Type::Fragment, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Screen Quad/Drawer 2D/1.fs"))),
						});

						program_->Set();

							return program_;
					}()),
					vertices(new Buffers::Array(context_, 1, nullptr)),
					attributes(new Buffers::Attribute(context_, program, vertices, nullptr, {}))
				{
					program->SetInt("textureColor", 0);
				}
			};
			struct GeometyPass
			{
			public:
				// /*
				// */
				// Texture*const color;	//	R8	G8	B8	XX	colorRGB
				// Texture*const normals;	//	R8	G8	B8	XX	encodedNormal2, roughness
				// Texture*const normals;	//	R8	G8	B8	XX	gloss, metallic, ???
				// Texture*const depth;	//	D24	S8
				/*
				Color + Alpha
				Specular + Roughness
				Normals + Height
				*/
			public:
				Texture*const color;	//	R8	G8	B8	XX	colorRGB
				Texture*const specular;	//	R8	G8	B8	XX	specularRGB
				Texture*const normals;	//	R16	G16	B16	XX	normalXYZ
				Texture*const material;	//	R8	XX	XX	XX	roughness, ???, ???
				Texture*const depthStencil;	//	D24	S8
				Buffers::Frame*const frame;
			public:
				inline GeometyPass(Context* context_, const Viewport& viewport_):
					color(new Texture(context_, viewport_.x, viewport_.y, 1,
						Texture::Type::D2, Texture::InternalFormat::RGBA8, Texture::Format::RGBA, Texture::ComponentType::UInt8,
						Texture::Wrap::Clamp, Texture::Filter::Off, nullptr)),
					specular(new Texture(context_, viewport_.x, viewport_.y, 1,
						Texture::Type::D2, Texture::InternalFormat::RGBA8, Texture::Format::RGBA, Texture::ComponentType::UInt8,
						Texture::Wrap::Clamp, Texture::Filter::Off, nullptr)),
					normals(new Texture(context_, viewport_.x, viewport_.y, 1,
						Texture::Type::D2, Texture::InternalFormat::RGBA16F, Texture::Format::RGBA, Texture::ComponentType::SFloat16,
						Texture::Wrap::Clamp, Texture::Filter::Off, nullptr)),
					material(new Texture(context_, viewport_.x, viewport_.y, 1,
						Texture::Type::D2, Texture::InternalFormat::RGBA8, Texture::Format::RGBA, Texture::ComponentType::UInt8,
						Texture::Wrap::Clamp, Texture::Filter::Off, nullptr)),
					depthStencil(new Texture(context_, viewport_.x, viewport_.y, 1,
						Texture::Type::D2, Texture::InternalFormat::Depth24Stencil8, Texture::Format::DepthStencil, Texture::ComponentType::UInt24_8,
						Texture::Wrap::Clamp, Texture::Filter::Off, nullptr)),
					frame(new Buffers::Frame(context_, {color, specular, normals, material}, depthStencil))
				{
					frame->Reset();
				}
				inline ~GeometyPass()
				{
					delete frame;
					delete color;
					delete specular;
					delete normals;
					delete material;
					delete depthStencil;
				}
			};
			struct DecalPass
			{
			public:
				Program*const program;
				Buffers::Array* vertices;
				Buffers::Attribute* attributes;
				Buffers::Frame*const frame;
				Texture* atlasAlbedo;
				Texture* atlasTopology;
				Texture* atlasMaterial;
			public:
				inline DecalPass(Context* context_, GeometyPass* geometyPass_):
					program([&](){
						auto program_ = new Program(context_, {
							new Shader(Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Decals/1.glsl.vs"))),
							new Shader(Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Decals/1.glsl.gs"))),
							new Shader(Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Decals/1.glsl.fs"))),
					});

					program_->Set();

					return program_;
				}()),
					vertices(new Buffers::Array(context_, 1, nullptr)),
					attributes(new Buffers::Attribute(context_, program, vertices, nullptr, {})),
					frame(new Buffers::Frame(context_, {
						geometyPass_->color,
						geometyPass_->specular,
						geometyPass_->normals,
						geometyPass_->material,
					}, geometyPass_->depthStencil))
				{
					program->SetInt("textureAlbedo", 0);
					program->SetInt("textureTopology", 1);
					program->SetInt("textureMaterial", 2);
					program->SetInt("textureDepth", 3);

					frame->Reset();
				}
				inline ~DecalPass()
				{
					delete frame;
					delete attributes;
					delete vertices;
					delete program;
				}
			};
			struct LightPass
			{
			public:
				struct Direction
				{
				public:
					Program*const program;
					Buffers::Array*const vertices;
					Buffers::Attribute*const attributes;
				public:
					inline Direction(Context* context_):
						program([&](){
							auto program_ = new Program(context_, {
								new Shader(Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Direction Light/Basic/1.glsl.vs"))),
								new Shader(Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Direction Light/Basic/1.glsl.gs"))),
								new Shader(Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Direction Light/Basic/1.glsl.fs"))),
							});

							program_->Set();

							return program_;
						}()),
						vertices(new Buffers::Array(context_, 1, nullptr)),
						attributes(new Buffers::Attribute(context_, program, vertices, nullptr, {}))
					{
						program->SetInt("textureColor", 0);
						program->SetInt("textureSpecular", 1);
						program->SetInt("textureNormal", 2);
						program->SetInt("textureMaterial", 3);
						program->SetInt("textureDepth", 4);
					}
				};
				struct Point
				{
				public:
					Program*const program;
					Buffers::Array* vertices;
					Buffers::Attribute* attributes;
				public:
					inline Point(Context* context_):
						program([&](){
						auto program_ = new Program(context_, {
							new Shader(Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Point Light/Basic/1.glsl.vs"))),
							new Shader(Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Point Light/Basic/1.glsl.gs"))),
							new Shader(Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Point Light/Basic/1.glsl.fs"))),
						});

						program_->Set();

						return program_;
					}()),
						vertices(new Buffers::Array(context_, 1, nullptr)),
						attributes(new Buffers::Attribute(context_, program, vertices, nullptr, {}))
					{
						program->SetInt("textureColor", 0);
						program->SetInt("textureSpecular", 1);
						program->SetInt("textureNormal", 2);
						program->SetInt("textureMaterial", 3);
						program->SetInt("textureDepth", 4);
					}
				};
			public:
				Direction*const direction;
				Point*const point;
			public:
				inline LightPass(Context* context_):
					direction(new Direction(context_)),
					point(new Point(context_))
				{
				}
				inline ~LightPass()
				{
					delete direction;
				}
			};
			struct EnvironmentPass
			{
			public:
				struct Globalmap
				{
				public:
					Sampler* sampler;
					Program*const program;
					Buffers::Array* vertices;
					Buffers::Attribute* attributes;
				public:
					inline Globalmap(Context* context_):
						sampler(new Sampler(context_, Sampler::Wrap::Clamp, Sampler::Filter::Mipmap)),
						program([&](){
							auto program_ = new Program(context_, {
								new Shader(Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Globalmap Environment/1.glsl.vs"))),
								new Shader(Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Globalmap Environment/1.glsl.gs"))),
								new Shader(Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Globalmap Environment/1.glsl.fs"))),
							});

							program_->Set();

							return program_;
						}()),
						vertices(new Buffers::Array(context_, 1, nullptr)),
						attributes(new Buffers::Attribute(context_, program, vertices, nullptr, {}))
					{
						program->SetInt("textureColor", 0);
						program->SetInt("textureSpecular", 1);
						program->SetInt("textureNormal", 2);
						program->SetInt("textureMaterial", 3);
						program->SetInt("textureDepth", 4);
						program->SetInt("textureDiffuse", 5);
						program->SetInt("textureEnvironment", 8);
					}
				};
				struct Cubemap
				{
				public:
					Sampler* sampler;
					Program*const program;
					Buffers::Array* vertices;
					Buffers::Attribute* attributes;
				public:
					inline Cubemap(Context* context_):
						sampler(new Sampler(context_, Sampler::Wrap::Clamp, Sampler::Filter::Mipmap)),
						program([&](){
							auto program_ = new Program(context_, {
								new Shader(Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Cubemap Environment/1.glsl.vs"))),
								new Shader(Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Cubemap Environment/1.glsl.gs"))),
								new Shader(Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Cubemap Environment/1.glsl.fs"))),
							});

							program_->Set();

							return program_;
						}()),
						vertices(new Buffers::Array(context_, 1, nullptr)),
						attributes(new Buffers::Attribute(context_, program, vertices, nullptr, {}))
					{
						program->SetInt("textureColor", 0);
						program->SetInt("textureSpecular", 1);
						program->SetInt("textureNormal", 2);
						program->SetInt("textureMaterial", 3);
						program->SetInt("textureDepth", 4);
						program->SetInt("textureDiffuse", 5);
						program->SetInt("textureEnvironment", 8);
					}
				};
			public:
				Cubemap*const cubemap;
				Globalmap*const globalmap;
				bool isEnabled_ScreenSpaceLocalReflections = true;
			public:
				inline EnvironmentPass(Context* context_):
					cubemap(new Cubemap(context_)),
					globalmap(new Globalmap(context_))
				{
				}
				inline ~EnvironmentPass()
				{
					delete cubemap;
					delete globalmap;
				}
			};
			struct Drawer
			{
			public:
				struct Line
				{
				public:
					Vec2 p1, p2;
					Vec4 c1, c2;
				};
			public:
				Program*const program;
				Buffers::Array* vertices;
				Buffers::Attribute* attributes;
				Vector<Line> lines;
			public:
				inline Drawer(Context* context_):
					program([&](){
						auto program_ = new Program(context_, {
							new Shader(Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/2D/1.glsl.vs"))),
							new Shader(Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/2D/1.glsl.gs"))),
							new Shader(Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/2D/1.glsl.fs"))),
						});

						program_->Set();

						return program_;
					}()),
					vertices(new Buffers::Array(context_, 1, nullptr)),
					attributes(new Buffers::Attribute(context_, program, vertices, nullptr, {}))
				{
				}
			};
		protected:
			const Reference<Engine> engine;
			Viewport viewport;
			Target* target;
			GeometyPass* geometyPass;
			DecalPass* decalPass;
			LightPass* lightPass;
			EnvironmentPass* environmentPass;
			Drawer* drawer;
			Presenter* presenter;
			Vector<Reference<Object>> objects;
			Vector<Reference<Lights::Direction>> lightsDirection;
			Vector<Reference<Lights::Point>> lightsPoint;
			Vector<Reference<Lights::Spot>> lightsSpot;
			Vector<Reference<Environments::Cubemap>> environmentsCubemap;
			Vector<Reference<Environments::Globalmap>> environmentsGlobalmap;
			Dictionary<Graphics::Decal::BitGroup::Value, Vector<Reference<Decal>>> decals;
		public:
			inline Scene(Reference<Engine> engine_, const Viewport& viewport_):
				engine(engine_),
				viewport(viewport_),
				target(new Target(engine_->context, viewport_)),
				geometyPass(new GeometyPass(engine->context, viewport_)),
				decalPass(new DecalPass(engine->context, geometyPass)),
				lightPass(new LightPass(engine->context)),
				environmentPass(new EnvironmentPass(engine->context)),
				drawer(new Drawer(engine->context)),
				presenter(new Presenter(engine->context))
			{
			}
			virtual ~Scene() override
			{
				delete geometyPass;
				delete lightPass;
				delete presenter;
			}
		public:
			inline void LoadDecals(const Filename& albedo_, const Filename& topology_, const Filename& material_)
			{
				decalPass->atlasAlbedo = engine->LoadTexture(albedo_);
				decalPass->atlasTopology = engine->LoadTexture(topology_);
				decalPass->atlasMaterial = engine->LoadTexture(material_);
			}
		public:
			inline void Add(Reference<Object> object_);
			inline void Add(Reference<Lights::Direction> light_);
			inline void Add(Reference<Lights::Point> light_);
			inline void Add(Reference<Lights::Spot> light_);
			inline void Add(Reference<Environments::Cubemap> environment_);
			inline void Add(Reference<Environments::Globalmap> environment_);
			inline void Add(Reference<Decal> decal_);
			inline void Remove(Reference<Object> object_);
			inline void Remove(Reference<Lights::Direction> light_);
			inline void Remove(Reference<Lights::Point> light_);
			inline void Remove(Reference<Lights::Spot> light_);
			inline void Remove(Reference<Environments::Cubemap> environment_);
			inline void Remove(Reference<Environments::Globalmap> environment_);
			inline void Remove(Reference<Decal> decal_);
			inline void Render(Reference<Graphics::Camera> camera_, const Material::TechniqueType& techniqueType_ = Material::TechniqueType::Basic);
		public:
			inline void DrawLine(const Vec2& p1_, const Vec2& p2_, const Vec4& c1_, const Vec4& c2_)
			{
				drawer->lines.push_back({p1_, p2_, c1_, c2_});
			}
			inline void DrawLine(const Vec2& p1_, const Vec2& p2_, const Vec4& c_ = Vec4(1.0f))
			{
				DrawLine(p1_, p2_, c_, c_);
			}
		};

		class Light
		{
		};
		namespace Lights
		{
			class Direction:
				public Graphics::Lights::Direction,
				public Light
			{
			};
			class Point:
				public Graphics::Lights::Point,
				public Light
			{
			};
			class Spot:
				public Graphics::Lights::Spot,
				public Light
			{
			};
		}

		namespace Environments
		{
			class Cubemap:
				public Graphics::Environments::Cubemap
			{
			public:
				Texture*const texture;
			public:
				inline Cubemap(Texture* texture_):
					texture(texture_)
				{
				}
				inline ~Cubemap()
				{
					delete texture;
				}
			public:
				inline static Reference<Cubemap> LoadCube(Reference<Engine> engine_, const Filename& filename_)
				{
					auto cubeImages = OpenIL::Image::LoadCube(filename_);
					auto image = cubeImages[OpenIL::Image::Face::NegativeX];

					OpenGL::Texture::CubeMipmaps data;
					for(Size i = 0; i < image->GetMipmaps().size(); ++i)
					{
						data[0].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetRawData()});
						data[1].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetRawData()});
						data[2].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetRawData()});
						data[3].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetRawData()});
						data[4].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetRawData()});
						data[5].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetRawData()});
					}

					auto texture = new OpenGL::Texture(
						engine_->context,
						OpenGL::Texture::InternalFormat::RGBA8,
						OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
						OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
						OpenGL::Texture::Wrap::Repeat, OpenGL::Texture::Filter::Off, data);

					// OpenGL::Texture::CubeData data;
					// data[0] = (void*)cubeImages[OpenIL::Image::Face::NegativeX]->GetData().data();
					// data[1] = (void*)cubeImages[OpenIL::Image::Face::PositiveX]->GetData().data();
					// data[2] = (void*)cubeImages[OpenIL::Image::Face::NegativeY]->GetData().data();
					// data[3] = (void*)cubeImages[OpenIL::Image::Face::PositiveY]->GetData().data();
					// data[4] = (void*)cubeImages[OpenIL::Image::Face::NegativeZ]->GetData().data();
					// data[5] = (void*)cubeImages[OpenIL::Image::Face::PositiveZ]->GetData().data();
					// 
					// auto texture = new OpenGL::Texture(
					// 	engine_->context,
					// 	image->GetWidth(),
					// 	OpenGL::Texture::InternalFormat::RGBA8,
					// 	OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
					// 	OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
					// 	OpenGL::Texture::Wrap::Repeat, OpenGL::Texture::Filter::Off, data, true);

					return MakeReference(new Cubemap(texture));
				}
			};
			class Globalmap:
				public Graphics::Environments::Globalmap
			{
			public:
				Texture*const texture;
			public:
				inline Globalmap(Texture* texture_):
					texture(texture_)
				{
				}
				inline ~Globalmap()
				{
					delete texture;
				}
			public:
				inline static Reference<Globalmap> LoadCube(Reference<Engine> engine_, const Filename& filename_)
				{
					auto cubeImages = OpenIL::Image::LoadCube(filename_);
					auto image = cubeImages[OpenIL::Image::Face::NegativeX];

					OpenGL::Texture::CubeMipmaps data;
					for(Size i = 0; i < image->GetMipmaps().size(); ++i)
					{
						data[0].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetRawData()});
						data[1].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetRawData()});
						data[2].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetRawData()});
						data[3].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetRawData()});
						data[4].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetRawData()});
						data[5].push_back(OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetRawData()});
					}

					auto texture = new OpenGL::Texture(
						engine_->context,
						OpenGL::Texture::InternalFormat::RGBA8,
						OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
						OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
						OpenGL::Texture::Wrap::Repeat, OpenGL::Texture::Filter::Off, data);

					return MakeReference(new Globalmap(texture));
				}
			};
		}

		class Decal:
			public Graphics::Decal
		{
		public:
			inline Decal() = default;
			inline ~Decal() = default;
		};
	}
}


#pragma region Object
inline GreatVEngine::Reference<GreatVEngine::OpenGL::Object> GreatVEngine::OpenGL::Object::Load(Reference<System::BinaryFileReader> reader_, Vector<Reference<Geometry>>& meshes_, Reference<Material> material_)
{
	auto nameLength = (Size)reader_->Read<UInt32>();
	auto nameData = reader_->Read<char>(nameLength);
	Name name;
	{
		for(auto &glyph : nameData)
		{
			name.push_back(glyph);
		}
	}

	auto meshId = (Size)reader_->Read<UInt32>();

	auto matrixData = reader_->Read<Float32>(16);

	auto matrix = transpose(Mat4(
		matrixData[0],	matrixData[1],	matrixData[2],	matrixData[3],
		matrixData[4],	matrixData[5],	matrixData[6],	matrixData[7],
		matrixData[8],	matrixData[9],	matrixData[10],	matrixData[11],
		matrixData[12],	matrixData[13],	matrixData[14],	matrixData[15]));

	auto object = MakeReference(new Object());
	{
		object->SetName(name);

		auto position = VecXYZ(matrix * Vec4(Vec3(0.0f), 1.0f));
		auto angle = GreatVEngine::GetAngle(Move4(-position) * matrix);
		auto scale = VecXYZ((RotateYXZ4(-angle) * Move4(-position) * matrix) * Vec4(1.0f));

		object->SetLocalPosition(position);
		object->SetLocalAngle(angle);
		object->SetLocalScale(scale);

		auto shape = MakeReference(new Shape(material_->engine, meshes_[meshId]));
		auto model = MakeReference(new Model(material_->engine, shape, material_));

		object->SetModel(model);
	}

	auto subobjectsCount = (Size)reader_->Read<UInt32>();
	for(Size i = 0; i < subobjectsCount; ++i)
	{
		auto subobject = Load(reader_, meshes_, material_);
		subobject->SetParent(object);
	}

	return object;
}
inline GreatVEngine::Reference<GreatVEngine::OpenGL::Object> GreatVEngine::OpenGL::Object::Load(const Filename& filename_, Reference<Material> material_)
{
	auto reader = System::BinaryFileReader::LoadFile(filename_);
	auto root = MakeReference(new Object());


	auto meshesCount = (Size)reader->Read<UInt32>();
	Vector<Reference<Geometry>> meshes(meshesCount);
	{
		for(auto &mesh : meshes)
		{
			mesh = Geometry::LoadMesh(reader);
		}
	}
	
	auto objectsCount = (Size)reader->Read<UInt32>();
	for(Size i = 0; i < objectsCount; ++i)
	{
		auto object = Load(reader, meshes, material_);
		object->SetParent(root);
	}

	return root;
}
#pragma endregion
#pragma region Scene
inline void GreatVEngine::OpenGL::Scene::Add(Reference<OpenGL::Object> object_)
{
	struct T
	{
		Scene* scene;
		Reference<OpenGL::Object> object;
	};

	if(std::find(objects.begin(), objects.end(), object_) == objects.end())
	{
		object_->Subscribe_OnModelChange(new T{this, object_}, [](void* data, Reference<Graphics::Model> model_){
			auto t = (T*)data;
			auto $this = t->scene;

			$this->Remove(t->object);
			$this->Add(t->object);

			return true;
		});

		objects.push_back(object_);
	}

	for(auto &child : object_->childs)
	{
		Add(child);
	}
}
inline void GreatVEngine::OpenGL::Scene::Add(Reference<Lights::Direction> light_)
{
	lightsDirection.push_back(light_);
}
inline void GreatVEngine::OpenGL::Scene::Add(Reference<Lights::Point> light_)
{
	lightsPoint.push_back(light_);
}
inline void GreatVEngine::OpenGL::Scene::Add(Reference<Lights::Spot> light_)
{
	lightsSpot.push_back(light_);
}
inline void GreatVEngine::OpenGL::Scene::Add(Reference<Environments::Cubemap> environment_)
{
	environmentsCubemap.push_back(environment_);
}
inline void GreatVEngine::OpenGL::Scene::Add(Reference<Environments::Globalmap> environment_)
{
	environmentsGlobalmap.push_back(environment_);
}
inline void GreatVEngine::OpenGL::Scene::Add(Reference<Decal> decal_)
{
	struct T
	{
		Scene* scene;
		Reference<OpenGL::Decal> decal;
	};

	decal_->Subscribe_OnGroupsChange(new T{this, decal_}, [](void* data, UInt8 groups){
		auto t = (T*)data;
		auto $this = t->scene;
	
		$this->Remove(t->decal);
		$this->Add(t->decal);
	
		return true;
	});

	decals[decal_->GetGroups()].push_back(decal_);
}
inline void GreatVEngine::OpenGL::Scene::Remove(Reference<OpenGL::Object> object_)
{
	for(auto &child : object_->childs)
	{
		Remove(child);
	}

	objects.erase(std::find(objects.begin(), objects.end(), object_));
}
inline void GreatVEngine::OpenGL::Scene::Remove(Reference<Lights::Direction> light_)
{
	lightsDirection.erase(std::find(lightsDirection.begin(), lightsDirection.end(), light_));
}
inline void GreatVEngine::OpenGL::Scene::Remove(Reference<Lights::Point> light_)
{
	lightsPoint.erase(std::find(lightsPoint.begin(), lightsPoint.end(), light_));
}
inline void GreatVEngine::OpenGL::Scene::Remove(Reference<Lights::Spot> light_)
{
	lightsSpot.erase(std::find(lightsSpot.begin(), lightsSpot.end(), light_));
}
inline void GreatVEngine::OpenGL::Scene::Remove(Reference<Environments::Cubemap> environment_)
{
	environmentsCubemap.erase(std::find(environmentsCubemap.begin(), environmentsCubemap.end(), environment_));
}
inline void GreatVEngine::OpenGL::Scene::Remove(Reference<Environments::Globalmap> environment_)
{
	environmentsGlobalmap.erase(std::find(environmentsGlobalmap.begin(), environmentsGlobalmap.end(), environment_));
}
inline void GreatVEngine::OpenGL::Scene::Remove(Reference<OpenGL::Decal> decal_)
{
	auto &container = decals[decal_->GetGroups()];
	container.erase(std::find(container.begin(), container.end(), decal_));
}
inline void GreatVEngine::OpenGL::Scene::Render(Reference<Graphics::Camera> camera_, const Material::TechniqueType& techniqueType_)
{
	auto previousFrame = engine->context->GetBufferFrame();


	// render to geometry buffer
	geometyPass->frame->Set();
	{
		// sort by material/shape
		std::sort(objects.begin(), objects.end(), [](Reference<Object> a, Reference<Object> b){
			return *a.get() < *b.get();
		});

		glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glFrontFace(GL_CCW); DebugTest();
		glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS); DebugTest();

		// glDisable(GL_STENCIL_TEST); DebugTest();
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glDisable(GL_BLEND); DebugTest();

		glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
		glClearDepth(1.0f);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); DebugTest();

		glViewport(0, 0, viewport.x, viewport.y);

		for(auto &object : objects)
		{
			if(object->IsVisible())
			{
				glStencilFunc(GL_ALWAYS, object->GetGroups(), 0xFF);

				auto &model = object->model;

				if(model)
				{
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
			}
		}
	}

	// render decals
	decalPass->frame->Set();
	{
		if(decalPass->atlasAlbedo) decalPass->atlasAlbedo->Set(0);
		if(decalPass->atlasTopology) decalPass->atlasTopology->Set(1);
		if(decalPass->atlasMaterial) decalPass->atlasMaterial->Set(2);
		geometyPass->depthStencil->Set(3);

		glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glFrontFace(GL_CCW); DebugTest();
		glDisable(GL_DEPTH_TEST); DebugTest();

		// glDisable(GL_STENCIL_TEST); DebugTest();
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); DebugTest();

		decalPass->attributes->Set(); DebugTest();
		decalPass->vertices->Set(); DebugTest();
		decalPass->program->Set(); DebugTest();
		decalPass->program->SetVec2("screen", Vec2(viewport.x, viewport.y));
		decalPass->program->SetFloat("camFar", camera_->GetPerspective().zFar);
		decalPass->program->SetFloat("camFarMNear", camera_->GetPerspective().zFar - camera_->GetPerspective().zNear);
		decalPass->program->SetFloat("camFarXNear", camera_->GetPerspective().zFar * camera_->GetPerspective().zNear);
		decalPass->program->SetMat4("viewProjectionInverseMatrix", Move4(-camera_->GetPosition()) * camera_->GetViewProjectionInverseMatrix());

		for(auto &decalsGroupNode : decals)
		{
			auto &decalsGroup = decalsGroupNode.first;
			auto &decalsContainer = decalsGroupNode.second;

			glStencilFunc(GL_NOTEQUAL, 0, decalsGroup);

			// sort decals by priority
			std::sort(decalsContainer.begin(), decalsContainer.end(), [](Reference<Decal> a, Reference<Decal> b){ return *a.get() < *b.get(); }); // bigger priority goes first

			for(auto &decal : decalsContainer)
			{
				if(decal->IsVisible())
				{
					decalPass->program->SetVec3("decalNormal", decal->GetRMat() * Vec3(0.0f, 0.0f, -1.0f));
					decalPass->program->SetMat3("rotateMatrix", decal->GetRMat());
					decalPass->program->SetMat4("modelViewProjectionMatrix", camera_->GetVPMat() * decal->GetMMat());
					decalPass->program->SetMat4("modelInverseMatrix", Move4(Vec3(0.5f)) * Scale4(Vec3(0.5f)) * decal->GetMIMat() * Move4(camera_->GetPosition()));

					glDrawArrays(GL_POINTS, 0, 1);
				}
			}
		}
	}

	// render light (diffuse + specular)
	target->frameComplete->Set();
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		geometyPass->color->Set(0);
		geometyPass->specular->Set(1);
		geometyPass->normals->Set(2);
		geometyPass->material->Set(3);
		geometyPass->depthStencil->Set(4);

		lightPass->direction;
		{
			glDisable(GL_CULL_FACE); DebugTest();
			glDisable(GL_DEPTH_TEST); DebugTest();
			glDisable(GL_STENCIL_TEST); DebugTest();
			glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE); DebugTest();

			lightPass->direction->attributes->Set();
			lightPass->direction->vertices->Set();
			lightPass->direction->program->Set();
			lightPass->direction->program->SetFloat("camFar", camera_->GetPerspective().zFar);
			lightPass->direction->program->SetFloat("camFarMNear", camera_->GetPerspective().zFar - camera_->GetPerspective().zNear);
			lightPass->direction->program->SetFloat("camFarXNear", camera_->GetPerspective().zFar * camera_->GetPerspective().zNear);
			lightPass->direction->program->SetMat4("viewProjectionInverseMatrix", Move4(-camera_->GetPosition()) * camera_->GetViewProjectionInverseMatrix());
			
			for(auto &light : lightsDirection)
			{
				if(light->IsVisible())
				{
					lightPass->direction->program->SetVec3("lightDirection", light->GetRMat() * Vec3(0.0f, 0.0f, 1.0f));
					lightPass->direction->program->SetVec3("lightColor", VecXYZ(light->GetColor()) * light->GetColor().w);
					lightPass->direction->program->SetFloat("lightAmbient", light->GetAmbient());
					lightPass->direction->program->SetFloat("lightFog", light->GetFog());

					glDrawArrays(GL_POINTS, 0, 1);
				}
			}
		}

		lightPass->point;
		{
			glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); DebugTest();
			glDisable(GL_DEPTH_TEST); DebugTest();
			glDisable(GL_STENCIL_TEST); DebugTest();
			glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE); DebugTest();

			lightPass->point->attributes->Set();
			lightPass->point->vertices->Set();
			lightPass->point->program->Set();
			lightPass->point->program->SetVec2("screen", Vec2(viewport.x, viewport.y));
			lightPass->point->program->SetFloat("camFar", camera_->GetPerspective().zFar);
			lightPass->point->program->SetFloat("camFarMNear", camera_->GetPerspective().zFar - camera_->GetPerspective().zNear);
			lightPass->point->program->SetFloat("camFarXNear", camera_->GetPerspective().zFar * camera_->GetPerspective().zNear);
			lightPass->point->program->SetMat4("viewProjectionMatrix", camera_->GetVPMat() * Move4(camera_->GetPosition()));
			lightPass->point->program->SetMat4("viewProjectionInverseMatrix", Move4(-camera_->GetPosition()) * camera_->GetViewProjectionInverseMatrix());
			
			for(auto &light : lightsPoint)
			{
				if(light->IsVisible())
				{
					lightPass->point->program->SetVec3("lightPosition", light->GetPosition() - camera_->GetPosition());
					lightPass->point->program->SetVec3("lightColor", VecXYZ(light->GetColor()) * light->GetColor().w);
					// lightPass->point->program->SetVec3("lightRange", Vec3(light->GetRangeNear(), 1.0f / (light->GetRangeFar() - light->GetRangeNear()), 0.0f));
					lightPass->point->program->SetVec3("lightRange", Vec3(light->GetRangeNear(), light->GetRangeFar(), 0.0f));
					lightPass->point->program->SetFloat("lightAmbient", light->GetAmbient());
					lightPass->point->program->SetFloat("lightFog", light->GetFog());

					glDrawArrays(GL_POINTS, 0, 1);
				}
			}
		}
	}

	// render environment (specular only)
	target->frameSpecular->Set();
	{
		// Clear value of environment coverage
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		// Set textures
		geometyPass->color->Set(0);
		geometyPass->specular->Set(1);
		geometyPass->normals->Set(2);
		geometyPass->material->Set(3);
		geometyPass->depthStencil->Set(4);
		target->diffuse->Set(5);

		environmentPass->cubemap;
		{
			std::sort(environmentsCubemap.begin(), environmentsCubemap.end(), [](Reference<Environments::Cubemap> a, Reference<Environments::Cubemap> b){ return a->GetPriority() < b->GetPriority(); }); // bigger priority goes first

			environmentPass->cubemap->sampler->Set(8);

			glEnable(GL_CULL_FACE); glCullFace(GL_FRONT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			glEnable(GL_BLEND); glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_ONE, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

			environmentPass->cubemap->attributes->Set();
			environmentPass->cubemap->vertices->Set();
			environmentPass->cubemap->program->Set();
			environmentPass->cubemap->program->SetVec2("screen", Vec2(viewport.x, viewport.y));
			environmentPass->cubemap->program->SetFloat("camFar", camera_->GetPerspective().zFar);
			environmentPass->cubemap->program->SetFloat("camFarMNear", camera_->GetPerspective().zFar - camera_->GetPerspective().zNear);
			environmentPass->cubemap->program->SetFloat("camFarXNear", camera_->GetPerspective().zFar * camera_->GetPerspective().zNear);
			environmentPass->cubemap->program->SetMat4("viewProjectionMatrix", camera_->GetViewProjectionMatrix() * Move4(camera_->GetPosition()));
			environmentPass->cubemap->program->SetMat4("viewProjectionInverseMatrix", Move4(-camera_->GetPosition()) * camera_->GetViewProjectionInverseMatrix());
			
			for(auto &environment : environmentsCubemap)
			{
				if(environment->IsVisible())
				{
					environment->texture->Set(8);

					environmentPass->cubemap->program->SetVec3("environmentRangeNear", environment->GetRangeNear());
					environmentPass->cubemap->program->SetVec3("environmentRangeFar", environment->GetRangeFar());
					{
						auto t = (environment->GetRangeMiddle() - environment->GetRangeNear()) / (environment->GetRangeFar() - environment->GetRangeNear());
						auto a = Vec3(4.0f) * (Vec3(0.5f) - t);
						auto b = Vec3(1.0f) - a;

						environmentPass->cubemap->program->SetVec3("environmentRangeA", a);
						environmentPass->cubemap->program->SetVec3("environmentRangeB", b);
					}
					environmentPass->cubemap->program->SetMat4("environmentMatrix", camera_->GetVPMat() * environment->GetMMat() * Scale4(environment->GetRangeFar()));
					environmentPass->cubemap->program->SetMat4("environmentInverseMatrix", environment->GetMIMat() * Move4(camera_->GetPosition()));
					environmentPass->cubemap->program->SetVec3("environmentColor", VecXYZ(environment->GetColor()) * environment->GetColor().w);
					environmentPass->cubemap->program->SetFloat("environmentMipmapsCount", glm::log2((Float32)environment->texture->GetWidth()));

					glDrawArrays(GL_POINTS, 0, 1);
				}
			}

			environmentPass->cubemap->sampler->Reset(8);
		}

		environmentPass->globalmap;
		{
			environmentPass->globalmap->sampler->Set(8);

			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			glEnable(GL_BLEND); glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_ONE, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

			environmentPass->globalmap->attributes->Set();
			environmentPass->globalmap->vertices->Set();
			environmentPass->globalmap->program->Set();
			environmentPass->globalmap->program->SetVec2("screen", Vec2(viewport.x, viewport.y));
			environmentPass->globalmap->program->SetFloat("camFar", camera_->GetPerspective().zFar);
			environmentPass->globalmap->program->SetFloat("camFarMNear", camera_->GetPerspective().zFar - camera_->GetPerspective().zNear);
			environmentPass->globalmap->program->SetFloat("camFarXNear", camera_->GetPerspective().zFar * camera_->GetPerspective().zNear);
			environmentPass->globalmap->program->SetMat4("viewProjectionMatrix", camera_->GetViewProjectionMatrix() * Move4(camera_->GetPosition()));
			environmentPass->globalmap->program->SetMat4("viewProjectionInverseMatrix", Move4(-camera_->GetPosition()) * camera_->GetViewProjectionInverseMatrix());

			for(auto &environment : environmentsGlobalmap)
			{
				if(environment->IsVisible())
				{
					environment->texture->Set(8);

					environmentPass->globalmap->program->SetVec4("environmentColor", environment->GetColor());
					environmentPass->globalmap->program->SetFloat("environmentMipmapsCount", glm::log2((Float32)environment->texture->GetWidth()));

					glDrawArrays(GL_POINTS, 0, 1);
				}
			}

			environmentPass->globalmap->sampler->Reset(8);
		}
	}

	// enable buffer to draw
	if(previousFrame)
	{
		previousFrame->Set();
	}
	else
	{
		engine->context->SetBufferFrame(nullptr);
	}

	auto draw = [&](Texture* texture)
	{
		texture->Set(0);

		presenter->program->Set();
		presenter->attributes->Set();
		presenter->vertices->Set();
		
		glDrawArrays(GL_POINTS, 0, 1);
	};

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE);

	draw(target->diffuse);
	draw(target->specular);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawer;
	{
		drawer->attributes->Set();
		drawer->vertices->Set();
		drawer->program->Set();

		for(auto &line : drawer->lines)
		{
			drawer->program->SetVec2("p1", (line.p1 / Vec2(viewport.x, viewport.y)) *2.0f - 1.0f);
			drawer->program->SetVec2("p2", (line.p2 / Vec2(viewport.x, viewport.y)) *2.0f - 1.0f);
			drawer->program->SetVec4("c1", line.c1);
			drawer->program->SetVec4("c2", line.c2);

			glDrawArrays(GL_POINTS, 0, 1);
		}

		drawer->lines.clear();
	}
	// draw(target->specular);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);

	if(GetAsyncKeyState(VK_F1)) draw(geometyPass->color);
	if(GetAsyncKeyState(VK_F2)) draw(geometyPass->specular);
	if(GetAsyncKeyState(VK_F3)) draw(geometyPass->normals);
	if(GetAsyncKeyState(VK_F4)) draw(geometyPass->material);
	if(GetAsyncKeyState(VK_F5)) draw(geometyPass->depthStencil);
	if(GetAsyncKeyState(VK_F6)) draw(target->diffuse);
	if(GetAsyncKeyState(VK_F7)) draw(target->specular);

	glFlush();
}
#pragma endregion


#pragma region
#pragma endregion

