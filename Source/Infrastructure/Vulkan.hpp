#pragma region Include
#pragma once


#include <mutex>

#include "Graphics.hpp"


#include <APIs/Vulkan/Wrapper.hpp>
#include <Utilities/OpenIL/Functions.hpp>
#pragma endregion


namespace Log
{
	const GreatVEngine::String filename = "log.log";
	std::mutex m;

	void Clear()
	{
		m.lock();
		FILE* file;
		fopen_s(&file, filename.c_str(), "w");
		fclose(file);
		m.unlock();
	}
	void Write(const GreatVEngine::String& message)
	{
		// m.lock();
		// FILE* file;
		// fopen_s(&file, filename.c_str(), "a");
		// fwrite(message.data(), message.size(), 1, file);
		// fclose(file);
		// m.unlock();
	}
}


namespace GreatVEngine
{
	namespace Vulkan
	{
		class Engine;
		class Technique;
		class Map;
		class Material;
		class Shape;
		class Model;
		class Object;
		class Scene;

		class Engine
		{
			friend Technique;
			friend Map;
			friend Material;
			friend Shape;
			friend Model;
			friend Object;
			friend Scene;
		protected:
			Instance* instance;
			PhysicalDevice* physicalDevice;
			Device* device;
			SwapchainKHR* swapchain;
			Vector<ImageView*> swapchainImageViews;
			Image* swapchainImageDepth;
			ImageView* swapchainImageViewDepth;
			Vector<Framebuffer*> swapchainFramebuffers_Clear;
			Vector<Framebuffer*> swapchainFramebuffers_Geometry;
			SurfaceKHR* surface;
			CommandPool* commandPool;
			Queue* queue;
			RenderPass* renderPass_Clear;
			RenderPass* renderPass_Geometry;
			DescriptorPool* descriptorPool_Geometry;
			DescriptorSetLayout* descriptorSetLayout_Geometry;
			Sampler* sampler_Linear;
			Fence* swapchainFence;
		public:
			inline Engine(HINSTANCE winInstance_, HWND winWindow_);
			inline ~Engine();
		};

