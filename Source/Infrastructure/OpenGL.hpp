#pragma region Include
#pragma once


#include "Graphics.hpp"


#include <APIs/OpenGL/Wrapper.hpp>
#include <APIs/OpenGL/WinAPI.hpp>
#include <Utilities/OpenIL/Functions.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace Graphics
	{
		namespace OpenGL
		{
			class Engine;
			class Technique;
			class Map;
			class Material;
			class Shape;
			class Model;
			class Bone;
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
				GreatVEngine::OpenGL::Context*const context;
			protected:
				inline GreatVEngine::OpenGL::Texture* LoadTexture(const Filename& filename_)
				{
					auto image = OpenIL::Image::Load2D(filename_);

					if(image->GetOriginMode() != OpenIL::Image::Origin::LowerLeft)
					{
						image->Flip();
					}
				
					auto texture = new GreatVEngine::OpenGL::Texture(
						context,
						image->GetWidth(), image->GetHeight(), image->GetDepth(),
						GreatVEngine::OpenGL::Texture::Type::D2, GreatVEngine::OpenGL::Texture::InternalFormat::RGBA8,
						GreatVEngine::OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
						GreatVEngine::OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
						GreatVEngine::OpenGL::Texture::Wrap::Repeat, GreatVEngine::OpenGL::Texture::Filter::Linear, image->GetRawData());

					return texture;
				}
			public:
				inline Engine(Reference<GreatVEngine::OpenGL::WinAPI::Context::DeviceContext> deviceContext_):
					context(new GreatVEngine::OpenGL::WinAPI::ExtendedContext(deviceContext_))
				{
					context->Set();
				}
				inline ~Engine()
				{
					delete context;
				}
			};

			class Loader
			{
			public:
				enum class NodeType: UInt8
				{
					Undefined = 0,
					Object = 1,
					Bone = 2,
				};
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
				GreatVEngine::OpenGL::Shader*const vertex;
				GreatVEngine::OpenGL::Shader*const tessellationControl;
				GreatVEngine::OpenGL::Shader*const tessellationEvaluation;
				GreatVEngine::OpenGL::Shader*const geometry;
				GreatVEngine::OpenGL::Shader*const fragment;
				GreatVEngine::OpenGL::Program*const program;
			public:
				inline Technique(
					Reference<Engine> engine_,
					const GreatVEngine::OpenGL::Shader::Source& sourceVertex_,
					const GreatVEngine::OpenGL::Shader::Source& sourceTessellationControl_,
					const GreatVEngine::OpenGL::Shader::Source& sourceTessellationEvaluation_,
					const GreatVEngine::OpenGL::Shader::Source& sourceGeometry_,
					const GreatVEngine::OpenGL::Shader::Source& sourceFragment_):
					engine(engine_),
					vertex(!sourceVertex_.empty() ? new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex, sourceVertex_) : nullptr),
					tessellationControl(!sourceTessellationControl_.empty() ? new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::TessellationControl, sourceTessellationControl_) : nullptr),
					tessellationEvaluation(!sourceTessellationEvaluation_.empty() ? new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::TessellationEvaluation, sourceTessellationEvaluation_) : nullptr),
					geometry(!sourceGeometry_.empty() ? new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry, sourceGeometry_) : nullptr),
					fragment(!sourceFragment_.empty() ? new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment, sourceFragment_) : nullptr),
					program(new GreatVEngine::OpenGL::Program(engine->context, {vertex, tessellationControl, tessellationEvaluation, geometry, fragment}))
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
					return WrapReference(new Technique(
						engine_,
						vertex_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + vertex_) : GreatVEngine::OpenGL::Shader::Source(),
						tessellationControl_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + tessellationControl_) : GreatVEngine::OpenGL::Shader::Source(),
						tessellationEvaluation_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + tessellationEvaluation_) : GreatVEngine::OpenGL::Shader::Source(),
						geometry_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + geometry_) : GreatVEngine::OpenGL::Shader::Source(),
						fragment_.size() > 0 ? System::LoadFileContent<UInt8>(base_ + fragment_) : GreatVEngine::OpenGL::Shader::Source()));
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
				GreatVEngine::OpenGL::Texture*const texture;
			protected:
				inline Map(GreatVEngine::OpenGL::Texture* texture_):
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
				
					return WrapReference(new Map(engine_->LoadTexture(filename_))); // texture));
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
				static const Geometry::VertexPackMode vertexPackMode = Geometry::VertexPackMode::Pos32F_TBN32F_Tex32F_Ind32U_Wei32F;
				static const Geometry::IndexPackMode indexPackMode = Geometry::IndexPackMode::UInt32;
			public:
				const Reference<Geometry> geometry;
			protected:
				const Reference<Engine> engine;
				GreatVEngine::OpenGL::Buffers::Array* vertices;
				GreatVEngine::OpenGL::Buffers::Index* indices;
				Size verticesCount;
				Size indicesCount;
			public:
				inline Shape(Reference<Engine> engine_, Reference<Geometry> geometry_):
					Graphics::Shape(geometry_->GetMin(), geometry_->GetMax()),
					engine(engine_),
					geometry(geometry_),
					vertices(new GreatVEngine::OpenGL::Buffers::Array(engine->context, *geometry_->GetVertices(vertexPackMode).get())),
					indices(new GreatVEngine::OpenGL::Buffers::Index(engine->context, *geometry_->GetIndices(indexPackMode).get())),
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
				const Array<GreatVEngine::OpenGL::Buffers::Attribute*, Material::TechniquesCount> attributes;
			public:
				inline Model(Reference<Engine> engine_, Reference<Shape> shape_, Reference<Material> material_):
					engine(engine_),
					shape(shape_),
					material(material_),
					attributes([&]()
					{
						Array<GreatVEngine::OpenGL::Buffers::Attribute*, Material::TechniquesCount> attributes;

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

								attributes[i] = new GreatVEngine::OpenGL::Buffers::Attribute(
									engine->context,
									program,
									shape->vertices,
									shape->indices, {
										GreatVEngine::OpenGL::Buffers::Attribute::Data(
											program->GetAttribute("vPos"),
											GreatVEngine::OpenGL::Buffers::Attribute::Data::Type::Float32, 3, false,
											Geometry::GetVertexSize(shape->vertexPackMode), 0),
										GreatVEngine::OpenGL::Buffers::Attribute::Data(
											program->GetAttribute("vTan"),
											GreatVEngine::OpenGL::Buffers::Attribute::Data::Type::Float32, 3, false,
											Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)*3),
										GreatVEngine::OpenGL::Buffers::Attribute::Data(
											program->GetAttribute("vBin"),
											GreatVEngine::OpenGL::Buffers::Attribute::Data::Type::Float32, 3, false,
											Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)* 6),
										GreatVEngine::OpenGL::Buffers::Attribute::Data(
											program->GetAttribute("vNor"),
											GreatVEngine::OpenGL::Buffers::Attribute::Data::Type::Float32, 3, false,
											Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)* 9),
										GreatVEngine::OpenGL::Buffers::Attribute::Data(
											program->GetAttribute("vTex"),
											GreatVEngine::OpenGL::Buffers::Attribute::Data::Type::Float32, 2, false,
											Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)* 12),
										GreatVEngine::OpenGL::Buffers::Attribute::Data(
											program->GetAttribute("vInd"),
											GreatVEngine::OpenGL::Buffers::Attribute::Data::Type::Float32, Geometry::BONE_WEIGHT_COUNT, false,
											Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)* 14),
										GreatVEngine::OpenGL::Buffers::Attribute::Data(
											program->GetAttribute("vWei"),
											GreatVEngine::OpenGL::Buffers::Attribute::Data::Type::Float32, Geometry::BONE_WEIGHT_COUNT, false,
											Geometry::GetVertexSize(shape->vertexPackMode), sizeof(Float32)* 18),
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

			class Hierarchycal
			{
			public:
				using Name = Helper::Logic::Named::Name;
				using HierarchyMatrix = Helper::Transformation::Dimension3::HierarchyMatrix;
			protected:
				Reference<Hierarchycal> parent;
				Vector<Reference<Object>> childObjects;
				Vector<Reference<Bone>> childBones;
			public:
				inline Hierarchycal() = default;
				inline Hierarchycal(const Hierarchycal&) = delete;
				inline ~Hierarchycal() = default;
			public:
				inline Reference<Object> operator [] (const Name& name_);
				inline Reference<Object> operator [] (const Size& i_);
				inline Reference<Bone> operator () (const Name& name_);
			public: // call only inside SetParent(...), public for access from Bone/Object
				inline void AddChild(Reference<Bone> bone_);
				inline void AddChild(Reference<Object> child_);
				inline void RemoveChild(Reference<Bone> bone_);
				inline void RemoveChild(Reference<Object> child_);
			public:
				inline Reference<Hierarchycal> GetParentHierarchycal() const;
			};

			class Bone:
				public Shared<Bone>,
				public Graphics::Bone,
				public Hierarchycal
			{
				friend Object;
				friend Scene;
			public:
				inline static Reference<Bone> Load(Reference<Engine> engine_, Reference<System::BinaryFileReader> reader_, Vector<Reference<Geometry>>& meshes_, Reference<Material> material_, Dictionary<Size, Reference<Bone>>& bonesTable_);
			public:
				inline Bone() = default;
				inline Bone(const Bone&) = delete;
				inline ~Bone() = default;
			public:
				inline void SetParent(HierarchyMatrix::Parent parent_);
				inline void SetParent(Reference<Bone> parent_);
				inline void SetParent(Reference<Object> parent_);
			public:
				inline void EnableBones();
				inline void DisableBones();
			public:
				inline void Visualize(Reference<Scene> scene_) const;
			};

			class Object:
				public Shared<Object>,
				public Graphics::Object,
				public Hierarchycal
			{
				friend Scene;
				friend Technique;
				friend Map;
				friend Material;
				friend Model;
				friend Shape;
			public:
				static const Size MAX_BONES_COUNT = 128;
			public:
				inline static void EnumerateBones(Reference<Object> object_, const Dictionary<Size, Reference<OpenGL::Bone>>& bonesTable_);
				inline static void EnumerateBones(Reference<OpenGL::Bone> bone_, const Dictionary<Size, Reference<OpenGL::Bone>>& bonesTable_);
				inline static Reference<Object> Load(Reference<Engine> engine_, Reference<System::BinaryFileReader> reader_, Vector<Reference<Geometry>>& meshes_, Reference<Material> material_, Dictionary<Size, Reference<OpenGL::Bone>>& bonesTable_);
				inline static Reference<Object> Load(Reference<Engine> engine_, const Filename& filename_, Reference<Material> material_);
			public:
				using OnModelChanged = Helper::Subscrption::OnEvent<Reference<Object>, Reference<Model>>;
			protected:
				OnModelChanged onModelChanged;
			public:
				inline void Subscribe_OnModelChanged(OnModelChanged::Subscriber subscriber_)
				{
					onModelChanged += subscriber_;
				}
				inline void Unsubscribe_OnModelChanged(OnModelChanged::Subscriber subscriber_)
				{
					onModelChanged -= subscriber_;
				}
			protected:
				Reference<Model> model = nullptr;
				Vector<Reference<OpenGL::Bone>> bones;
				GreatVEngine::OpenGL::Buffers::Uniform*const bufferBonesData;
			public:
				inline Object(Reference<Engine> engine_):
					bufferBonesData(new GreatVEngine::OpenGL::Buffers::Uniform(engine_->context, sizeof(Float32)* 16 * MAX_BONES_COUNT))
				{
				}
				inline ~Object()
				{
					delete bufferBonesData;
				}
			public:
				inline Reference<Model> GetModel() const
				{
					return model;
				}
				inline void SetModel(Reference<Model> model_)
				{
					model = model_;

					onModelChanged(shared_from_this(), model);
				}
				inline Vector<Reference<OpenGL::Bone>> GetBones()
				{
					return bones;
				}
			public:
				inline void SetParent(HierarchyMatrix::Parent parent_);
				inline void SetParent(Reference<OpenGL::Bone> parent_);
				inline void SetParent(Reference<Object> parent_);
				inline void AddBone(Reference<OpenGL::Bone> bone_)
				{
					bones.push_back(bone_);
				}
				inline void RemoveBone(Reference<OpenGL::Bone> bone_)
				{
					bones.erase(std::find(bones.begin(), bones.end(), bone_));
				}
			public:
				inline void EnableBones();
				inline void DisableBones();
			public:
				inline void Visualize(Reference<Scene> scene_) const;
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
				public Shared<Scene>,
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
					GreatVEngine::OpenGL::Texture*const diffuse;
					GreatVEngine::OpenGL::Texture*const specular;
					GreatVEngine::OpenGL::Buffers::Frame*const frameComplete;
					GreatVEngine::OpenGL::Buffers::Frame*const frameSpecular;
				public:
					inline Target(GreatVEngine::OpenGL::Context* context_, const Viewport& viewport_):
						diffuse(new GreatVEngine::OpenGL::Texture( // RGB - diffuse total color, A - diffuse intensity (for environment calculation)
							context_, viewport_.x, viewport_.y, 1, GreatVEngine::OpenGL::Texture::Type::D2,
							GreatVEngine::OpenGL::Texture::InternalFormat::RGBA16F, GreatVEngine::OpenGL::Texture::Format::RGBA, GreatVEngine::OpenGL::Texture::ComponentType::SFloat16,
							GreatVEngine::OpenGL::Texture::Wrap::Clamp, GreatVEngine::OpenGL::Texture::Filter::Off, nullptr)),
						specular(new GreatVEngine::OpenGL::Texture( // RGB - specular total color, A - environment intensity (for composition)
							context_, viewport_.x, viewport_.y, 1, GreatVEngine::OpenGL::Texture::Type::D2,
							GreatVEngine::OpenGL::Texture::InternalFormat::RGBA16F, GreatVEngine::OpenGL::Texture::Format::RGBA, GreatVEngine::OpenGL::Texture::ComponentType::SFloat16,
							GreatVEngine::OpenGL::Texture::Wrap::Clamp, GreatVEngine::OpenGL::Texture::Filter::Off, nullptr)),
						frameComplete(new GreatVEngine::OpenGL::Buffers::Frame(context_, {diffuse, specular}, nullptr, nullptr)),
						frameSpecular(new GreatVEngine::OpenGL::Buffers::Frame(context_, {specular}, nullptr, nullptr))
					{
					}
				};
				struct Presenter
				{
				public:
					GreatVEngine::OpenGL::Program*const program;
					GreatVEngine::OpenGL::Buffers::Array*const vertices;
					GreatVEngine::OpenGL::Buffers::Attribute*const attributes;
				public:
					inline Presenter(GreatVEngine::OpenGL::Context* context_):
						program([&](){
							auto program_ = new GreatVEngine::OpenGL::Program(context_, {
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Screen Quad/Drawer 2D/1.vs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Screen Quad/Drawer 2D/1.gs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Screen Quad/Drawer 2D/1.fs"))),
							});

							program_->Set();

								return program_;
						}()),
						vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
						attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {}))
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
					GreatVEngine::OpenGL::Texture*const color;	//	R8	G8	B8	XX	colorRGB
					GreatVEngine::OpenGL::Texture*const specular;	//	R8	G8	B8	XX	specularRGB
					GreatVEngine::OpenGL::Texture*const normals;	//	R16	G16	B16	XX	normalXYZ
					GreatVEngine::OpenGL::Texture*const material;	//	R8	XX	XX	XX	roughness, ???, ???
					GreatVEngine::OpenGL::Texture*const depthStencil;	//	D24	S8
					GreatVEngine::OpenGL::Buffers::Frame*const frame;
				public:
					inline GeometyPass(GreatVEngine::OpenGL::Context* context_, const Viewport& viewport_):
						color(new GreatVEngine::OpenGL::Texture(context_, viewport_.x, viewport_.y, 1,
							GreatVEngine::OpenGL::Texture::Type::D2, GreatVEngine::OpenGL::Texture::InternalFormat::RGBA8, GreatVEngine::OpenGL::Texture::Format::RGBA, GreatVEngine::OpenGL::Texture::ComponentType::UInt8,
							GreatVEngine::OpenGL::Texture::Wrap::Clamp, GreatVEngine::OpenGL::Texture::Filter::Off, nullptr)),
						specular(new GreatVEngine::OpenGL::Texture(context_, viewport_.x, viewport_.y, 1,
							GreatVEngine::OpenGL::Texture::Type::D2, GreatVEngine::OpenGL::Texture::InternalFormat::RGBA8, GreatVEngine::OpenGL::Texture::Format::RGBA, GreatVEngine::OpenGL::Texture::ComponentType::UInt8,
							GreatVEngine::OpenGL::Texture::Wrap::Clamp, GreatVEngine::OpenGL::Texture::Filter::Off, nullptr)),
						normals(new GreatVEngine::OpenGL::Texture(context_, viewport_.x, viewport_.y, 1,
							GreatVEngine::OpenGL::Texture::Type::D2, GreatVEngine::OpenGL::Texture::InternalFormat::RGBA16F, GreatVEngine::OpenGL::Texture::Format::RGBA, GreatVEngine::OpenGL::Texture::ComponentType::SFloat16,
							GreatVEngine::OpenGL::Texture::Wrap::Clamp, GreatVEngine::OpenGL::Texture::Filter::Off, nullptr)),
						material(new GreatVEngine::OpenGL::Texture(context_, viewport_.x, viewport_.y, 1,
							GreatVEngine::OpenGL::Texture::Type::D2, GreatVEngine::OpenGL::Texture::InternalFormat::RGBA8, GreatVEngine::OpenGL::Texture::Format::RGBA, GreatVEngine::OpenGL::Texture::ComponentType::UInt8,
							GreatVEngine::OpenGL::Texture::Wrap::Clamp, GreatVEngine::OpenGL::Texture::Filter::Off, nullptr)),
						depthStencil(new GreatVEngine::OpenGL::Texture(context_, viewport_.x, viewport_.y, 1,
							GreatVEngine::OpenGL::Texture::Type::D2, GreatVEngine::OpenGL::Texture::InternalFormat::Depth24Stencil8, GreatVEngine::OpenGL::Texture::Format::DepthStencil, GreatVEngine::OpenGL::Texture::ComponentType::UInt24_8,
							GreatVEngine::OpenGL::Texture::Wrap::Clamp, GreatVEngine::OpenGL::Texture::Filter::Off, nullptr)),
						frame(new GreatVEngine::OpenGL::Buffers::Frame(context_, {color, specular, normals, material}, depthStencil))
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
					GreatVEngine::OpenGL::Program*const program;
					GreatVEngine::OpenGL::Buffers::Array* vertices;
					GreatVEngine::OpenGL::Buffers::Attribute* attributes;
					GreatVEngine::OpenGL::Buffers::Frame*const frame;
					GreatVEngine::OpenGL::Texture* atlasAlbedo = nullptr;
					GreatVEngine::OpenGL::Texture* atlasTopology = nullptr;
					GreatVEngine::OpenGL::Texture* atlasMaterial = nullptr;
				public:
					inline DecalPass(GreatVEngine::OpenGL::Context* context_, GeometyPass* geometyPass_):
						program([&](){
							auto program_ = new GreatVEngine::OpenGL::Program(context_, {
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Decals/1.glsl.vs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Decals/1.glsl.gs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Decals/1.glsl.fs"))),
						});

						program_->Set();

						return program_;
					}()),
						vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
						attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {})),
						frame(new GreatVEngine::OpenGL::Buffers::Frame(context_, {
							geometyPass_->color,
							geometyPass_->specular,
							geometyPass_->normals,
							geometyPass_->material,
						}, geometyPass_->depthStencil))
					{
						frame->Reset();

						program->SetInt("textureAlbedo", 0);
						program->SetInt("textureTopology", 1);
						program->SetInt("textureMaterial", 2);
						program->SetInt("textureDepth", 3);
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
					struct AmbientOcclusion
					{
					public:
						const static Size COMPRESSED_DEPTH_SCALE_FACTOR = 1;
					public:
						GreatVEngine::OpenGL::Program*const program;
						GreatVEngine::OpenGL::Buffers::Array*const vertices;
						GreatVEngine::OpenGL::Buffers::Attribute*const attributes;
						GreatVEngine::OpenGL::Buffers::Frame*const frame;
						GreatVEngine::OpenGL::Texture*const compressedDepth;
						GreatVEngine::OpenGL::Buffers::Frame*const frameCompressedDepth;
						GreatVEngine::OpenGL::Program*const programCompressedDepth;
					public:
						inline AmbientOcclusion(GreatVEngine::OpenGL::Context* context_, GeometyPass* geometyPass_):
							program([&](){
								auto program_ = new GreatVEngine::OpenGL::Program(context_, {
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Ambient Occlusion/1.glsl.vs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Ambient Occlusion/1.glsl.gs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Ambient Occlusion/1.glsl.fs"))),
								});

								program_->Set();

								return program_;
							}()),
							vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
							attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {})),
							frame(new GreatVEngine::OpenGL::Buffers::Frame(context_, {
								geometyPass_->color,
								geometyPass_->specular,
							}, nullptr)),
							compressedDepth(new GreatVEngine::OpenGL::Texture(
								context_,
								(Size)(geometyPass_->depthStencil->GetWidth() / COMPRESSED_DEPTH_SCALE_FACTOR), (Size)(geometyPass_->depthStencil->GetHeight() / COMPRESSED_DEPTH_SCALE_FACTOR), 0,
								GreatVEngine::OpenGL::Texture::Type::D2,
								GreatVEngine::OpenGL::Texture::InternalFormat::R32F, GreatVEngine::OpenGL::Texture::Format::R, GreatVEngine::OpenGL::Texture::ComponentType::SFloat32,
								GreatVEngine::OpenGL::Texture::Wrap::Clamp, GreatVEngine::OpenGL::Texture::Filter::Linear,
								nullptr)),
							frameCompressedDepth(new GreatVEngine::OpenGL::Buffers::Frame(context_, {
								compressedDepth,
							}, nullptr)),
							programCompressedDepth([&](){
								auto program_ = new GreatVEngine::OpenGL::Program(context_, {
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Ambient Occlusion/Depth Blur/1.glsl.vs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Ambient Occlusion/Depth Blur/1.glsl.gs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Ambient Occlusion/Depth Blur/1.glsl.fs"))),
								});

								program_->Set();

								return program_;
							}())
						{
							frameCompressedDepth->Reset();

							program->Set();
							program->SetInt("textureDepth", 0);
							program->SetInt("textureNormal", 1);

							programCompressedDepth->Set();
							programCompressedDepth->SetInt("textureDepth", 0);
						}
					};
					struct Direction
					{
					public:
						GreatVEngine::OpenGL::Program*const program;
						GreatVEngine::OpenGL::Buffers::Array*const vertices;
						GreatVEngine::OpenGL::Buffers::Attribute*const attributes;
					public:
						inline Direction(GreatVEngine::OpenGL::Context* context_):
							program([&](){
								auto program_ = new GreatVEngine::OpenGL::Program(context_, {
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Direction Light/Basic/1.glsl.vs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Direction Light/Basic/1.glsl.gs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Direction Light/Basic/1.glsl.fs"))),
								});

								program_->Set();

								return program_;
							}()),
							vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
							attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {}))
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
						GreatVEngine::OpenGL::Program*const program;
						GreatVEngine::OpenGL::Buffers::Array* vertices;
						GreatVEngine::OpenGL::Buffers::Attribute* attributes;
					public:
						inline Point(GreatVEngine::OpenGL::Context* context_):
							program([&](){
								auto program_ = new GreatVEngine::OpenGL::Program(context_, {
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Point Light/Basic/1.glsl.vs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Point Light/Basic/1.glsl.gs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Point Light/Basic/1.glsl.fs"))),
							});

							program_->Set();

							return program_;
						}()),
							vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
							attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {}))
						{
							program->SetInt("textureColor", 0);
							program->SetInt("textureSpecular", 1);
							program->SetInt("textureNormal", 2);
							program->SetInt("textureMaterial", 3);
							program->SetInt("textureDepth", 4);
						}
					};
				public:
					AmbientOcclusion*const ambientOcclusion;
					Direction*const direction;
					Point*const point;
				public:
					inline LightPass(GreatVEngine::OpenGL::Context* context_, GeometyPass* geometyPass_):
						ambientOcclusion(new AmbientOcclusion(context_, geometyPass_)),
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
					struct LocalReflections
					{
					public:
						GreatVEngine::OpenGL::Program*const program;
						GreatVEngine::OpenGL::Buffers::Array* vertices;
						GreatVEngine::OpenGL::Buffers::Attribute* attributes;
					public:
						inline LocalReflections(GreatVEngine::OpenGL::Context* context_):
							program([&](){
								auto program_ = new GreatVEngine::OpenGL::Program(context_, {
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Local Reflections/1.glsl.vs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Local Reflections/1.glsl.gs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Local Reflections/1.glsl.fs"))),
							});

							program_->Set();

							return program_;
						}()),
							vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
							attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {}))
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
					struct Globalmap
					{
					public:
						GreatVEngine::OpenGL::Sampler* sampler;
						GreatVEngine::OpenGL::Program*const program;
						GreatVEngine::OpenGL::Buffers::Array* vertices;
						GreatVEngine::OpenGL::Buffers::Attribute* attributes;
					public:
						inline Globalmap(GreatVEngine::OpenGL::Context* context_):
							sampler(new GreatVEngine::OpenGL::Sampler(context_, GreatVEngine::OpenGL::Sampler::Wrap::Clamp, GreatVEngine::OpenGL::Sampler::Filter::Mipmap)),
							program([&](){
								auto program_ = new GreatVEngine::OpenGL::Program(context_, {
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Globalmap Environment/1.glsl.vs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Globalmap Environment/1.glsl.gs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Globalmap Environment/1.glsl.fs"))),
								});

								program_->Set();

								return program_;
							}()),
							vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
							attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {}))
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
						GreatVEngine::OpenGL::Sampler* sampler;
						GreatVEngine::OpenGL::Program*const program;
						GreatVEngine::OpenGL::Buffers::Array* vertices;
						GreatVEngine::OpenGL::Buffers::Attribute* attributes;
					public:
						inline Cubemap(GreatVEngine::OpenGL::Context* context_):
							sampler(new GreatVEngine::OpenGL::Sampler(context_, GreatVEngine::OpenGL::Sampler::Wrap::Clamp, GreatVEngine::OpenGL::Sampler::Filter::Mipmap)),
							program([&](){
								auto program_ = new GreatVEngine::OpenGL::Program(context_, {
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Cubemap Environment/1.glsl.vs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Cubemap Environment/1.glsl.gs"))),
									new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment, System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Deferred Shading/Cubemap Environment/1.glsl.fs"))),
								});

								program_->Set();

								return program_;
							}()),
							vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
							attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {}))
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
					LocalReflections*const localReflections;
					Cubemap*const cubemap;
					Globalmap*const globalmap;
					// bool isEnabled_ScreenSpaceLocalReflections = true;
				public:
					inline EnvironmentPass(GreatVEngine::OpenGL::Context* context_):
						localReflections(new LocalReflections(context_)),
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
				struct PostProcessing
				{
				public:
					struct Bloom
					{
					public:
						struct Holder
						{
						public:
							GreatVEngine::OpenGL::Texture*const color;
							GreatVEngine::OpenGL::Buffers::Frame*const frame;
						public:
							inline Holder(GreatVEngine::OpenGL::Context* context_, const Viewport& viewport_):
								color(new GreatVEngine::OpenGL::Texture(context_, viewport_.x, viewport_.y, 1,
									GreatVEngine::OpenGL::Texture::Type::D2, GreatVEngine::OpenGL::Texture::InternalFormat::RGB16F, GreatVEngine::OpenGL::Texture::Format::RGB, GreatVEngine::OpenGL::Texture::ComponentType::SFloat16,
									GreatVEngine::OpenGL::Texture::Wrap::Clamp, GreatVEngine::OpenGL::Texture::Filter::Off, nullptr)),
								frame(new GreatVEngine::OpenGL::Buffers::Frame(context_, {color}, nullptr))
							{
								frame->Reset();
							}
							inline ~Holder()
							{
								delete frame;
								delete color;
							}
						};
						struct IntensityFilter
						{
						public:
							GreatVEngine::OpenGL::Program*const program;
							GreatVEngine::OpenGL::Buffers::Array*const vertices;
							GreatVEngine::OpenGL::Buffers::Attribute*const attributes;
						public:
							inline IntensityFilter(GreatVEngine::OpenGL::Context* context_):
								program([&](){
									auto program_ = new GreatVEngine::OpenGL::Program(context_, {
										new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Post Processing/Bloom/1.glsl.vs"))),
										new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Post Processing/Bloom/1.glsl.gs"))),
										new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Post Processing/Bloom/1.glsl.fs"))),
									});

									program_->Set();

										return program_;
								}()),
								vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
								attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {}))
							{
								program->SetInt("textureDiffuse", 0);
								program->SetInt("textureSpecular", 1);
							}
						};
						struct Blur
						{
						public:
							GreatVEngine::OpenGL::Program*const program;
							GreatVEngine::OpenGL::Buffers::Array*const vertices;
							GreatVEngine::OpenGL::Buffers::Attribute*const attributes;
						public:
							inline Blur(GreatVEngine::OpenGL::Context* context_):
								program([&](){
									auto program_ = new GreatVEngine::OpenGL::Program(context_, {
										new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Post Processing/Bloom/Blur/1.glsl.vs"))),
										new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Post Processing/Bloom/Blur/1.glsl.gs"))),
										new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Post Processing/Bloom/Blur/1.glsl.fs"))),
									});

									program_->Set();

										return program_;
								}()),
								vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
								attributes(new GreatVEngine::OpenGL::Buffers::Attribute(context_, program, vertices, nullptr, {}))
							{
								program->SetInt("textureColor", 0);
							}

						};
					public:
						Holder*const holder;
						IntensityFilter*const intensityFilter;
						Blur*const blur;
					public:
						inline Bloom(GreatVEngine::OpenGL::Context* context_, const Viewport& viewport_):
							holder(new Holder(context_, viewport_)),
							intensityFilter(new IntensityFilter(context_)),
							blur(new Blur(context_))
						{
						}
					};
				public:
					Bloom*const bloom;
				public:
					inline PostProcessing(GreatVEngine::OpenGL::Context* context_, const Viewport& viewport_):
						bloom(new Bloom(context_, viewport_))
					{
					}
				};
				struct Drawer
				{
				public:
					struct Line2D
					{
					public:
						Vec2 p1, p2;
						Vec4 c1, c2;
					};
					struct Line3D
					{
					public:
						Vec3 p1, p2;
						Vec4 c1, c2;
					};
					struct Circle2D
					{
					public:
						Vec2 p;
						Float32 s;
						Vec4 c;
					};
					struct Circle3D
					{
					public:
						Vec3 p;
						Float32 s;
						Vec4 c;
					};
				public:
					GreatVEngine::OpenGL::Buffers::Array* vertices;
					GreatVEngine::OpenGL::Program*const programLines2D;
					GreatVEngine::OpenGL::Buffers::Attribute* attributesLines2D;
					GreatVEngine::OpenGL::Program*const programLines3D;
					GreatVEngine::OpenGL::Buffers::Attribute* attributesLines3D;
					GreatVEngine::OpenGL::Program*const programCircles2D;
					GreatVEngine::OpenGL::Buffers::Attribute* attributesCircles2D;
					GreatVEngine::OpenGL::Program*const programCircles3D;
					GreatVEngine::OpenGL::Buffers::Attribute* attributesCircles3D;
					Vector<Line2D> lines2D;
					Vector<Line3D> lines3D;
					Vector<Circle2D> circles2D;
					Vector<Circle3D> circles3D;
				public:
					inline Drawer(GreatVEngine::OpenGL::Context* context_):
						vertices(new GreatVEngine::OpenGL::Buffers::Array(context_, 1, nullptr)),
						programLines2D([&](){
							auto program_ = new GreatVEngine::OpenGL::Program(context_, {
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/2D/Line.glsl.vs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/2D/Line.glsl.gs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/2D/Line.glsl.fs"))),
							});

							program_->Set();

							return program_;
						}()),
						attributesLines2D(new GreatVEngine::OpenGL::Buffers::Attribute(context_, programLines2D, vertices, nullptr, {})),
						programLines3D([&](){
							auto program_ = new GreatVEngine::OpenGL::Program(context_, {
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/3D/Line.glsl.vs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/3D/Line.glsl.gs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/3D/Line.glsl.fs"))),
							});

							program_->Set();

							return program_;
						}()),
						attributesLines3D(new GreatVEngine::OpenGL::Buffers::Attribute(context_, programLines3D, vertices, nullptr, {})),
						programCircles2D([&](){
							auto program_ = new GreatVEngine::OpenGL::Program(context_, {
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/2D/Circle.glsl.vs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/2D/Circle.glsl.gs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/2D/Circle.glsl.fs"))),
							});

							program_->Set();

							return program_;
						}()),
						attributesCircles2D(new GreatVEngine::OpenGL::Buffers::Attribute(context_, programCircles2D, vertices, nullptr, {})),
						programCircles3D([&](){
							auto program_ = new GreatVEngine::OpenGL::Program(context_, {
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Vertex,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/3D/Circle.glsl.vs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Geometry,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/3D/Circle.glsl.gs"))),
								new GreatVEngine::OpenGL::Shader(GreatVEngine::OpenGL::Shader::Type::Fragment,	System::LoadFileContent<UInt8>(Filepath("Media/Shaders/Drawer/3D/Circle.glsl.fs"))),
							});

							program_->Set();

							return program_;
						}()),
						attributesCircles3D(new GreatVEngine::OpenGL::Buffers::Attribute(context_, programCircles3D, vertices, nullptr, {}))
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
				PostProcessing* postProcessing;
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
					lightPass(new LightPass(engine->context, geometyPass)),
					environmentPass(new EnvironmentPass(engine->context)),
					postProcessing(new PostProcessing(engine->context, viewport_)),
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
			protected:
				inline void Add(Reference<Bone> bone_); // recursively add child objects of bone
				inline void Remove(Reference<Bone> bone_); // recursively remove child objects of bone
				inline void OnModelChanged(Reference<Object> object_, Reference<Model> model_);
			public:
				inline Reference<Engine> GetEngine() const
				{
					return engine;
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
					drawer->lines2D.push_back({p1_, p2_, c1_, c2_});
				}
				inline void DrawLine(const Vec2& p1_, const Vec2& p2_, const Vec4& c_ = Vec4(1.0f))
				{
					DrawLine(p1_, p2_, c_, c_);
				}
				inline void DrawLine(const Vec3& p1_, const Vec3& p2_, const Vec4& c1_, const Vec4& c2_)
				{
					drawer->lines3D.push_back({p1_, p2_, c1_, c2_});
				}
				inline void DrawLine(const Vec3& p1_, const Vec3& p2_, const Vec4& c_ = Vec4(1.0f))
				{
					DrawLine(p1_, p2_, c_, c_);
				}
				inline void DrawCircle(const Vec2& p_, const Float32& s_, const Vec4& c_ = Vec4(1.0f))
				{
					drawer->circles2D.push_back({p_, s_, c_});
				}
				inline void DrawCircle(const Vec3& p_, const Float32& s_, const Vec4& c_ = Vec4(1.0f))
				{
					drawer->circles3D.push_back({p_, s_, c_});
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
					GreatVEngine::OpenGL::Texture*const texture;
				public:
					inline Cubemap(GreatVEngine::OpenGL::Texture* texture_):
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

						GreatVEngine::OpenGL::Texture::CubeMipmaps data;
						for(Size i = 0; i < image->GetMipmaps().size(); ++i)
						{
							data[0].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetRawData()});
							data[1].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetRawData()});
							data[2].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetRawData()});
							data[3].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetRawData()});
							data[4].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetRawData()});
							data[5].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetRawData()});
						}

						auto texture = new GreatVEngine::OpenGL::Texture(
							engine_->context,
							GreatVEngine::OpenGL::Texture::InternalFormat::RGBA8,
							GreatVEngine::OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
							GreatVEngine::OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
							GreatVEngine::OpenGL::Texture::Wrap::Repeat, GreatVEngine::OpenGL::Texture::Filter::Off, data);

						return WrapReference(new Cubemap(texture));
					}
				};
				class Globalmap:
					public Graphics::Environments::Globalmap
				{
				public:
					GreatVEngine::OpenGL::Texture*const texture;
				public:
					inline Globalmap(GreatVEngine::OpenGL::Texture* texture_):
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

						GreatVEngine::OpenGL::Texture::CubeMipmaps data;
						for(Size i = 0; i < image->GetMipmaps().size(); ++i)
						{
							data[0].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeX])[i].GetRawData()});
							data[1].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveX])[i].GetRawData()});
							data[2].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeY])[i].GetRawData()});
							data[3].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveY])[i].GetRawData()});
							data[4].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::NegativeZ])[i].GetRawData()});
							data[5].push_back(GreatVEngine::OpenGL::Texture::Mipmap{(*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetWidth(), (*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetWidth(), 0, (*cubeImages[OpenIL::Image::Face::PositiveZ])[i].GetRawData()});
						}

						auto texture = new GreatVEngine::OpenGL::Texture(
							engine_->context,
							GreatVEngine::OpenGL::Texture::InternalFormat::RGBA8,
							GreatVEngine::OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
							GreatVEngine::OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
							GreatVEngine::OpenGL::Texture::Wrap::Repeat, GreatVEngine::OpenGL::Texture::Filter::Off, data);

						return WrapReference(new Globalmap(texture));
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
}


