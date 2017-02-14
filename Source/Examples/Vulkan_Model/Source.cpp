#include <iostream>
#include <mutex>
using namespace std;

#include <System/System.hpp>
#include <APIs/Vulkan/Vulkan.hpp>
#include <APIs/WinAPI/Window.hpp>
#include <Geometry/Geometry.hpp>
#include <Utilities/OpenIL/Functions.hpp>
namespace GVE = GreatVEngine;

using namespace GVE;


namespace Log
{
	const String filename = "log.log";
	mutex m;

	void Clear()
	{
		m.lock();
		FILE* file;
		fopen_s(&file, filename.c_str(), "w");
		fclose(file);
		m.unlock();
	}
	void Write(const String& message)
	{
		m.lock();
		FILE* file;
		fopen_s(&file, filename.c_str(), "a");
		fwrite(message.data(), message.size(), 1, file);
		fclose(file);
		m.unlock();
	}
}


void func()
{
	Log::Clear();

	auto winInstance = GreatVEngine::WinAPI::Instance::Get();
	auto windowClass = MakeReference(new GreatVEngine::WinAPI::WindowClass(winInstance, "class"));
	auto window = MakeReference(new GreatVEngine::WinAPI::Window(windowClass, "window"));
	auto winDeviceContext = MakeReference(new GreatVEngine::WinAPI::DeviceContext(window));
	{
		winDeviceContext->SetPixelFormat();
	}


	Helper::Transformation::Dimension3::ViewProjectionMatrix camera(
		Vec3(0.0f, 8.0f, -20.0f),
		Vec3(20.0f, 0.0f, 0.0f),
		Helper::Transformation::Dimension3::Projection::Params::Perspective(60.0f, 800.0f / 600.0f, 0.1f, 1000.0f));
	Helper::Transformation::Dimension3::ModelMatrix model(
		Vec3(0.0f, 0.0f, 0.0f),
		Vec3(0.0f),
		Vec3(1.0f));

	auto geometry = Geometry::CreateBox(Vec3(10.0f), Vec3(1.0f), UVec3(50));
	auto dataVertices = geometry->GetVertices();
	auto dataIndices = geometry->GetIndices();

	auto image = OpenIL::Image::Load2D("../../../../../Media/Images/Brick1_D.png");
	{
		if(image->GetOriginMode() != OpenIL::Image::Origin::LowerLeft)
		{
			image->Flip();
		}
	}

	auto vkInstance = [&]()
	{
		auto layersProperties = Vulkan::Instance::EnumerateLayersProperties();
		auto extensionProperties = Vulkan::Instance::EnumerateExtensionsProperties(layersProperties);

		auto layersNames = Vulkan::Instance::GetLayersCNames(layersProperties, {
			"VK_LAYER_LUNARG_vktrace",
			"VK_LAYER_LUNARG_standard_validation",
			"VK_LAYER_LUNARG_api_dump"
		});
		auto extensionsNames = Vulkan::Instance::GetExtensionsCNames(extensionProperties);

		auto instance = new Vulkan::Instance(layersNames, extensionsNames);

		instance->Subscribe_OnDebug(Function<void,String>([](String message)
		{
			// cout << message << endl;
			Log::Write(message + "\n");
		}));

		return instance;
	}();
	auto physicalDevice = [&]()
	{
		auto physicalDevices = vkInstance->GetPhysicalDevices();

		if(physicalDevices.empty())
		{
			throw Exception("no physical devices");
		}

		return physicalDevices[0];
	}();
	auto vkDevice = [&]()
	{
		auto layersProperties = Vulkan::Device::EnumerateLayersProperties(physicalDevice);
		auto extensionProperties = Vulkan::Device::EnumerateExtensionsProperties(physicalDevice, layersProperties);

		auto layersNames = Vulkan::Device::GetLayersCNames(layersProperties, {});
		auto extensionsNames = Vulkan::Device::GetExtensionsCNames(extensionProperties);

		return new Vulkan::Device(physicalDevice, layersNames, extensionsNames);
	}();

	auto surface = new Vulkan::SurfaceKHR(physicalDevice, winInstance->GetHangle(), window->GetHandle());
	auto swapchain = new Vulkan::SwapchainKHR(vkDevice, surface, surface->GetCapabilities().currentExtent, surface->GetFormats()[0].format, surface->GetFormats()[0].colorSpace);
	auto swapchainImages = swapchain->GetImages();

	auto imageDepth = new Vulkan::Image(
		vkDevice, {surface->GetCapabilities().currentExtent.width, surface->GetCapabilities().currentExtent.height, 1}, Vulkan::Image::Type::VK_IMAGE_TYPE_2D,
		Vulkan::Image::Format::VK_FORMAT_D32_SFLOAT, Vulkan::Image::UsageBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		Vulkan::Image::Tiling::VK_IMAGE_TILING_OPTIMAL, Vulkan::Image::Layout::VK_IMAGE_LAYOUT_UNDEFINED);
	{
		auto memory = new Vulkan::DeviceMemory(imageDepth);
		imageDepth->SetDeviceMemory(memory);
	}
	auto imageViewDepth = new Vulkan::ImageView(vkDevice, imageDepth, Vulkan::ImageView::Type::VK_IMAGE_VIEW_TYPE_2D, Vulkan::ImageView::AspectBits::VK_IMAGE_ASPECT_DEPTH_BIT);

	auto imageTexture = new Vulkan::Image(
		vkDevice, {image->GetWidth(), image->GetHeight(), 1}, Vulkan::Image::Type::VK_IMAGE_TYPE_2D,
		
		// Vulkan::Image::Format::VK_FORMAT_R8G8B8A8_UNORM, // OpenIL::GetVulkanFormat(image->GetFormat(), image->GetComponentType()),
		Vulkan::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType())),
		
		Vulkan::Image::UsageBits::VK_IMAGE_USAGE_SAMPLED_BIT,
		Vulkan::Image::Tiling::VK_IMAGE_TILING_LINEAR, Vulkan::Image::Layout::VK_IMAGE_LAYOUT_PREINITIALIZED);
	{
		auto memory = new Vulkan::DeviceMemory(imageTexture, Vulkan::DeviceMemory::Properties::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		{
			auto data = memory->Map();
			{
				auto usedFormat = Vulkan::GetFormat(OpenIL::GetFormat(image->GetFormat(), image->GetComponentType()));

				if(usedFormat != imageTexture->GetFormat())
				{
					auto transformedData = Vulkan::Image::TransformData(
						image->GetData(),
						usedFormat,
						imageTexture->GetFormat());
				
					memcpy(data, transformedData.data(), memory->GetSize());
				}
				else
				{
					memcpy(data, image->GetRawData(), image->GetBytesPerPixel()*image->GetWidth()*image->GetHeight());
				}
			}
			memory->Unmap();
		}
		imageTexture->SetDeviceMemory(memory);
	}
	auto imageViewTexture = new Vulkan::ImageView(vkDevice, imageTexture, Vulkan::ImageView::Type::VK_IMAGE_VIEW_TYPE_2D, Vulkan::ImageView::AspectBits::VK_IMAGE_ASPECT_COLOR_BIT);
	auto samplerTexture = new Vulkan::Sampler(vkDevice);

	auto renderPass = [&]()
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
				vk_attachmentDescriptions[1].format = imageDepth->GetFormat();
				vk_attachmentDescriptions[1].samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
				vk_attachmentDescriptions[1].loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
				vk_attachmentDescriptions[1].storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
				vk_attachmentDescriptions[1].stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vk_attachmentDescriptions[1].stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vk_attachmentDescriptions[1].initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
				vk_attachmentDescriptions[1].finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
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

		return new Vulkan::RenderPass(vkDevice, vk_attachmentDescriptions, vk_subpassDescriptions);
	}();

	auto bufferVertices = new Vulkan::Buffer(
		vkDevice,
		dataVertices->size(),
		Vulkan::Buffer::Usages::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		Vulkan::Buffer::Sharing::VK_SHARING_MODE_EXCLUSIVE);
	{
		auto memory = new Vulkan::DeviceMemory(bufferVertices);
		{
			auto data = memory->Map<Float32>();
			{
				memcpy(data, dataVertices->data(), dataVertices->size());
			}
			memory->Unmap();
		}
		bufferVertices->SetDeviceMemory(memory);
	}
	auto bufferIndices = new Vulkan::Buffer(
		vkDevice,
		dataIndices->size(),
		Vulkan::Buffer::Usages::VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		Vulkan::Buffer::Sharing::VK_SHARING_MODE_EXCLUSIVE);
	{
		auto memory = new Vulkan::DeviceMemory(bufferIndices);
		{
			auto data = memory->Map<Float32>();
			{
				memcpy(data, dataIndices->data(), dataIndices->size());
			}
			memory->Unmap();
		}
		bufferIndices->SetDeviceMemory(memory);
	}
	auto bufferUniforms = new Vulkan::Buffer(
		vkDevice,
		sizeof(Float32)*16,
		Vulkan::Buffer::Usages::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		Vulkan::Buffer::Sharing::VK_SHARING_MODE_EXCLUSIVE);
	{
		auto memory = new Vulkan::DeviceMemory(bufferUniforms);
		{
			auto mat = Scale4(Vec3(1.0f, 1.0f, -1.0f)) * camera.GetVPMat() * model.GetMMat();
			// auto mat = camera.GetVPMat() * model.GetMMat();

			auto data = memory->Map<Float32>();
			{
				memcpy(data, &mat, sizeof(Float32)* 16);
			}
			memory->Unmap();
		}
		bufferUniforms->SetDeviceMemory(memory);
	}

	auto loadShader = [](const std::string& filename)
	{
		FILE* file = nullptr;
		if(fopen_s(&file, filename.c_str(), "rb") != 0)
		{
			throw std::exception("failed to load file");
		}
		fseek(file, 0, FILE_END);
		auto size = ftell(file);
		if(size % 4 != 0) throw std::exception("");

		rewind(file);

		std::vector<uint32_t> result(size);
		fread((void*)result.data(), 1, size, file);

		fclose(file);

		return std::move(result);
	};

	auto sourceShaderVertex = System::LoadFileContentBinary<UInt32>(Filepath("Media/Shaders/Vulkan_Model/1.spir-v.vs"));
	auto sourceShaderFragment = System::LoadFileContentBinary<UInt32>(Filepath("Media/Shaders/Vulkan_Model/1.spir-v.fs"));

	auto shaderVertex = new Vulkan::Shader(vkDevice, sourceShaderVertex);
	auto shaderFragment = new Vulkan::Shader(vkDevice, sourceShaderFragment);
	// auto shaderVertex = new Vulkan::Shader(vkDevice, loadShader("../../../../../Media/Shaders/1.vert.spv"));
	// auto shaderFragment = new Vulkan::Shader(vkDevice, loadShader("../../../../../Media/Shaders/1.frag.spv"));

	Vector<Vulkan::ImageView*> swapchainImageViews(swapchainImages.size());
	Vector<Vulkan::Framebuffer*> framebuffers(swapchainImages.size());
	{
		for(Size i = 0; i < swapchainImages.size(); ++i)
		{
			swapchainImageViews[i] = new Vulkan::ImageView(vkDevice, swapchainImages[i], Vulkan::ImageView::Type::VK_IMAGE_VIEW_TYPE_2D);
			framebuffers[i] = new Vulkan::Framebuffer(
				vkDevice, renderPass,
				{swapchainImageViews[i]->GetBasicImage()->GetExtent().width, swapchainImageViews[i]->GetBasicImage()->GetExtent().height},
				{swapchainImageViews[i], imageViewDepth});
		}
	}

	auto descriptorPool = new Vulkan::DescriptorPool(
		vkDevice,
		2, {
			Vulkan::DescriptorPool::Param(Vulkan::DescriptorPool::Param::Type::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1),
			Vulkan::DescriptorPool::Param(Vulkan::DescriptorPool::Param::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1),
		});
	auto descriptorSetLayout = new Vulkan::DescriptorSetLayout(
		vkDevice, {
			Vulkan::DescriptorSetLayout::Binding(
				0, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_VERTEX_BIT),
			Vulkan::DescriptorSetLayout::Binding(
				1, Vulkan::DescriptorSetLayout::Binding::Type::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				1, Vulkan::DescriptorSetLayout::Binding::FlagBits::VK_SHADER_STAGE_FRAGMENT_BIT),
		});
	auto descriptorSet = new Vulkan::DescriptorSet(descriptorPool, descriptorSetLayout);
	{
		Vector<VkDescriptorBufferInfo> vk_descriptorBufferInfos(1);
		{
			vk_descriptorBufferInfos[0].buffer = bufferUniforms->GetHandle();
			vk_descriptorBufferInfos[0].offset = 0;
			vk_descriptorBufferInfos[0].range = VK_WHOLE_SIZE;
		}
		Vector<VkDescriptorImageInfo> vk_descriptorImageInfos(1);
		{
			vk_descriptorImageInfos[0].sampler = samplerTexture->GetHandle();
			vk_descriptorImageInfos[0].imageView = imageViewTexture->GetHandle();
			vk_descriptorImageInfos[0].imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_PREINITIALIZED;
		}

		Vector<VkWriteDescriptorSet> vk_writeDescriptorSet(2);
		{
			vk_writeDescriptorSet[0];
			{
				vk_writeDescriptorSet[0].sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				vk_writeDescriptorSet[0].pNext = nullptr;
				vk_writeDescriptorSet[0].dstSet = descriptorSet->GetHandle();
				vk_writeDescriptorSet[0].dstBinding = 0;
				vk_writeDescriptorSet[0].dstArrayElement = 0;
				vk_writeDescriptorSet[0].descriptorCount = vk_descriptorBufferInfos.size();
				vk_writeDescriptorSet[0].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				vk_writeDescriptorSet[0].pImageInfo = nullptr;
				vk_writeDescriptorSet[0].pBufferInfo = vk_descriptorBufferInfos.data();
				vk_writeDescriptorSet[0].pTexelBufferView = nullptr;
			}
			vk_writeDescriptorSet[1];
			{
				vk_writeDescriptorSet[1].sType = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				vk_writeDescriptorSet[1].pNext = nullptr;
				vk_writeDescriptorSet[1].dstSet = descriptorSet->GetHandle();
				vk_writeDescriptorSet[1].dstBinding = 1;
				vk_writeDescriptorSet[1].dstArrayElement = 0;
				vk_writeDescriptorSet[1].descriptorCount = vk_descriptorImageInfos.size();
				vk_writeDescriptorSet[1].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				vk_writeDescriptorSet[1].pImageInfo = vk_descriptorImageInfos.data();
				vk_writeDescriptorSet[1].pBufferInfo = nullptr;
				vk_writeDescriptorSet[1].pTexelBufferView = nullptr;
			}
		}

		vkUpdateDescriptorSets(vkDevice->GetHandle(), vk_writeDescriptorSet.size(), vk_writeDescriptorSet.data(), 0, nullptr);
	}

	auto pipelineLayout = new Vulkan::PipelineLayout(vkDevice, {descriptorSetLayout});
	auto pipelineCache = new Vulkan::PipelineCache(vkDevice);
	auto pipeline = new Vulkan::GraphicPipeline(
		vkDevice, pipelineCache, pipelineLayout, renderPass,
		Vulkan::GraphicPipeline::Shaders(
		shaderVertex,
		nullptr,
		nullptr,
		nullptr,
		shaderFragment),
		{Vulkan::GraphicPipeline::Binding(0, geometry->GetVertexSize(), Vulkan::GraphicPipeline::Binding::Rate::VK_VERTEX_INPUT_RATE_VERTEX)},
		{
			Vulkan::GraphicPipeline::Attribute(0, 0, Vulkan::GraphicPipeline::Attribute::Format::VK_FORMAT_R32G32B32_SFLOAT, 0),
			Vulkan::GraphicPipeline::Attribute(1, 0, Vulkan::GraphicPipeline::Attribute::Format::VK_FORMAT_R32G32_SFLOAT, sizeof(Float32)*3*4)},
		{VkViewport{0, 0, (Float32)surface->GetCapabilities().currentExtent.width, (Float32)surface->GetCapabilities().currentExtent.height, 1.0f, 0.0f}},
		{VkRect2D{{0, 0}, {surface->GetCapabilities().currentExtent.width, surface->GetCapabilities().currentExtent.height}}},
		Vulkan::GraphicPipeline::Topology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		Vulkan::GraphicPipeline::Fill::VK_POLYGON_MODE_FILL,
		Vulkan::GraphicPipeline::Culls::VK_CULL_MODE_BACK_BIT,
		Vulkan::GraphicPipeline::Face::VK_FRONT_FACE_COUNTER_CLOCKWISE,
		Vulkan::GraphicPipeline::DepthState(true, true, false, Vulkan::GraphicPipeline::DepthState::Operation::VK_COMPARE_OP_LESS),
		Vulkan::GraphicPipeline::BlendState(
			false, Vulkan::GraphicPipeline::BlendState::Logic::VK_LOGIC_OP_AND,
			Vec4(1.0f, 1.0f, 1.0f, 1.0f),
			{
				Vulkan::GraphicPipeline::BlendState::Attachment(
					true,
					Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_SRC_ALPHA,
					Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ONE,
					Vulkan::GraphicPipeline::BlendState::Operation::VK_BLEND_OP_ADD,
					Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
					Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ZERO,
					Vulkan::GraphicPipeline::BlendState::Operation::VK_BLEND_OP_ADD)
			}
	));


	auto queue = new Vulkan::Queue(vkDevice, 0, 0);
	auto commandPool = new Vulkan::CommandPool(vkDevice, 0);

	// auto t = new Vulkan::CommandBuffer(commandPool, Vulkan::CommandBuffer::Level::Secondary);
	// {
	// 	t->Record(nullptr, 0, nullptr, {
	// 		new Vulkan::Commands::BindGraphicPipeline(pipeline),
	// 	});
	// 
	// 	delete t;
	// }

	auto commandBuffers = [&]()
	{
		Vector<Vulkan::CommandBuffer*> commandBuffers(2);

		for(Size i = 0; i < commandBuffers.size(); ++i)
		{
			auto &commandBuffer = commandBuffers[i];
			auto &framebuffer = framebuffers[i];

			commandBuffer = new Vulkan::CommandBuffer(commandPool, Vulkan::CommandBuffer::Level::Primary);
			commandBuffer->Record(
				renderPass, 0, framebuffer, {
				new Vulkan::Commands::RenderPass(
				renderPass, framebuffer, Vulkan::Commands::RenderPass::Content::VK_SUBPASS_CONTENTS_INLINE,
				{{0, 0}, surface->GetCapabilities().currentExtent}, {
					{1.0f, 0.0f, 0.0f, 1.0f},
					{1.0f, 0}
				}, {
					new Vulkan::Commands::BindGraphicPipeline(pipeline),
					new Vulkan::Commands::BindVertexBuffers({bufferVertices}, {0}),
					new Vulkan::Commands::BindIndexBuffer(bufferIndices, 0, Vulkan::Commands::BindIndexBuffer::Type::VK_INDEX_TYPE_UINT32),
					new Vulkan::Commands::BindGraphicDescriptorSet(pipelineLayout, {descriptorSet}),
					new Vulkan::Commands::DrawIndexed(0, geometry->indices.size(), 0, 1)
				})
			});
		}

		return commandBuffers;
	}();

	auto fence = new Vulkan::Fence(vkDevice);

	while(!GetAsyncKeyState(VK_ESCAPE))
	{
		cout << "------------------------------------------ LOOP ------------------------------------------" << endl;

		window->Loop();

		model.SetAngle(Vec3(0.0f, model.GetAngle().y + 1.0f, 0.0f));

		auto memory = bufferUniforms->GetDeviceMemory();
		{
			auto mat = camera.GetVPMat() * model.GetMMat();

			auto data = memory->Map<Float32>();
			{
				memcpy(data, &mat, sizeof(Float32)* 16);
			}
			memory->Unmap();
		}

		auto swapchainImage = swapchain->AccuireNextImage(fence);

		fence->Wait();
		fence->Reset();

		queue->Submit(commandBuffers[swapchainImage]);
		queue->Wait();

		swapchain->Preset(queue, swapchainImage);
	}

	delete fence;
	for(auto &commandBuffer : commandBuffers) delete commandBuffer;
	delete commandPool;
	delete pipeline;
	delete pipelineLayout;
	delete pipelineCache;
	delete descriptorSet;
	delete descriptorSetLayout;
	delete descriptorPool;
	delete queue;
	for(auto &framebuffer : framebuffers) delete framebuffer;
	for(auto &swapchainImageView : swapchainImageViews) delete swapchainImageView;
	delete shaderVertex; delete shaderFragment;
	delete bufferVertices->GetDeviceMemory(); delete bufferVertices;
	delete bufferIndices->GetDeviceMemory(); delete bufferIndices;
	delete bufferUniforms->GetDeviceMemory(); delete bufferUniforms;
	delete renderPass;
	delete samplerTexture; delete imageViewTexture; delete imageTexture->GetDeviceMemory(); delete imageTexture;
	delete imageViewDepth; delete imageDepth->GetDeviceMemory(); delete imageDepth;
	delete swapchain;
	delete surface;
	delete vkDevice;
	delete vkInstance;
}

void main()
{
	try
	{
		func();
	}
	catch(GVE::Vulkan::Exception e)
	{
		cout << e.GetCode() << ": " << e.GetText() << endl;
	}
	catch(GVE::WinAPI::Exception e)
	{
		cout << e.GetCode() << ": " << e.GetText() << endl;
	}
	catch(GVE::Exception e)
	{
		cout << e.GetText() << endl;
	}

	system("pause");
}