		class Technique:
			public Graphics::Technique
		{
			friend Engine;
			friend Map;
			friend Material;
			friend Shape;
			friend Model;
			friend Object;
			friend Scene;
		protected:
			Reference<Engine> engine;
			Shader* vertex;
			Shader* tessellationControl;
			Shader* tessellationEvaluation;
			Shader* geometry;
			Shader* fragment;
		public:
			inline Technique(
				Reference<Engine> engine_,
				const Shader::Source& sourceVertex_,
				const Shader::Source& sourceTessellationControl_,
				const Shader::Source& sourceTessellationEvaluation_,
				const Shader::Source& sourceGeometry_,
				const Shader::Source& sourceFragment_):
				engine(engine_),
				vertex(!sourceVertex_.empty() ? new Shader(engine->device, sourceVertex_) : nullptr),
				tessellationControl(!sourceTessellationControl_.empty() ? new Shader(engine->device, sourceTessellationControl_) : nullptr),
				tessellationEvaluation(!sourceTessellationEvaluation_.empty() ? new Shader(engine->device, sourceTessellationEvaluation_) : nullptr),
				geometry(!sourceGeometry_.empty() ? new Shader(engine->device, sourceGeometry_) : nullptr),
				fragment(!sourceFragment_.empty() ? new Shader(engine->device, sourceFragment_) : nullptr)
			{
			}
			inline ~Technique()
			{
				delete vertex;
				delete tessellationControl;
				delete tessellationEvaluation;
				delete geometry;
				delete fragment;
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
					vertex_.size() > 0 ? System::LoadFileContent<UInt32>(base_ + vertex_) : Shader::Source(),
					tessellationControl_.size() > 0 ? System::LoadFileContent<UInt32>(base_ + tessellationControl_) : Shader::Source(),
					tessellationEvaluation_.size() > 0 ? System::LoadFileContent<UInt32>(base_ + tessellationEvaluation_) : Shader::Source(),
					geometry_.size() > 0 ? System::LoadFileContent<UInt32>(base_ + geometry_) : Shader::Source(),
					fragment_.size() > 0 ? System::LoadFileContent<UInt32>(base_ + fragment_) : Shader::Source()));
			}
		};
		class Map:
			public Graphics::Map
		{
			friend Engine;
			friend Technique;
			friend Material;
			friend Shape;
			friend Model;
			friend Object;
			friend Scene;
		protected:
			Image* image;
			ImageView* imageView;
		public:
			inline Map(Image* image_, ImageView* imageView_):
				image(image_),
				imageView(imageView_)
			{
			}
			inline ~Map()
			{
				delete image;
			}
		public:
			inline static Reference<Map> Load2D(Reference<Engine> engine_, const Filename& filename_)
			{
				auto image = OpenIL::Image::Load2D(filename_);
				{
					if(image->GetOriginMode() != OpenIL::Image::Origin::LowerLeft)
					{
						image->Flip();
					}
				}

				// auto texture = new OpenGL::Texture(
				// 	context_.get(),
				// 	image->GetWidth(), image->GetHeight(), image->GetDepth(),
				// 	OpenGL::Texture::Type::D2, OpenGL::Texture::InternalFormat::RGBA8,
				// 	OpenGL::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
				// 	OpenGL::GetComponentType(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
				// 	OpenGL::Texture::Wrap::Repeat, OpenGL::Texture::Filter::Linear, image->GetRawData());

				auto texture = new Image(
					engine_->device,
					{image->GetWidth(), image->GetHeight(), image->GetDepth()},
					Image::Type::VK_IMAGE_TYPE_2D, Image::Format::VK_FORMAT_R8G8B8A8_UNORM,
					Image::UsageBits::VK_IMAGE_USAGE_SAMPLED_BIT, Image::Tiling::VK_IMAGE_TILING_LINEAR,
					Image::Layout::VK_IMAGE_LAYOUT_PREINITIALIZED);
				{
					auto memory = new DeviceMemory(texture, Vulkan::DeviceMemory::Properties::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
					{
						auto data = memory->Map();
						{
							auto usedFormat = GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType()));

							if(usedFormat != texture->GetFormat())
							{
								auto transformedData = Vulkan::Image::TransformData(
									image->GetData(),
									usedFormat,
									texture->GetFormat());

								memcpy(data, transformedData.data(), memory->GetSize());
							}
							else
							{
								memcpy(data, image->GetRawData(), image->GetBytesPerPixel()*image->GetWidth()*image->GetHeight());
							}
						}
						memory->Unmap();
					}
					texture->SetDeviceMemory(memory);
				}
				auto textureView = new Vulkan::ImageView(engine_->device, texture, Vulkan::ImageView::Type::VK_IMAGE_VIEW_TYPE_2D, Vulkan::ImageView::AspectBits::VK_IMAGE_ASPECT_COLOR_BIT);

				return MakeReference(new Map(texture, textureView));
			}
		};
		class Material:
			public Graphics::Material
		{
			friend Engine;
			friend Technique;
			friend Map;
			friend Shape;
			friend Model;
			friend Object;
			friend Scene;
		protected:
			Reference<Engine> engine;
			Array<Reference<Technique>, TechniquesCount> techniques;
			Array<Reference<Map>, MapsCount> maps;
		public:
			inline Material(Reference<Engine> engine_):
				engine(engine_)
			{
			}
			inline ~Material()
			{
			}
		public:
			inline Reference<Technique>& Technique(const TechniqueType& techniqueType_)
			{
				return techniques[(Size)techniqueType_];
			}
			inline Reference<Map>& Map(const MapType& mapType_)
			{
				return maps[(Size)mapType_];
			}
		};

		class Shape:
			public Graphics::Shape
		{
			friend Engine;
			friend Technique;
			friend Map;
			friend Material;
			friend Shape;
			friend Model;
			friend Object;
			friend Scene;
		public:
			static const Geometry::VertexPackMode vertexPackMode = Geometry::VertexPackMode::Pos32F_TBN32F_Tex32F;
			static const Geometry::IndexPackMode indexPackMode = Geometry::IndexPackMode::UInt32;
		protected:
			Reference<Engine> engine;
			Buffer* vertices;
			Buffer* indices;
			Size verticesCount;
			Size indicesCount;
		public:
			inline Shape(Reference<Engine> engine_, Reference<Geometry> geometry_):
				engine(engine_),
				verticesCount(geometry_->vertices.size()),
				indicesCount(geometry_->indices.size())
			{
				auto sourceVertices = geometry_->GetVertices();
				auto sourceIndices = geometry_->GetIndices();

				vertices = new Buffer(engine->device, *sourceVertices.get(), Buffer::Usages::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, Buffer::Sharing::VK_SHARING_MODE_EXCLUSIVE);
				{
					auto memory = new Vulkan::DeviceMemory(vertices);
					{
						auto data = memory->Map<Float32>();
						{
							memcpy(data, sourceVertices->data(), sourceVertices->size());
						}
						memory->Unmap();
					}
					vertices->SetDeviceMemory(memory);
				}
				indices = new Buffer(engine->device, *sourceIndices.get(), Buffer::Usages::VK_BUFFER_USAGE_INDEX_BUFFER_BIT, Buffer::Sharing::VK_SHARING_MODE_EXCLUSIVE);
				{
					auto memory = new Vulkan::DeviceMemory(indices);
					{
						auto data = memory->Map<Float32>();
						{
							memcpy(data, sourceIndices->data(), sourceIndices->size());
						}
						memory->Unmap();
					}
					indices->SetDeviceMemory(memory);
				}
			}
			inline ~Shape()
			{
				delete vertices->GetDeviceMemory(); delete vertices;
				delete indices->GetDeviceMemory(); delete indices;
			}
		};

		class Model:
			public Graphics::Model
		{
			friend Engine;
			friend Technique;
			friend Map;
			friend Material;
			friend Shape;
			friend Object;
			friend Scene;
		protected:
			const Reference<Engine> engine;
			const Reference<Shape> shape;
			const Reference<Material> material;
			PipelineLayout* pipelineLayout;
			PipelineCache* pipelineCache;
			Array<GraphicPipeline*, Material::TechniquesCount> pipelines;
		public:
			inline Model(Reference<Engine> engine_, Reference<Shape> shape_, Reference<Material> material_):
				engine(engine_),
				shape(shape_),
				material(material_)
			{
				pipelineLayout = new Vulkan::PipelineLayout(engine->device, {engine->descriptorSetLayout_Geometry});
				pipelineCache = new Vulkan::PipelineCache(engine->device);

				for(Size i = 0; i < Material::TechniquesCount; ++i)
				{
					auto &pipeline = pipelines[i];
					auto &technique = material->techniques[i];

					if(technique)
					{
						pipeline = new Vulkan::GraphicPipeline(
							engine->device, pipelineCache, pipelineLayout, engine->renderPass_Geometry,
							Vulkan::GraphicPipeline::Shaders(
								technique->vertex,
								nullptr,
								nullptr,
								nullptr,
								technique->fragment), {
								Vulkan::GraphicPipeline::Binding(0, Geometry::GetVertexSize(shape->vertexPackMode), Vulkan::GraphicPipeline::Binding::Rate::VK_VERTEX_INPUT_RATE_VERTEX)
							}, {
								Vulkan::GraphicPipeline::Attribute(0, 0, Vulkan::GraphicPipeline::Attribute::Format::VK_FORMAT_R32G32B32_SFLOAT, sizeof(Float32) * 3 * 0),
								Vulkan::GraphicPipeline::Attribute(1, 0, Vulkan::GraphicPipeline::Attribute::Format::VK_FORMAT_R32G32B32_SFLOAT, sizeof(Float32)* 3 * 1),
								Vulkan::GraphicPipeline::Attribute(2, 0, Vulkan::GraphicPipeline::Attribute::Format::VK_FORMAT_R32G32B32_SFLOAT, sizeof(Float32)* 3 * 2),
								Vulkan::GraphicPipeline::Attribute(3, 0, Vulkan::GraphicPipeline::Attribute::Format::VK_FORMAT_R32G32B32_SFLOAT, sizeof(Float32)* 3 * 3),
								Vulkan::GraphicPipeline::Attribute(4, 0, Vulkan::GraphicPipeline::Attribute::Format::VK_FORMAT_R32G32_SFLOAT, sizeof(Float32)* 3 * 4)},
							{VkViewport{0, 0, (Float32)engine->surface->GetCapabilities().currentExtent.width, (Float32)engine->surface->GetCapabilities().currentExtent.height, 0.0f, 1.0f}},
							{VkRect2D{{0, 0}, {engine->surface->GetCapabilities().currentExtent.width, engine->surface->GetCapabilities().currentExtent.height}}},
							Vulkan::GraphicPipeline::Topology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
							Vulkan::GraphicPipeline::Fill::VK_POLYGON_MODE_FILL,
							Vulkan::GraphicPipeline::Culls::VK_CULL_MODE_BACK_BIT,
							Vulkan::GraphicPipeline::Face::VK_FRONT_FACE_COUNTER_CLOCKWISE,
							Vulkan::GraphicPipeline::DepthState(true, true, false, Vulkan::GraphicPipeline::DepthState::Operation::VK_COMPARE_OP_LESS),
							Vulkan::GraphicPipeline::BlendState(
								false, Vulkan::GraphicPipeline::BlendState::Logic::VK_LOGIC_OP_CLEAR,
								Vec4(1.0f, 1.0f, 1.0f, 1.0f), {
									Vulkan::GraphicPipeline::BlendState::Attachment(
										false,
										Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ONE,
										Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ONE,
										Vulkan::GraphicPipeline::BlendState::Operation::VK_BLEND_OP_ADD,
										Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ZERO,
										Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ZERO,
										Vulkan::GraphicPipeline::BlendState::Operation::VK_BLEND_OP_ADD)
								}
						));
					}
					else
					{
						pipeline = nullptr;
					}
				}
			}
			inline ~Model()
			{
				for(auto &pipeline : pipelines)
				{
					if(pipeline) delete pipeline;
				}
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
				// onModelChange(model_);
				model = model_;
			}
		};

		class Scene:
			public Graphics::Scene
		{
			friend Engine;
			friend Technique;
			friend Map;
			friend Material;
			friend Shape;
			friend Model;
			friend Object;
		protected:
			Reference<Engine> engine;
			Vector<Reference<Object>> objects;
		public:
			inline Scene(Reference<Engine> engine_):
				engine(engine_)
			{
			}
			inline ~Scene()
			{
			}
		public:
			inline void Add(Reference<Object> object_);
			inline void Remove(Reference<Object> object_);
			inline void Render(Reference<Graphics::Camera> camera_, const Material::TechniqueType& techniqueType_ = Material::TechniqueType::Basic);
		};
	}
}