#pragma region Hierarchycal
inline GreatVEngine::Reference<GreatVEngine::Graphics::OpenGL::Object> GreatVEngine::Graphics::OpenGL::Hierarchycal::operator [] (const Name& name_)
{
	for(auto &child : childObjects)
	{
		if(child->GetName() == name_)
		{
			return child;
		}
	}

	return nullptr;
}
inline GreatVEngine::Reference<GreatVEngine::Graphics::OpenGL::Object> GreatVEngine::Graphics::OpenGL::Hierarchycal::operator [] (const Size& i_)
{
	return childObjects[i_];
}
inline GreatVEngine::Reference<GreatVEngine::Graphics::OpenGL::Bone> GreatVEngine::Graphics::OpenGL::Hierarchycal::operator () (const Name& name_)
{
	for(auto &child : childBones)
	{
		if(child->GetName() == name_)
		{
			return child;
		}
	}

	return nullptr;
}
inline GreatVEngine::Reference<GreatVEngine::Graphics::OpenGL::Hierarchycal> GreatVEngine::Graphics::OpenGL::Hierarchycal::GetParentHierarchycal() const
{
	return parent;
}
inline void GreatVEngine::Graphics::OpenGL::Hierarchycal::AddChild(Reference<Bone> child_)
{
	childBones.push_back(child_);
}
inline void GreatVEngine::Graphics::OpenGL::Hierarchycal::AddChild(Reference<Object> child_)
{
	childObjects.push_back(child_);
}
inline void GreatVEngine::Graphics::OpenGL::Hierarchycal::RemoveChild(Reference<Bone> child_)
{
	childBones.erase(std::find(childBones.begin(), childBones.end(), child_));
}
inline void GreatVEngine::Graphics::OpenGL::Hierarchycal::RemoveChild(Reference<Object> child_)
{
	childObjects.erase(std::find(childObjects.begin(), childObjects.end(), child_));
}
#pragma endregion
#pragma region Bone
inline GreatVEngine::Reference<GreatVEngine::Graphics::OpenGL::Bone> GreatVEngine::Graphics::OpenGL::Bone::Load(Reference<Engine> engine_, Reference<System::BinaryFileReader> reader_, Vector<Reference<Geometry>>& meshes_, Reference<Material> material_, Dictionary<Size, Reference<Bone>>& bonesTable_)
{
	auto nameLength = (Size)reader_->Read<UInt32>();
	auto nameData = nameLength > 0 ? reader_->Read<char>(nameLength) : Vector<char>();
	Named::Name name;
	{
		for(auto &glyph : nameData)
		{
			name.push_back(glyph);
		}
	}

	auto matrixData = reader_->Read<Float32>(16);

	auto matrix = transpose(Mat4(
		matrixData[0], matrixData[1], matrixData[2], matrixData[3],
		matrixData[4], matrixData[5], matrixData[6], matrixData[7],
		matrixData[8], matrixData[9], matrixData[10], matrixData[11],
		matrixData[12], matrixData[13], matrixData[14], matrixData[15]));

	auto index = (Size)reader_->Read<UInt32>();

	auto root = WrapReference(new Bone());
	{
		root->SetName(name);

		auto position = VecXYZ(matrix * Vec4(Vec3(0.0f), 1.0f));
		auto angle = GreatVEngine::GetAngle(Move4(-position) * matrix);
		auto scale = Vec3(1.0f); // TODO

		root->origin.SetPosition(position);
		root->origin.SetAngle(angle);
		root->origin.SetScale(scale);

		root->SetLocalPosition(position);
		root->SetLocalAngle(angle);
		root->SetLocalScale(scale);
	}

	bonesTable_[index] = root;

	auto subobjectsCount = (Size)reader_->Read<UInt32>();
	for(Size i = 0; i < subobjectsCount; ++i)
	{
		auto nodeType = (Loader::NodeType)reader_->Read<UInt8>();

		switch(nodeType)
		{
			case Loader::NodeType::Object:
			{
				auto object = OpenGL::Object::Load(engine_, reader_, meshes_, material_, bonesTable_);
				object->SetParent(root);
			} break;
			case Loader::NodeType::Bone:
			{
				auto bone = Load(engine_, reader_, meshes_, material_, bonesTable_);
				bone->SetParent(root);
			} break;
			default:
				throw Exception("Unknown node type.");
		}
	}

	return root;
}

