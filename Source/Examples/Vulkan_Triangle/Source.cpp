#include <iostream>
#include <mutex>
using namespace std;

#include <System/System.hpp>
#include <APIs/Vulkan/Vulkan.hpp>
#include <APIs/WinAPI/Window.hpp>
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

		instance->Subscribe_OnDebug([](String message)
		{
			cout << message << endl;
			Log::Write(message + "\n");
		});

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

	auto renderPass = [&]()
	{
		Vector<VkAttachmentDescription> vk_attachmentDescriptions(1);
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

		Vector<VkAttachmentReference> vk_attachmentReferences(1);
		{
			vk_attachmentReferences[0].attachment = 0;
			vk_attachmentReferences[0].layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}
		Vector<VkSubpassDescription> vk_subpassDescriptions(1);
		{
			vk_subpassDescriptions[0].flags = 0;
			vk_subpassDescriptions[0].pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
			vk_subpassDescriptions[0].inputAttachmentCount = 0;
			vk_subpassDescriptions[0].pInputAttachments = nullptr;
			vk_subpassDescriptions[0].colorAttachmentCount = vk_attachmentReferences.size();
			vk_subpassDescriptions[0].pColorAttachments = vk_attachmentReferences.data();
			vk_subpassDescriptions[0].pResolveAttachments = 0;
			vk_subpassDescriptions[0].pDepthStencilAttachment = nullptr;
			vk_subpassDescriptions[0].preserveAttachmentCount = 0;
			vk_subpassDescriptions[0].pPreserveAttachments = nullptr;
		}

		return new Vulkan::RenderPass(vkDevice, vk_attachmentDescriptions, vk_subpassDescriptions);
	}();

	auto vertexBuffer = new Vulkan::Buffer(
		vkDevice,
		sizeof(Float32)* 2 * 3,
		Vulkan::Buffer::Usages::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		Vulkan::Buffer::Sharing::VK_SHARING_MODE_EXCLUSIVE);
	{
		auto memory = new Vulkan::DeviceMemory(vertexBuffer);
		{
			auto data = memory->Map<Float32>();
			{
				data[0] = -0.5f; data[1] = +0.5f;
				data[2] = +0.5f; data[3] = +0.5f;
				data[4] = +0.0f; data[5] = -0.5f;
			}
			memory->Unmap();
		}
		vertexBuffer->SetDeviceMemory(memory);
	}

	auto sourceShaderVertex = System::LoadFileContent<UInt32>(Filepath("Media/Shaders/Vulkan_Triangle/1.spir-v.vs"));
	auto sourceShaderFragment = System::LoadFileContent<UInt32>(Filepath("Media/Shaders/Vulkan_Triangle/1.spir-v.fs"));

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
				{swapchainImageViews[i]});
		}
	}

	auto pipelineLayout = new Vulkan::PipelineLayout(vkDevice, {});
	auto pipelineCache = new Vulkan::PipelineCache(vkDevice);
	auto pipeline = new Vulkan::GraphicPipeline(
		vkDevice, pipelineCache, pipelineLayout, renderPass,
		Vulkan::GraphicPipeline::Shaders(
			shaderVertex,
			nullptr, 
			nullptr, 
			nullptr, 
			shaderFragment),
		{Vulkan::GraphicPipeline::Binding(0, sizeof(Float32)*2, Vulkan::GraphicPipeline::Binding::Rate::VK_VERTEX_INPUT_RATE_VERTEX)},
		{Vulkan::GraphicPipeline::Attribute(0, 0, Vulkan::GraphicPipeline::Attribute::Format::VK_FORMAT_R32G32_SFLOAT, 0)},
		{VkViewport{0, 0, (Float32)surface->GetCapabilities().currentExtent.width, (Float32)surface->GetCapabilities().currentExtent.height, 0.0f, 1.0f}},
		{VkRect2D{{0, 0}, surface->GetCapabilities().currentExtent}},
		Vulkan::GraphicPipeline::Topology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		Vulkan::GraphicPipeline::Fill::VK_POLYGON_MODE_FILL,
		Vulkan::GraphicPipeline::Culls::VK_CULL_MODE_NONE,
		Vulkan::GraphicPipeline::Face::VK_FRONT_FACE_COUNTER_CLOCKWISE,
		Vulkan::GraphicPipeline::DepthState(false, false, false),
		Vulkan::GraphicPipeline::BlendState(
			false, Vulkan::GraphicPipeline::BlendState::Logic::VK_LOGIC_OP_CLEAR,
			Vec4(1.0f, 1.0f, 1.0f, 1.0f),
			{
				Vulkan::GraphicPipeline::BlendState::Attachment(
					true,
					Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_SRC_ALPHA,
					Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ONE,
					Vulkan::GraphicPipeline::BlendState::Operation::VK_BLEND_OP_ADD,
					Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
					Vulkan::GraphicPipeline::BlendState::Factor::VK_BLEND_FACTOR_ONE,
					Vulkan::GraphicPipeline::BlendState::Operation::VK_BLEND_OP_ADD)
			}
	));

	auto queue = new Vulkan::Queue(vkDevice, 0, 0);
	auto commandPool = new Vulkan::CommandPool(vkDevice, 0);
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
						{1.0f, 0.0f, 0.0f, 1.0f}
					}, {
						new Vulkan::Commands::BindGraphicPipeline(pipeline),
						new Vulkan::Commands::BindVertexBuffers({vertexBuffer}, {0}),
						new Vulkan::Commands::Draw(0, 3, 0, 1)
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
	delete pipelineLayout;
	delete pipelineCache;
	delete pipeline;
	delete queue;
	for(auto &framebuffer : framebuffers) delete framebuffer;
	for(auto &swapchainImageView : swapchainImageViews) delete swapchainImageView;
	delete shaderVertex; delete shaderFragment;
	delete vertexBuffer->GetDeviceMemory(); delete vertexBuffer;
	delete renderPass;
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