#pragma region Engine
inline GreatVEngine::Vulkan::Engine::Engine(HINSTANCE winInstance_, HWND winWindow_)
{
	instance;
	{
		auto layersProperties = Instance::EnumerateLayersProperties();
		auto extensionProperties = Instance::EnumerateExtensionsProperties(layersProperties);

		Layer::CNames instanceLayersNames =
#if GVE_DEBUG
			Instance::GetLayersCNames(layersProperties, {
			"VK_LAYER_LUNARG_vktrace",
			"VK_LAYER_LUNARG_standard_validation",
			"VK_LAYER_LUNARG_api_dump"});
#else
			{};
#endif

		Extension::CNames instanceExtensionsNames = Instance::GetExtensionsCNames(extensionProperties);

		instance = new Instance(instanceLayersNames, instanceExtensionsNames);

#if GVE_DEBUG
		// instance->Subscribe_OnDebug([](String message)
		// {
		// 	// cout << message << endl;
		// 	Log::Write(message + "\n");
		// });
#endif
	}

	physicalDevice;
	{
		physicalDevice = instance->GetPhysicalDevices()[0];
	}

	device;
	{
		auto layersProperties = Vulkan::Device::EnumerateLayersProperties(physicalDevice);
		auto extensionProperties = Vulkan::Device::EnumerateExtensionsProperties(physicalDevice, layersProperties);

		Layer::CNames deviceLayersNames =
#if GVE_DEBUG
			Vulkan::Device::GetLayersCNames(layersProperties, {});
#else
			{};
#endif
		Extension::CNames deviceExtensionsNames = Vulkan::Device::GetExtensionsCNames(extensionProperties);

		device = new Device(physicalDevice, deviceLayersNames, deviceExtensionsNames);
	}

	surface = new Vulkan::SurfaceKHR(physicalDevice, winInstance_, winWindow_);
	swapchain = new Vulkan::SwapchainKHR(
		device, surface,
		surface->GetCapabilities().currentExtent,
		surface->GetFormats()[0].format, surface->GetFormats()[0].colorSpace);
	auto swapchainImages = swapchain->GetImages();

	swapchainImageDepth = new Vulkan::Image(
		device, {surface->GetCapabilities().currentExtent.width, surface->GetCapabilities().currentExtent.height, 1}, Vulkan::Image::Type::VK_IMAGE_TYPE_2D,
		Vulkan::Image::Format::VK_FORMAT_D16_UNORM, Vulkan::Image::UsageBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		Vulkan::Image::Tiling::VK_IMAGE_TILING_OPTIMAL, Vulkan::Image::Layout::VK_IMAGE_LAYOUT_UNDEFINED);
	{
		auto memory = new Vulkan::DeviceMemory(swapchainImageDepth);
		swapchainImageDepth->SetDeviceMemory(memory);
	}
	swapchainImageViewDepth = new Vulkan::ImageView(device, swapchainImageDepth, Vulkan::ImageView::Type::VK_IMAGE_VIEW_TYPE_2D, Vulkan::ImageView::AspectBits::VK_IMAGE_ASPECT_DEPTH_BIT);

	renderPass_Clear = [&]()
	{
		Vector<VkAttachmentDescription> vk_attachmentDescriptions(2);
		{
			vk_attachmentDescriptions[0];
			{
				vk_attachmentDescriptions[0].flags = 0; // VkAttachmentDescriptionFlagBits::VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
				vk_attachmentDescriptions[0].format = surface->GetFormats()[0].format;
				vk_attachmentDescriptions[0].samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
				vk_attachmentDescriptions[0].loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
				vk_attachmentDescriptions[0].storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
				vk_attachmentDescriptions[0].stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vk_attachmentDescriptions[0].stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vk_attachmentDescriptions[0].initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
				vk_attachmentDescriptions[0].finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			}
			vk_attachmentDescriptions[1];
			{
				vk_attachmentDescriptions[1].flags = 0;
				vk_attachmentDescriptions[1].format = swapchainImageDepth->GetFormat();
				vk_attachmentDescriptions[1].samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
				vk_attachmentDescriptions[1].loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
				vk_attachmentDescriptions[1].storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
				vk_attachmentDescriptions[1].stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vk_attachmentDescriptions[1].stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vk_attachmentDescriptions[1].initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
				vk_attachmentDescriptions[1].finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
			}
		}

		Vector<VkAttachmentReference> vk_colorAttachmentReferences(1);
		{
			vk_colorAttachmentReferences[0];
			{
				vk_colorAttachmentReferences[0].attachment = 0;
				vk_colorAttachmentReferences[0].layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			}
		}
		VkAttachmentReference vk_depthAttachmentReference;
		{
			vk_depthAttachmentReference.attachment = 1;
			vk_depthAttachmentReference.layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}
		Vector<VkSubpassDescription> vk_subpassDescriptions(1);
		{
			vk_subpassDescriptions[0].flags = 0;
			vk_subpassDescriptions[0].pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
			vk_subpassDescriptions[0].inputAttachmentCount = 0;
			vk_subpassDescriptions[0].pInputAttachments = nullptr;
			vk_subpassDescriptions[0].colorAttachmentCount = vk_colorAttachmentReferences.size();
			vk_subpassDescriptions[0].pColorAttachments = vk_colorAttachmentReferences.data();
			vk_subpassDescriptions[0].pResolveAttachments = 0;
			vk_subpassDescriptions[0].pDepthStencilAttachment = &vk_depthAttachmentReference;
			vk_subpassDescriptions[0].preserveAttachmentCount = 0;
			vk_subpassDescriptions[0].pPreserveAttachments = nullptr;
		}

		return new RenderPass(device, vk_attachmentDescriptions, vk_subpassDescriptions);
	}();
	renderPass_Geometry = [&]()
	{
		Vector<VkAttachmentDescription> vk_attachmentDescriptions(2);
		{
			vk_attachmentDescriptions[0];
			{
				vk_attachmentDescriptions[0].flags = 0; // VkAttachmentDescriptionFlagBits::VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
				vk_attachmentDescriptions[0].format = surface->GetFormats()[0].format;
				vk_attachmentDescriptions[0].samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
				vk_attachmentDescriptions[0].loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
				vk_attachmentDescriptions[0].storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
				vk_attachmentDescriptions[0].stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vk_attachmentDescriptions[0].stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vk_attachmentDescriptions[0].initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
				vk_attachmentDescriptions[0].finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			}
			vk_attachmentDescriptions[1];
			{
				vk_attachmentDescriptions[1].flags = 0;
				vk_attachmentDescriptions[1].format = swapchainImageDepth->GetFormat();
				vk_attachmentDescriptions[1].samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
				vk_attachmentDescriptions[1].loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
				vk_attachmentDescriptions[1].storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
				vk_attachmentDescriptions[1].stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vk_attachmentDescriptions[1].stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vk_attachmentDescriptions[1].initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
				vk_attachmentDescriptions[1].finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
			}
		}

		Vector<VkAttachmentReference> vk_colorAttachmentReferences(1);
		{
			vk_colorAttachmentReferences[0];
			{
				vk_colorAttachmentReferences[0].attachment = 0;
				vk_colorAttachmentReferences[0].layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			}
		}
		VkAttachmentReference vk_depthAttachmentReference;
		{
			vk_depthAttachmentReference.attachment = 1;
			vk_depthAttachmentReference.layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}
		Vector<VkSubpassDescription> vk_subpassDescriptions(1);
		{
			vk_subpassDescriptions[0].flags = 0;
			vk_subpassDescriptions[0].pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
			vk_subpassDescriptions[0].inputAttachmentCount = 0;
			vk_subpassDescriptions[0].pInputAttachments = nullptr;
			vk_subpassDescriptions[0].colorAttachmentCount = vk_colorAttachmentReferences.size();
			vk_subpassDescriptions[0].pColorAttachments = vk_colorAttachmentReferences.data();
			vk_subpassDescriptions[0].pResolveAttachments = 0;
			vk_subpassDescriptions[0].pDepthStencilAttachment = &vk_depthAttachmentReference;
			vk_subpassDescriptions[0].preserveAttachmentCount = 0;
			vk_subpassDescriptions[0].pPreserveAttachments = nullptr;
		}

		return new RenderPass(device, vk_attachmentDescriptions, vk_subpassDescriptions);
	}();

	sampler_Linear = new Sampler(device);

	descriptorPool_Geometry = new Vulkan::DescriptorPool(
		device,
		1, {
		Vulkan::DescriptorPool::Param(Vulkan::DescriptorPool::Param::Type::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
		Vulkan::DescriptorPool::Param(Vulkan::DescriptorPool::Param::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 8),
	});
	descriptorSetLayout_Geometry = new Vulkan::DescriptorSetLayout(
		device, {
			Vulkan::DescriptorSetLayout::Binding(
				0, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_VERTEX_BIT),
			Vulkan::DescriptorSetLayout::Binding(
				1, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
			Vulkan::DescriptorSetLayout::Binding(
				2, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
			Vulkan::DescriptorSetLayout::Binding(
				3, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
			Vulkan::DescriptorSetLayout::Binding(
				4, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
			Vulkan::DescriptorSetLayout::Binding(
				5, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
			Vulkan::DescriptorSetLayout::Binding(
				6, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
			Vulkan::DescriptorSetLayout::Binding(
				7, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
			Vulkan::DescriptorSetLayout::Binding(
				8, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		});

	swapchainImageViews.resize(swapchainImages.size());
	swapchainFramebuffers_Clear.resize(swapchainImages.size());
	swapchainFramebuffers_Geometry.resize(swapchainImages.size());
	{
		for(Size i = 0; i < swapchainImages.size(); ++i)
		{
			swapchainImageViews[i] = new Vulkan::ImageView(device, swapchainImages[i], Vulkan::ImageView::Type::VK_IMAGE_VIEW_TYPE_2D);
			swapchainFramebuffers_Clear[i] = new Vulkan::Framebuffer(
				device, renderPass_Clear,
				{swapchainImageViews[i]->GetBasicImage()->GetExtent().width, swapchainImageViews[i]->GetBasicImage()->GetExtent().height},
				{swapchainImageViews[i], swapchainImageViewDepth});
			swapchainFramebuffers_Geometry[i] = new Vulkan::Framebuffer(
				device, renderPass_Geometry,
				{swapchainImageViews[i]->GetBasicImage()->GetExtent().width, swapchainImageViews[i]->GetBasicImage()->GetExtent().height},
				{swapchainImageViews[i], swapchainImageViewDepth});
		}
	}

	commandPool = new Vulkan::CommandPool(device, 0);

	queue = new Queue(device, 0, 0);

	swapchainFence = new Fence(device);
}
inline GreatVEngine::Vulkan::Engine::~Engine()
{
	delete swapchainFence;
	delete queue;
	delete commandPool;
	for(auto &framebuffer : swapchainFramebuffers_Clear) delete framebuffer;
	for(auto &framebuffer : swapchainFramebuffers_Geometry) delete framebuffer;
	for(auto &imageView : swapchainImageViews) delete imageView;
	delete descriptorSetLayout_Geometry;
	delete descriptorPool_Geometry;
	delete sampler_Linear;
	delete renderPass_Clear;
	delete renderPass_Geometry;
	delete swapchainImageViewDepth;
	delete swapchainImageDepth->GetDeviceMemory(); delete swapchainImageDepth;
	delete swapchain;
	delete surface;
	delete device;
	delete instance;
}
#pragma endregion
#pragma region Scene
inline void GreatVEngine::Vulkan::Scene::Add(Reference<Object> object_)
{
	/*struct T
	{
		Scene* scene;
		Reference<Object> object;
	};

	object_->Subscribe_OnModelChange(new T{this, object_}, [](void* data, Reference<Graphics::Model> model_){
		auto t = (T*)data;
		auto $this = t->scene;

		$this->Remove(t->object);
		$this->Add(t->object);

		return true;
	});

	objects.push_back(object_);*/
}
inline void GreatVEngine::Vulkan::Scene::Remove(Reference<Object> object_)
{
	objects.erase(std::find(objects.begin(), objects.end(), object_));
}
inline void GreatVEngine::Vulkan::Scene::Render(Reference<Graphics::Camera> camera_, const Material::TechniqueType& techniqueType_)
{
	auto currentSwapchainImage = engine->swapchain->AccuireNextImage(engine->swapchainFence);
	engine->swapchainFence->Wait();
	engine->swapchainFence->Reset();

	auto framebuffer = engine->swapchainFramebuffers_Clear[currentSwapchainImage];


	auto commandBuffer_Clear = new CommandBuffer(engine->commandPool, CommandBuffer::Level::Primary);
	{
		commandBuffer_Clear->Record(
			engine->renderPass_Clear, 0, nullptr, {
				new Commands::RenderPass(
					engine->renderPass_Clear, framebuffer,
					Commands::RenderPass::Content::VK_SUBPASS_CONTENTS_INLINE,
					{{0, 0}, engine->surface->GetCapabilities().currentExtent},
					{{0.16f, 0.16f, 0.16f, 1.0f}, {1.0f, 0}}, {
						// new Comm
					})
			});
	}
	engine->queue->Submit(commandBuffer_Clear);

	engine->queue->Wait();

	const Size uniformDataSize = sizeof(Float32)* (16 + 16 + 4*3);
	auto uniformBuffer = new Buffer(engine->device, uniformDataSize, Buffer::Usages::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, Buffer::Sharing::VK_SHARING_MODE_EXCLUSIVE);
	{
		auto memory = new Vulkan::DeviceMemory(uniformBuffer);
		{
			auto data = memory->Map<Float32>();
			{
				// memcpy(data, uniformData.data(), uniformData.size());
				memset(data, 0, uniformDataSize);
			}
			memory->Unmap();
		}
		uniformBuffer->SetDeviceMemory(memory);
	}


	for(auto &object : objects)
	{
		auto &model = object->model;
		auto &pipeline = model->pipelines[(Size)techniqueType_];

		auto shape = model->shape;
		auto material = model->material;
		auto vertices = shape->vertices;
		auto indices = shape->indices;

		auto uniformData = uniformBuffer->GetDeviceMemory()->Map<Float32>();
		{
			auto rMat = object->GetRMat();
			auto mMat = Move4(-camera_->GetPosition()) * object->GetMMat();
			auto mvpMat = camera_->GetVPMat() * object->GetMMat();

			*(Mat4*)(uniformData + 0)	= mvpMat;
			*(Mat4*)(uniformData + 16)	= mMat;
			((Vec4*)(uniformData + 32))[0]	= Vec4(rMat[0][0], rMat[0][1], rMat[0][2], 0.0f);
			((Vec4*)(uniformData + 32))[1]	= Vec4(rMat[1][0], rMat[1][1], rMat[1][2], 0.0f);
			((Vec4*)(uniformData + 32))[2]	= Vec4(rMat[2][0], rMat[2][1], rMat[2][2], 0.0f);
		}
		uniformBuffer->GetDeviceMemory()->Unmap();

		auto descriptorSet = new Vulkan::DescriptorSet(engine->descriptorPool_Geometry, engine->descriptorSetLayout_Geometry);
		{
			Vector<VkDescriptorBufferInfo> vk_descriptorBufferInfos(1);
			{
				vk_descriptorBufferInfos[0].buffer = uniformBuffer->GetHandle();
				vk_descriptorBufferInfos[0].offset = 0;
				vk_descriptorBufferInfos[0].range = VK_WHOLE_SIZE;
			}
			Vector<VkDescriptorImageInfo> vk_descriptorImageInfos;
			{
				for(auto &map : material->maps)
				{
					if(map && map->imageView)
					{
						VkDescriptorImageInfo vk_descriptorImageInfo;
						{
							vk_descriptorImageInfo.sampler = engine->sampler_Linear->GetHandle();
							vk_descriptorImageInfo.imageView = map->imageView->GetHandle();
							vk_descriptorImageInfo.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED;
						}

						vk_descriptorImageInfos.push_back(vk_descriptorImageInfo);
					}
				}
			}
			/*Vector<VkWriteDescriptorSet> vk_writeDescriptorSets(1);
			{
				vk_writeDescriptorSets[0];
				{
					vk_writeDescriptorSets[0].sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					vk_writeDescriptorSets[0].pNext = nullptr;
					vk_writeDescriptorSets[0].dstSet = descriptorSet->GetHandle();
					vk_writeDescriptorSets[0].dstBinding = 0;
					vk_writeDescriptorSets[0].dstArrayElement = 0;
					vk_writeDescriptorSets[0].descriptorCount = vk_descriptorBufferInfos.size();
					vk_writeDescriptorSets[0].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					vk_writeDescriptorSets[0].pImageInfo = nullptr;
					vk_writeDescriptorSets[0].pBufferInfo = vk_descriptorBufferInfos.data();
					vk_writeDescriptorSets[0].pTexelBufferView = nullptr;
				}

				if(vk_descriptorImageInfos.size() > 0)
				{
					VkWriteDescriptorSet vk_writeDescriptorSet;
					{
						vk_writeDescriptorSet.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						vk_writeDescriptorSet.pNext = nullptr;
						vk_writeDescriptorSet.dstSet = descriptorSet->GetHandle();
						vk_writeDescriptorSet.dstBinding = 1;
						vk_writeDescriptorSet.dstArrayElement = 0;
						vk_writeDescriptorSet.descriptorCount = vk_descriptorImageInfos.size();
						vk_writeDescriptorSet.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						vk_writeDescriptorSet.pImageInfo = vk_descriptorImageInfos.data();
						vk_writeDescriptorSet.pBufferInfo = nullptr;
						vk_writeDescriptorSet.pTexelBufferView = nullptr;
					}

					vk_writeDescriptorSets.push_back(vk_writeDescriptorSet);
				}
			}*/

			Vector<VkWriteDescriptorSet> vk_writeDescriptorSets(1);
			{
				vk_writeDescriptorSets[0];
				{
					vk_writeDescriptorSets[0].sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					vk_writeDescriptorSets[0].pNext = nullptr;
					vk_writeDescriptorSets[0].dstSet = descriptorSet->GetHandle();
					vk_writeDescriptorSets[0].dstBinding = 0;
					vk_writeDescriptorSets[0].dstArrayElement = 0;
					vk_writeDescriptorSets[0].descriptorCount = vk_descriptorBufferInfos.size();
					vk_writeDescriptorSets[0].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					vk_writeDescriptorSets[0].pImageInfo = nullptr;
					vk_writeDescriptorSets[0].pBufferInfo = vk_descriptorBufferInfos.data();
					vk_writeDescriptorSets[0].pTexelBufferView = nullptr;
				}

				for(Size i = 0; i < vk_descriptorImageInfos.size(); ++i)
				{
					VkWriteDescriptorSet vk_writeDescriptorSet;
					{
						vk_writeDescriptorSet.sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						vk_writeDescriptorSet.pNext = nullptr;
						vk_writeDescriptorSet.dstSet = descriptorSet->GetHandle();
						vk_writeDescriptorSet.dstBinding = 1 + i;
						vk_writeDescriptorSet.dstArrayElement = 0;
						vk_writeDescriptorSet.descriptorCount = 1; // vk_descriptorImageInfos.size();
						vk_writeDescriptorSet.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
						vk_writeDescriptorSet.pImageInfo = vk_descriptorImageInfos.data() + i;
						vk_writeDescriptorSet.pBufferInfo = nullptr;
						vk_writeDescriptorSet.pTexelBufferView = nullptr;
					}

					vk_writeDescriptorSets.push_back(vk_writeDescriptorSet);
				}
			}

			UpdateDescriptorSets(engine->device->GetHandle(), vk_writeDescriptorSets, Vector<VkCopyDescriptorSet>({}));
		}


		auto commandBuffer_Geometry = new CommandBuffer(engine->commandPool, CommandBuffer::Level::Primary);
		{
			commandBuffer_Geometry->Record(
				engine->renderPass_Geometry, 0, framebuffer, {
					new Vulkan::Commands::RenderPass(
					engine->renderPass_Geometry, framebuffer, Vulkan::Commands::RenderPass::Content::VK_SUBPASS_CONTENTS_INLINE,
					{{0, 0}, engine->surface->GetCapabilities().currentExtent}, {}, {
						new Vulkan::Commands::BindGraphicPipeline(pipeline),
						new Vulkan::Commands::BindVertexBuffers({vertices}, {0}),
						new Vulkan::Commands::BindIndexBuffer(indices, 0, Vulkan::Commands::BindIndexBuffer::Type::VK_INDEX_TYPE_UINT32),
						new Vulkan::Commands::BindGraphicDescriptorSet(model->pipelineLayout, {descriptorSet}),
						new Vulkan::Commands::DrawIndexed(0, shape->indicesCount, 0, 1)
					})
			});
		}

		engine->queue->Submit(commandBuffer_Geometry);

		engine->queue->Wait();

		delete commandBuffer_Geometry;
		delete descriptorSet;
	}

	engine->queue->Wait();
	engine->swapchain->Preset(engine->queue, currentSwapchainImage);

	delete uniformBuffer->GetDeviceMemory(); delete uniformBuffer;

	delete commandBuffer_Clear;
}
#pragma endregion


#pragma region
#pragma endregion