inline void GreatVEngine::Graphics::OpenGL::Bone::SetParent(HierarchyMatrix::Parent parent_)
{
	if(Hierarchycal::parent)
	{
		Hierarchycal::parent->RemoveChild(shared_from_this());
	}

	HierarchyMatrix::SetParent(parent_);
}
inline void GreatVEngine::Graphics::OpenGL::Bone::SetParent(Reference<Bone> parent_)
{
	SetParent((HierarchyMatrix::Parent)parent_.get());

	Hierarchycal::parent = (Reference<Hierarchycal>)parent_;

	if(Hierarchycal::parent)
	{
		Hierarchycal::parent->AddChild(shared_from_this());
	}
}
inline void GreatVEngine::Graphics::OpenGL::Bone::SetParent(Reference<Object> parent_)
{
	SetParent((HierarchyMatrix::Parent)parent_.get());

	Hierarchycal::parent = (Reference<Hierarchycal>)parent_;

	if(Hierarchycal::parent)
	{
		Hierarchycal::parent->AddChild(shared_from_this());
	}
}

inline void GreatVEngine::Graphics::OpenGL::Bone::EnableBones()
{
	for(auto &child : childObjects)
	{
		child->EnableBones();
	}

	for(auto &child : childBones)
	{
		child->EnableBones();
	}
}
inline void GreatVEngine::Graphics::OpenGL::Bone::DisableBones()
{
	for(auto &child : childObjects)
	{
		child->DisableBones();
	}

	for(auto &child : childBones)
	{
		child->DisableBones();
	}
}

inline void GreatVEngine::Graphics::OpenGL::Bone::Visualize(Reference<Scene> scene_) const
{
	scene_->DrawLine(
		VecXYZ(GetMMat() * Vec4(0.0f, 0.0f, 0.0f, 1.0f)),
		VecXYZ(GetMMat() * Vec4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	scene_->DrawLine(
		VecXYZ(GetMMat() * Vec4(0.0f, 0.0f, 0.0f, 1.0f)),
		VecXYZ(GetMMat() * Vec4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	scene_->DrawLine(
		VecXYZ(GetMMat() * Vec4(0.0f, 0.0f, 0.0f, 1.0f)),
		VecXYZ(GetMMat() * Vec4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vec4(0.0f, 0.0f, 1.0f, 1.0f));

	for(auto &child : childObjects)
	{
		child->Visualize(scene_);
	}
	for(auto &child : childBones)
	{
		child->Visualize(scene_);
	}
}
#pragma endregion
#pragma region Object
inline void GreatVEngine::Graphics::OpenGL::Object::EnumerateBones(Reference<Object> object_, const Dictionary<Size, Reference<OpenGL::Bone>>& bonesTable_)
{
	for(auto &bone : bonesTable_)
	{
		object_->AddBone(bone.second);
	}

	for(auto &child : object_->childObjects)
	{
		EnumerateBones(child, bonesTable_);
	}
	
	for(auto &child : object_->childBones)
	{
		EnumerateBones(child, bonesTable_);
	}
}
inline void GreatVEngine::Graphics::OpenGL::Object::EnumerateBones(Reference<OpenGL::Bone> bone_, const Dictionary<Size, Reference<OpenGL::Bone>>& bonesTable_)
{
	for(auto &child : bone_->childObjects)
	{
		EnumerateBones(child, bonesTable_);
	}
	
	for(auto &child : bone_->childBones)
	{
		EnumerateBones(child, bonesTable_);
	}
}

inline GreatVEngine::Reference<GreatVEngine::Graphics::OpenGL::Object> GreatVEngine::Graphics::OpenGL::Object::Load(Reference<Engine> engine_, Reference<System::BinaryFileReader> reader_, Vector<Reference<Geometry>>& meshes_, Reference<Material> material_, Dictionary<Size, Reference<OpenGL::Bone>>& bonesTable_)
{
	auto nameLength = (Size)reader_->Read<UInt32>();
	auto nameData = nameLength > 0 ? reader_->Read<char>(nameLength) : Vector<char>();
	Named::Name name;
	{
		for(auto &glyph : nameData)
		{
			name.push_back(glyph);
		}
	}

	auto matrixData = reader_->Read<Float32>(16);

	auto matrix = transpose(Mat4(
		matrixData[0], matrixData[1], matrixData[2], matrixData[3],
		matrixData[4], matrixData[5], matrixData[6], matrixData[7],
		matrixData[8], matrixData[9], matrixData[10], matrixData[11],
		matrixData[12], matrixData[13], matrixData[14], matrixData[15]));

	auto meshId = (Size)reader_->Read<UInt32>();

	auto object = WrapReference(new Object(engine_));
	{
		object->SetName(name);

		auto position = VecXYZ(matrix * Vec4(Vec3(0.0f), 1.0f));
		auto angle = GreatVEngine::GetAngle(Move4(-position) * matrix);
		auto scale = Vec3(1.0f); // TODO

		object->SetLocalPosition(position);
		object->SetLocalAngle(angle);
		object->SetLocalScale(scale);

		if(meshId != UINT32_MAX)
		{
			auto shape = WrapReference(new Shape(material_->engine, meshes_[meshId]));
			auto model = WrapReference(new Model(material_->engine, shape, material_));

			object->SetModel(model);
		}
	}

	auto subobjectsCount = (Size)reader_->Read<UInt32>();
	for(Size i = 0; i < subobjectsCount; ++i)
	{
		auto subobject = Load(engine_, reader_, meshes_, material_, bonesTable_);
		subobject->SetParent(object);
	}

	return object;
}
inline GreatVEngine::Reference<GreatVEngine::Graphics::OpenGL::Object> GreatVEngine::Graphics::OpenGL::Object::Load(Reference<Engine> engine_, const Filename& filename_, Reference<Material> material_)
{
	auto reader = System::BinaryFileReader::LoadFile(filename_);
	auto root = WrapReference(new Object(engine_));


	auto meshesCount = (Size)reader->Read<UInt32>();
	Vector<Reference<Geometry>> meshes(meshesCount);
	{
		for(auto &mesh : meshes)
		{
			mesh = Geometry::LoadMesh(reader);
		}
	}
	
	Dictionary<Size, Reference<OpenGL::Bone>> bonesTable;

	auto nodesCount = (Size)reader->Read<UInt32>();
	for(Size i = 0; i < nodesCount; ++i)
	{
		auto nodeType = (Loader::NodeType)reader->Read<UInt8>();

		switch(nodeType)
		{
			case Loader::NodeType::Object:
			{
				auto object = Load(engine_, reader, meshes, material_, bonesTable);
				object->SetParent(root);
			} break;
			case Loader::NodeType::Bone:
			{
				auto bone = OpenGL::Bone::Load(engine_, reader, meshes, material_, bonesTable);
				bone->SetParent(root);
			} break;
			default:
				throw Exception("Unknown node type.");
		}
	}

	EnumerateBones(root, bonesTable);

	return root;
}

inline void GreatVEngine::Graphics::OpenGL::Object::SetParent(HierarchyMatrix::Parent parent_)
{
	if(Hierarchycal::parent)
	{
		Hierarchycal::parent->RemoveChild(shared_from_this());
	}

	HierarchyMatrix::SetParent(parent_);
}
inline void GreatVEngine::Graphics::OpenGL::Object::SetParent(Reference<OpenGL::Bone> parent_)
{
	SetParent((HierarchyMatrix::Parent)parent_.get());

	Hierarchycal::parent = (Reference<Hierarchycal>)parent_;

	if(Hierarchycal::parent)
	{
		Hierarchycal::parent->AddChild(shared_from_this());
	}
}
inline void GreatVEngine::Graphics::OpenGL::Object::SetParent(Reference<Object> parent_)
{
	SetParent((HierarchyMatrix::Parent)parent_.get());

	Hierarchycal::parent = (Reference<Hierarchycal>)parent_;

	if(Hierarchycal::parent)
	{
		Hierarchycal::parent->AddChild(shared_from_this());
	}
}

inline void GreatVEngine::Graphics::OpenGL::Object::EnableBones()
{
	EnableLocalBones();

	for(auto &child : childObjects)
	{
		child->EnableBones();
	}

	for(auto &child : childBones)
	{
		child->EnableBones();
	}
}
inline void GreatVEngine::Graphics::OpenGL::Object::DisableBones()
{
	DisableLocalBones();

	for(auto &child : childObjects)
	{
		child->DisableBones();
	}

	for(auto &child : childBones)
	{
		child->DisableBones();
	}
}

inline void GreatVEngine::Graphics::OpenGL::Object::Visualize(Reference<Scene> scene_) const
{
	scene_->DrawLine(
		VecXYZ(GetMMat() * Vec4(0.0f, 0.0f, 0.0f, 1.0f)),
		VecXYZ(GetMMat() * Vec4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	scene_->DrawLine(
		VecXYZ(GetMMat() * Vec4(0.0f, 0.0f, 0.0f, 1.0f)),
		VecXYZ(GetMMat() * Vec4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	scene_->DrawLine(
		VecXYZ(GetMMat() * Vec4(0.0f, 0.0f, 0.0f, 1.0f)),
		VecXYZ(GetMMat() * Vec4(0.0f, 0.0f, 1.0f, 1.0f)),
		Vec4(0.0f, 0.0f, 1.0f, 1.0f));

	if(model)
	{
		auto mi = model->GetShape()->GetMin();
		auto ma = model->GetShape()->GetMax();

		auto vNNN = VecXYZ(GetMMat() * vec4(Vec3(mi.x, mi.y, mi.z), 1.0f));
		auto vPNN = VecXYZ(GetMMat() * vec4(Vec3(ma.x, mi.y, mi.z), 1.0f));
		auto vNPN = VecXYZ(GetMMat() * vec4(Vec3(mi.x, ma.y, mi.z), 1.0f));
		auto vPPN = VecXYZ(GetMMat() * vec4(Vec3(ma.x, ma.y, mi.z), 1.0f));
		auto vNNP = VecXYZ(GetMMat() * vec4(Vec3(mi.x, mi.y, ma.z), 1.0f));
		auto vPNP = VecXYZ(GetMMat() * vec4(Vec3(ma.x, mi.y, ma.z), 1.0f));
		auto vNPP = VecXYZ(GetMMat() * vec4(Vec3(mi.x, ma.y, ma.z), 1.0f));
		auto vPPP = VecXYZ(GetMMat() * vec4(Vec3(ma.x, ma.y, ma.z), 1.0f));

		scene_->DrawLine(vNNN, vPNN, GetColor());
		scene_->DrawLine(vNPN, vPPN, GetColor());
		scene_->DrawLine(vNNP, vPNP, GetColor());
		scene_->DrawLine(vNPP, vPPP, GetColor());

		scene_->DrawLine(vNNN, vNPN, GetColor());
		scene_->DrawLine(vPNN, vPPN, GetColor());
		scene_->DrawLine(vNNP, vNPP, GetColor());
		scene_->DrawLine(vPNP, vPPP, GetColor());

		scene_->DrawLine(vNNN, vNNP, GetColor());
		scene_->DrawLine(vPNN, vPNP, GetColor());
		scene_->DrawLine(vNPN, vNPP, GetColor());
		scene_->DrawLine(vPPN, vPPP, GetColor());
	}

	for(auto &child : childObjects)
	{
		child->Visualize(scene_);
	}
	for(auto &child : childBones)
	{
		child->Visualize(scene_);
	}
}
#pragma endregion
#pragma region Scene
inline void GreatVEngine::Graphics::OpenGL::Scene::OnModelChanged(Reference<Object> object_, Reference<Model> model_)
{
}


inline void GreatVEngine::Graphics::OpenGL::Scene::Add(Reference<OpenGL::Bone> bone_)
{
	for(auto &child : bone_->childObjects)
	{
		Add(child);
	}
	for(auto &child : bone_->childBones)
	{
		Add(child);
	}
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Add(Reference<OpenGL::Object> object_)
{
	if(std::find(objects.begin(), objects.end(), object_) == objects.end())
	{
		object_->Subscribe_OnModelChanged(Object::OnModelChanged::Subscriber(*this, &Scene::OnModelChanged));
		objects.push_back(object_);
	}
	else
	{
		throw Exception("Failed to add object twice");
	}

	for(auto &child : object_->childObjects)
	{
		Add(child);
	}
	for(auto &child : object_->childBones)
	{
		Add(child);
	}
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Add(Reference<Lights::Direction> light_)
{
	lightsDirection.push_back(light_);
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Add(Reference<Lights::Point> light_)
{
	lightsPoint.push_back(light_);
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Add(Reference<Lights::Spot> light_)
{
	lightsSpot.push_back(light_);
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Add(Reference<Environments::Cubemap> environment_)
{
	environmentsCubemap.push_back(environment_);
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Add(Reference<Environments::Globalmap> environment_)
{
	environmentsGlobalmap.push_back(environment_);
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Add(Reference<Decal> decal_)
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
inline void GreatVEngine::Graphics::OpenGL::Scene::Remove(Reference<OpenGL::Object> object_)
{
	auto it = std::find(objects.begin(), objects.end(), object_);
	if(it != objects.end())
	{
		object_->Unsubscribe_OnModelChanged(Function<void, Reference<Object>, Reference<Model>>(*this, &Scene::OnModelChanged));

		for(auto &child : object_->childObjects)
		{
			Remove(child);
		}
		for(auto &child : object_->childBones)
		{
			Remove(child);
		}

		objects.erase(it);
	}
	else
	{
		throw Exception("Failed to remove not added object");
	}
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Remove(Reference<OpenGL::Bone> bone_)
{
	for(auto &child : bone_->childObjects)
	{
		Remove(child);
	}
	for(auto &child : bone_->childBones)
	{
		Remove(child);
	}
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Remove(Reference<Lights::Direction> light_)
{
	lightsDirection.erase(std::find(lightsDirection.begin(), lightsDirection.end(), light_));
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Remove(Reference<Lights::Point> light_)
{
	lightsPoint.erase(std::find(lightsPoint.begin(), lightsPoint.end(), light_));
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Remove(Reference<Lights::Spot> light_)
{
	lightsSpot.erase(std::find(lightsSpot.begin(), lightsSpot.end(), light_));
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Remove(Reference<Environments::Cubemap> environment_)
{
	environmentsCubemap.erase(std::find(environmentsCubemap.begin(), environmentsCubemap.end(), environment_));
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Remove(Reference<Environments::Globalmap> environment_)
{
	environmentsGlobalmap.erase(std::find(environmentsGlobalmap.begin(), environmentsGlobalmap.end(), environment_));
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Remove(Reference<OpenGL::Decal> decal_)
{
	auto &container = decals[decal_->GetGroups()];
	container.erase(std::find(container.begin(), container.end(), decal_));
}
inline void GreatVEngine::Graphics::OpenGL::Scene::Render(Reference<Graphics::Camera> camera_, const Material::TechniqueType& techniqueType_)
{
	auto previousFrame = engine->context->GetBufferFrame();


	// render to geometry buffer
	geometyPass->frame->Set();
	{
		// sort by material/shape
		std::sort(objects.begin(), objects.end(), [](Reference<Object> a, Reference<Object> b){
			return *a.get() < *b.get();
		});

		glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glFrontFace(GL_CCW); GreatVEngine::OpenGL::DebugTest();
		glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS); GreatVEngine::OpenGL::DebugTest();

		// glDisable(GL_STENCIL_TEST); DebugTest();
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glDisable(GL_BLEND); GreatVEngine::OpenGL::DebugTest();

		glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
		glClearDepth(1.0f);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); GreatVEngine::OpenGL::DebugTest();

		glViewport(0, 0, viewport.x, viewport.y);

		for(auto &object : objects)
		{
			if(object->IsVisible())
			{
				object->Visualize(shared_from_this());

				glStencilFunc(GL_ALWAYS, object->GetGroups(), 0xFF);

				auto &model = object->model;

				if(model)
				{
					auto shape = model->shape;
					auto material = model->material;
					auto program = model->material->techniques[(Size)techniqueType_]->program;

					model->Set(techniqueType_);

					if(object->isBonesEnabled)
					{
						program->SetBlock("BonesData", 0);
						object->bufferBonesData->BindBase(0);
						{
							object->bufferBonesData->Set();
							auto data = (UInt8*)object->bufferBonesData->Map();
							{
								for(Size i = 0; i < min(object->bones.size(), Object::MAX_BONES_COUNT); ++i)
								{
									auto &bone = object->bones[i];

									((Mat4*)data)[i] =
										bone->GetMMat()*
										(bone->GetParent() ? bone->origin.GetMIMat() * bone->GetParent()->GetMIMat() : bone->origin.GetMIMat())
										;
								}
							}
							object->bufferBonesData->Unmap();
						}
					}

					program->SetInt("textureColor", 0);
					program->SetInt("textureNormals", 1);
					program->SetInt("textureSpecular", 2);

					program->SetVec3("cameraPosition", camera_->GetPosition());
					program->SetMat4("viewProjectionMatrix", camera_->GetVPMat());
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

		glEnable(GL_CULL_FACE); glCullFace(GL_BACK); glFrontFace(GL_CCW); GreatVEngine::OpenGL::DebugTest();
		glDisable(GL_DEPTH_TEST); GreatVEngine::OpenGL::DebugTest();

		// glDisable(GL_STENCIL_TEST); DebugTest();
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GreatVEngine::OpenGL::DebugTest();

		decalPass->attributes->Set(); GreatVEngine::OpenGL::DebugTest();
		decalPass->vertices->Set(); GreatVEngine::OpenGL::DebugTest();
		decalPass->program->Set(); GreatVEngine::OpenGL::DebugTest();
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

	if(true)
	{
		// compressDepth
		lightPass->ambientOcclusion->frameCompressedDepth->Set();
		{
			glViewport(0, 0,
				lightPass->ambientOcclusion->compressedDepth->GetWidth(),
				lightPass->ambientOcclusion->compressedDepth->GetHeight());

			glDisable(GL_CULL_FACE); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_DEPTH_TEST); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_STENCIL_TEST); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_BLEND); GreatVEngine::OpenGL::DebugTest();

			geometyPass->depthStencil->Set(0);

			lightPass->ambientOcclusion->attributes->Set();
			lightPass->ambientOcclusion->vertices->Set();
			lightPass->ambientOcclusion->programCompressedDepth->Set();
			lightPass->ambientOcclusion->programCompressedDepth->SetVec2("screen", Vec2(viewport.x, viewport.y) / (Float32)LightPass::AmbientOcclusion::COMPRESSED_DEPTH_SCALE_FACTOR);
			lightPass->ambientOcclusion->programCompressedDepth->SetFloat("camFar", camera_->GetPerspective().zFar);
			lightPass->ambientOcclusion->programCompressedDepth->SetFloat("camFarMNear", camera_->GetPerspective().zFar - camera_->GetPerspective().zNear);
			lightPass->ambientOcclusion->programCompressedDepth->SetFloat("camFarXNear", camera_->GetPerspective().zFar * camera_->GetPerspective().zNear);
			lightPass->ambientOcclusion->programCompressedDepth->SetMat4("viewProjectionMatrix", camera_->GetViewProjectionMatrix() * Move4(camera_->GetPosition()));
			lightPass->ambientOcclusion->programCompressedDepth->SetMat4("viewProjectionInverseMatrix", Move4(-camera_->GetPosition()) * camera_->GetViewProjectionInverseMatrix());

			glDrawArrays(GL_POINTS, 0, 1);
		}
		// ambient occlusion
		lightPass->ambientOcclusion->frame->Set();
		// if(!GetAsyncKeyState('F'))
		{
			glViewport(0, 0, viewport.x, viewport.y);
			// glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			// glClear(GL_COLOR_BUFFER_BIT);

			glDisable(GL_CULL_FACE); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_DEPTH_TEST); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_STENCIL_TEST); GreatVEngine::OpenGL::DebugTest();
			glEnable(GL_BLEND); GreatVEngine::OpenGL::glBlendFuncSeparate(GL_ZERO, GL_SRC_ALPHA, GL_ZERO, GL_ONE); GreatVEngine::OpenGL::DebugTest();

			lightPass->ambientOcclusion->compressedDepth->Set(0);
			lightPass->ambientOcclusion->compressedDepth->GenerateMipmaps();
			// geometyPass->depthStencil->Set(0);
			geometyPass->normals->Set(1);

			lightPass->ambientOcclusion->attributes->Set();
			lightPass->ambientOcclusion->vertices->Set();
			lightPass->ambientOcclusion->program->Set();

			lightPass->ambientOcclusion->program->SetFloat("mipmapsCount", glm::log2((Float32)lightPass->ambientOcclusion->compressedDepth->GetWidth()));

			lightPass->ambientOcclusion->program->SetVec2("screen", Vec2(viewport.x, viewport.y));
			lightPass->ambientOcclusion->program->SetFloat("camFar", camera_->GetPerspective().zFar);
			lightPass->ambientOcclusion->program->SetFloat("camFarMNear", camera_->GetPerspective().zFar - camera_->GetPerspective().zNear);
			lightPass->ambientOcclusion->program->SetFloat("camFarXNear", camera_->GetPerspective().zFar * camera_->GetPerspective().zNear);
			lightPass->ambientOcclusion->program->SetMat4("viewProjectionMatrix", camera_->GetViewProjectionMatrix() * Move4(camera_->GetPosition()));
			lightPass->ambientOcclusion->program->SetMat4("viewProjectionInverseMatrix", Move4(-camera_->GetPosition()) * camera_->GetViewProjectionInverseMatrix());

			glDrawArrays(GL_POINTS, 0, 1);
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
			glDisable(GL_CULL_FACE); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_DEPTH_TEST); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_STENCIL_TEST); GreatVEngine::OpenGL::DebugTest();
			glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE); GreatVEngine::OpenGL::DebugTest();

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
			glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_DEPTH_TEST); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_STENCIL_TEST); GreatVEngine::OpenGL::DebugTest();
			glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE); GreatVEngine::OpenGL::DebugTest();

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
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE); GreatVEngine::OpenGL::DebugTest();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); GreatVEngine::OpenGL::DebugTest();
		glClear(GL_COLOR_BUFFER_BIT); GreatVEngine::OpenGL::DebugTest();
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); GreatVEngine::OpenGL::DebugTest();

		// Set textures
		geometyPass->color->Set(0);
		geometyPass->specular->Set(1);
		geometyPass->normals->Set(2);
		geometyPass->material->Set(3);
		geometyPass->depthStencil->Set(4);
		target->diffuse->Set(5);

		/*environmentPass->localReflections;
		{
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			glEnable(GL_BLEND); glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_ONE, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

			environmentPass->localReflections->attributes->Set();
			environmentPass->localReflections->vertices->Set();
			environmentPass->localReflections->program->Set();
			environmentPass->localReflections->program->SetVec2("screen", Vec2(viewport.x, viewport.y));
			environmentPass->localReflections->program->SetFloat("camFar", camera_->GetPerspective().zFar);
			environmentPass->localReflections->program->SetFloat("camFarMNear", camera_->GetPerspective().zFar - camera_->GetPerspective().zNear);
			environmentPass->localReflections->program->SetFloat("camFarXNear", camera_->GetPerspective().zFar * camera_->GetPerspective().zNear);
			environmentPass->localReflections->program->SetMat4("viewProjectionMatrix", camera_->GetViewProjectionMatrix() * Move4(camera_->GetPosition()));
			environmentPass->localReflections->program->SetMat4("viewProjectionInverseMatrix", Move4(-camera_->GetPosition()) * camera_->GetViewProjectionInverseMatrix());

			glDrawArrays(GL_POINTS, 0, 1);
		}*/

		environmentPass->cubemap;
		{
			std::sort(environmentsCubemap.begin(), environmentsCubemap.end(), [](Reference<Environments::Cubemap> a, Reference<Environments::Cubemap> b){ return a->GetPriority() < b->GetPriority(); }); // bigger priority goes first

			environmentPass->cubemap->sampler->Set(8);

			glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_DEPTH_TEST); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_STENCIL_TEST); GreatVEngine::OpenGL::DebugTest();
			glEnable(GL_BLEND); GreatVEngine::OpenGL::glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_ONE, GL_ONE_MINUS_DST_ALPHA, GL_ONE); GreatVEngine::OpenGL::DebugTest();

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

			glDisable(GL_CULL_FACE); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_DEPTH_TEST); GreatVEngine::OpenGL::DebugTest();
			glDisable(GL_STENCIL_TEST); GreatVEngine::OpenGL::DebugTest();
			glEnable(GL_BLEND); GreatVEngine::OpenGL::glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_ONE, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

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

	// post processing
	/*postProcessing;
	{
		// bloom
		postProcessing->bloom;
		{
			// filter
			postProcessing->bloom->holder->frame->Set();
			{
				target->diffuse->Set(0);
				target->specular->Set(1);

				glDisable(GL_CULL_FACE); GreatVEngine::OpenGL::DebugTest();
				glDisable(GL_DEPTH_TEST); GreatVEngine::OpenGL::DebugTest();
				glDisable(GL_STENCIL_TEST); GreatVEngine::OpenGL::DebugTest();
				glDisable(GL_BLEND); GreatVEngine::OpenGL::DebugTest();

				postProcessing->bloom->intensityFilter->attributes->Set();
				postProcessing->bloom->intensityFilter->vertices->Set();
				postProcessing->bloom->intensityFilter->program->Set();
				postProcessing->bloom->intensityFilter->program->SetVec2("screen", Vec2(viewport.x, viewport.y));

				glDrawArrays(GL_POINTS, 0, 1);
			}
		}
	}*/

	// enable buffer to draw
	if(previousFrame)
	{
		previousFrame->Set();
	}
	else
	{
		engine->context->SetBufferFrame(nullptr);
	}

	auto draw = [&](GreatVEngine::OpenGL::Texture* texture)
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

	/*// draw bloom
	postProcessing->bloom->blur;
	{
		postProcessing->bloom->holder->color->Set(0);

		glDisable(GL_CULL_FACE); GreatVEngine::OpenGL::DebugTest();
		glDisable(GL_DEPTH_TEST); GreatVEngine::OpenGL::DebugTest();
		glDisable(GL_STENCIL_TEST); GreatVEngine::OpenGL::DebugTest();
		glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE); GreatVEngine::OpenGL::DebugTest();

		postProcessing->bloom->blur->attributes->Set();
		postProcessing->bloom->blur->vertices->Set();
		postProcessing->bloom->blur->program->Set();
		postProcessing->bloom->blur->program->SetFloat("mipmapsCount", glm::log2((Float32)postProcessing->bloom->holder->color->GetWidth()));
		postProcessing->bloom->blur->program->SetVec2("screen", Vec2(viewport.x, viewport.y));

		glDrawArrays(GL_POINTS, 0, 1);
	}*/

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw(target->specular);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);

	drawer;
	{
		drawer->attributesLines2D->Set();
		drawer->vertices->Set();
		drawer->programLines2D->Set();

		for(auto &line : drawer->lines2D)
		{
			drawer->programLines2D->SetVec2("p1", (line.p1 / Vec2(viewport.x, viewport.y)) *2.0f - 1.0f);
			drawer->programLines2D->SetVec2("p2", (line.p2 / Vec2(viewport.x, viewport.y)) *2.0f - 1.0f);
			drawer->programLines2D->SetVec4("c1", line.c1);
			drawer->programLines2D->SetVec4("c2", line.c2);

			glDrawArrays(GL_POINTS, 0, 1);
		}

		drawer->lines2D.clear();


		drawer->attributesLines3D->Set();
		drawer->vertices->Set();
		drawer->programLines3D->Set();

		drawer->programLines3D->SetMat4("viewProjectionMatrix", camera_->GetVPMat());

		for(auto &line : drawer->lines3D)
		{
			drawer->programLines3D->SetVec3("p1", line.p1);
			drawer->programLines3D->SetVec3("p2", line.p2);
			drawer->programLines3D->SetVec4("c1", line.c1);
			drawer->programLines3D->SetVec4("c2", line.c2);

			glDrawArrays(GL_POINTS, 0, 1);
		}

		drawer->lines3D.clear();


		drawer->attributesCircles2D->Set();
		drawer->vertices->Set();
		drawer->programCircles2D->Set();

		drawer->programCircles2D->SetVec2("screen", Vec2(viewport));

		for(auto &circle : drawer->circles2D)
		{
			drawer->programCircles2D->SetVec2("p", circle.p);
			drawer->programCircles2D->SetFloat("s", circle.s);
			drawer->programCircles2D->SetVec4("c", circle.c);

			glDrawArrays(GL_POINTS, 0, 1);
		}

		drawer->circles2D.clear();


		drawer->attributesCircles3D->Set();
		drawer->vertices->Set();
		drawer->programCircles3D->Set();

		drawer->programCircles3D->SetMat3("rotateMatrix", camera_->GetRMat());
		drawer->programCircles3D->SetMat4("viewProjectionMatrix", camera_->GetVPMat());

		for(auto &circle : drawer->circles3D)
		{
			drawer->programCircles3D->SetVec3("p", circle.p);
			drawer->programCircles3D->SetFloat("s", circle.s);
			drawer->programCircles3D->SetVec4("c", circle.c);

			glDrawArrays(GL_POINTS, 0, 1);
		}

		drawer->circles3D.clear();
	}
	// draw(geometyPass->color);

	if(GetAsyncKeyState(VK_F1)) draw(geometyPass->color);
	if(GetAsyncKeyState(VK_F2)) draw(geometyPass->specular);
	if(GetAsyncKeyState(VK_F3)) draw(geometyPass->normals);
	if(GetAsyncKeyState(VK_F4)) draw(geometyPass->material);
	if(GetAsyncKeyState(VK_F5)) draw(geometyPass->depthStencil);
	if(GetAsyncKeyState(VK_F6)) draw(target->diffuse);
	if(GetAsyncKeyState(VK_F7)) draw(target->specular);
	if(GetAsyncKeyState(VK_F8)) draw(lightPass->ambientOcclusion->compressedDepth);
	if(GetAsyncKeyState(VK_F9)) draw(postProcessing->bloom->holder->color);

	glFlush();
}
#pragma endregion


#pragma region
#pragma endregion

