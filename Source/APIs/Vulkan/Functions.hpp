#pragma region Include
#pragma once


#include "Header.hpp"
#include "ErrorHandling.hpp"
#pragma endregion


#pragma region Macro
#ifdef CreateSemaphore
#undef CreateSemaphore
#endif
#pragma endregion


namespace GreatVEngine
{
	namespace Vulkan
	{
		inline Vector<VkLayerProperties>						EnumerateInstanceLayerProperties();
		inline Vector<VkExtensionProperties>					EnumerateInstanceExtensionProperties(const String& layerName_);

		inline VkInstance										CreateInstance(VkInstanceCreateInfo* vk_instanceInfo, const VkAllocationCallbacks* vk_allocator);
		inline void												DestroyInstance(VkInstance vk_instance, const VkAllocationCallbacks* vk_allocator);

		inline Vector<VkPhysicalDevice>							EnumeratePhysicalDevices(VkInstance vk_instance);
		inline VkPhysicalDeviceProperties						GetPhysicalDeviceProperties(VkPhysicalDevice vk_physicalDevice);
		inline Vector<VkQueueFamilyProperties>					GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vk_physicalDevice);
		inline VkPhysicalDeviceMemoryProperties					GetPhysicalDeviceMemoryProperties(VkPhysicalDevice vk_physicalDevice);

		inline VkDevice											CreateDevice(VkPhysicalDevice vk_physicalDevice, VkDeviceCreateInfo* vk_deviceCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyDevice(VkDevice vk_device, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DeviceWaitIdle(VkDevice vk_device);

		inline Vector<VkLayerProperties>						EnumerateDeviceLayerProperties(VkPhysicalDevice vk_physicalDevice);
		inline Vector<VkExtensionProperties>					EnumerateDeviceExtensionProperties(VkPhysicalDevice vk_physicalDevice, const String& vk_layerName);

		inline VkQueue											GetDeviceQueue(VkDevice vk_device, uint32_t vk_queueFamilyIndex, uint32_t vk_queueIndex);
		inline void												QueueSubmit(VkQueue vk_queue, Vector<VkSubmitInfo>& vk_submitInfos, VkFence vk_fence);
		inline void												QueueWaitIdle(VkQueue vk_queue);
		inline void												QueuePresentKHR(VkQueue vk_queue, VkPresentInfoKHR* vk_presentInfoKHR);
		
		inline VkCommandPool									CreateCommandPool(VkDevice vk_device, VkCommandPoolCreateInfo* vk_commandPoolCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												ResetCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkCommandPoolResetFlags vk_commandPoolResetFlags);
		
		inline Vector<VkCommandBuffer>							AllocateCommandBuffers(VkDevice vk_device, VkCommandBufferAllocateInfo* vk_CommandBufferAllocateInfo);
		inline void												FreeCommandBuffers(VkDevice vk_device, VkCommandPool vk_commandPool, Vector<VkCommandBuffer>& vk_commandBuffers);
		inline void												ResetCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferResetFlags vk_commandBufferResetFlags);
		inline void												BeginCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferBeginInfo *vk_commandBufferBeginInfo);
		inline void												EndCommandBuffer(VkCommandBuffer vk_commandBuffer);

		inline VkSurfaceKHR										CreateWin32SurfaceKHR(VkInstance vk_instance, VkWin32SurfaceCreateInfoKHR* vk_win32SurfaceCreateInfoKHR, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroySurfaceKHR(VkInstance vk_instance, VkSurfaceKHR vk_surface, VkAllocationCallbacks* vk_allocationCallbacks);
		inline Vector<VkSurfaceFormatKHR>						GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface);
		inline VkSurfaceCapabilitiesKHR							GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface);
		inline Vector<VkPresentModeKHR>							GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface);
		inline bool												GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surfaceKHR, uint32_t queueFamilyIndex);

		inline VkSwapchainKHR									CreateSwapchainKHR(VkDevice vk_device, VkSwapchainCreateInfoKHR* vk_SwapchainCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroySwapchainKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain, VkAllocationCallbacks* vk_allocationCallbacks);
		inline Vector<VkImage>									GetSwapchainImagesKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain);

		inline VkRenderPass										CreateRenderPass(VkDevice vk_device, VkRenderPassCreateInfo* vk_renderPassCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyRenderPass(VkDevice vk_device, VkRenderPass vk_renderPass, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkDeviceMemory									AllocateMemory(VkDevice vk_device, VkMemoryAllocateInfo* vk_memoryAllocateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												FreeMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												BindImageMemory(VkDevice vk_device, VkImage vk_image, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_deviceSize);
		inline void												BindBufferMemory(VkDevice vk_device, VkBuffer vk_buffer, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_memoryOffset);
		inline void*											MapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags vk_memoryMapFlags);
		inline void												UnmapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory);

		inline VkBuffer											CreateBuffer(VkDevice vk_device, VkBufferCreateInfo* vk_bufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyBuffer(VkDevice vk_device, VkBuffer vk_buffer, VkAllocationCallbacks* vk_allocationCallbacks);
		inline VkMemoryRequirements								GetBufferMemoryRequirements(VkDevice vk_device, VkBuffer vk_buffer);

		inline VkImage											CreateImage(VkDevice vk_device, VkImageCreateInfo* vk_imageCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline VkMemoryRequirements								GetImageMemoryRequirements(VkDevice vk_device, VkImage vk_image);
		inline void												DestroyImage(VkDevice vk_device, VkImage vk_image, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkImageView										CreateImageView(VkDevice vk_device, VkImageViewCreateInfo* vk_imageViewCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyImageView(VkDevice vk_device, VkImageView vk_imageView, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkSampler										CreateSampler(VkDevice vk_device, VkSamplerCreateInfo* vk_samplerCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroySampler(VkDevice vk_device, VkSampler vk_sampler, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkFramebuffer									CreateFramebuffer(VkDevice vk_device, VkFramebufferCreateInfo* vk_framebufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyFramebuffer(VkDevice vk_device, VkFramebuffer vk_framebuffer, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkFence											CreateFence(VkDevice vk_device, VkFenceCreateInfo* vk_fenceCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyFence(VkDevice vk_device, VkFence vk_fence, VkAllocationCallbacks* vk_allocationCallbacks);
		inline bool												GetFenceStatus(VkDevice vk_device, VkFence vk_fence);
		inline void												ResetFence(VkDevice vk_device, const Vector<VkFence>& vk_fences);
		inline bool												WaitForFences(VkDevice vk_device, const Vector<VkFence>& vk_fences, bool waitAll, uint64_t timeout = UINT64_MAX);

		inline VkSemaphore										CreateSemaphore(VkDevice vk_device, VkSemaphoreCreateInfo* vk_semaphoreCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroySemaphore(VkDevice vk_device, VkSemaphore vk_semaphore, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkShaderModule									CreateShaderModule(VkDevice vk_device, VkShaderModuleCreateInfo* vk_shaderModuleCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyShaderModule(VkDevice vk_device, VkShaderModule vk_shaderModule, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkPipelineCache									CreatePipelineCache(VkDevice vk_device, VkPipelineCacheCreateInfo* vk_pipelineCacheCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyPipelineCache(VkDevice vk_device, VkPipelineCache vk_pipelineCache, VkAllocationCallbacks* vk_allocationCallbacks);
		inline VkPipelineLayout									CreatePipelineLayout(VkDevice vk_device, VkPipelineLayoutCreateInfo* vk_pipelineLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyPipelineLayout(VkDevice vk_device, VkPipelineLayout vk_pipelineLayout, VkAllocationCallbacks* vk_allocationCallbacks);
		inline GreatVEngine::Vector<VkPipeline>					CreateGraphicsPipelines(VkDevice vk_device, VkPipelineCache vk_pipelineCache, std::vector<VkGraphicsPipelineCreateInfo>& vk_graphicsPipelineCreateInfos, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyPipeline(VkDevice vk_device, VkPipeline vk_pipeline, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkDescriptorSetLayout							CreateDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayoutCreateInfo* vk_descriptorSetLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayout vk_descriptorSetLayout, VkAllocationCallbacks* vk_allocationCallbacks);
		inline VkDescriptorPool									CreateDescriptorPool(VkDevice vk_device, VkDescriptorPoolCreateInfo* vk_descriptorPoolCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyDescriptorPool(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, VkAllocationCallbacks* vk_allocationCallbacks);
		inline Vector<VkDescriptorSet>							AllocateDescriptorSets(VkDevice vk_device, VkDescriptorSetAllocateInfo* vk_descriptorSetAllocateInfo);
		inline void												FreeDescriptorSets(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, const std::vector<VkDescriptorSet>& vk_descriptorSets);


		inline uint32_t											GetCorrectMemoryType(const VkPhysicalDeviceMemoryProperties& vk_physicalDeviceMemoryProperties, uint32_t vk_memoryTypeBits, VkFlags flags);
	}
}


inline GreatVEngine::Vector<VkLayerProperties>					GreatVEngine::Vulkan::EnumerateInstanceLayerProperties()
{
	uint32_t vk_instancelayersPropertiesCount;
	ErrorTest(vkEnumerateInstanceLayerProperties(&vk_instancelayersPropertiesCount, nullptr));

	Vector<VkLayerProperties> vk_instancelayersProperties(vk_instancelayersPropertiesCount);
	ErrorTest(vkEnumerateInstanceLayerProperties(&vk_instancelayersPropertiesCount, vk_instancelayersProperties.data()));

	return std::move(vk_instancelayersProperties);
}
inline GreatVEngine::Vector<VkExtensionProperties>				GreatVEngine::Vulkan::EnumerateInstanceExtensionProperties(const String& layerName_)
{
	uint32_t vk_instanceExtensionPropertiesCount;
	ErrorTest(vkEnumerateInstanceExtensionProperties(layerName_.c_str(), &vk_instanceExtensionPropertiesCount, nullptr));

	Vector<VkExtensionProperties> vk_extensionProperties(vk_instanceExtensionPropertiesCount);
	ErrorTest(vkEnumerateInstanceExtensionProperties(layerName_.c_str(), &vk_instanceExtensionPropertiesCount, vk_extensionProperties.data()));

	return std::move(vk_extensionProperties);
}

inline VkInstance												GreatVEngine::Vulkan::CreateInstance(VkInstanceCreateInfo* vk_instanceCreateInfo, const VkAllocationCallbacks* vk_allocator)
{
	VkInstance vk_instance;
	ErrorTest(vkCreateInstance(vk_instanceCreateInfo, vk_allocator, &vk_instance));
	return vk_instance;
}
inline void														GreatVEngine::Vulkan::DestroyInstance(VkInstance vk_instance, const VkAllocationCallbacks* vk_allocator)
{
	vkDestroyInstance(vk_instance, vk_allocator);
}

inline GreatVEngine::Vector<VkPhysicalDevice>					GreatVEngine::Vulkan::EnumeratePhysicalDevices(VkInstance vk_instance)
{
	uint32_t vk_physicalDevicesCount = 0;
	{
		ErrorTest(vkEnumeratePhysicalDevices(vk_instance, &vk_physicalDevicesCount, nullptr));
	}

	Vector<VkPhysicalDevice> vk_physicalDevices(vk_physicalDevicesCount);

	ErrorTest(vkEnumeratePhysicalDevices(vk_instance, &vk_physicalDevicesCount, vk_physicalDevices.data()));

	return std::move(vk_physicalDevices);
}
inline VkPhysicalDeviceProperties								GreatVEngine::Vulkan::GetPhysicalDeviceProperties(VkPhysicalDevice vk_physicalDevice)
{
	VkPhysicalDeviceProperties vk_physicalDeviceProperties;

	vkGetPhysicalDeviceProperties(vk_physicalDevice, &vk_physicalDeviceProperties);

	return vk_physicalDeviceProperties;
}
inline GreatVEngine::Vector<VkQueueFamilyProperties>			GreatVEngine::Vulkan::GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vk_physicalDevice)
{
	uint32_t vk_queueFamilyPropertiesCount;
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice, &vk_queueFamilyPropertiesCount, nullptr);

	Vector<VkQueueFamilyProperties> vk_queueFamilyProperties(vk_queueFamilyPropertiesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice, &vk_queueFamilyPropertiesCount, vk_queueFamilyProperties.data());

	return std::move(vk_queueFamilyProperties);
}
inline VkPhysicalDeviceMemoryProperties							GreatVEngine::Vulkan::GetPhysicalDeviceMemoryProperties(VkPhysicalDevice vk_physicalDevice)
{
	VkPhysicalDeviceMemoryProperties vk_physicalDeviceMemoryProperties;

	vkGetPhysicalDeviceMemoryProperties(vk_physicalDevice, &vk_physicalDeviceMemoryProperties);

	return vk_physicalDeviceMemoryProperties;
}

inline VkDevice													GreatVEngine::Vulkan::CreateDevice(VkPhysicalDevice vk_physicalDevice, VkDeviceCreateInfo* vk_deviceCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDevice vk_device;

	ErrorTest(vkCreateDevice(vk_physicalDevice, vk_deviceCreateInfo, vk_allocationCallbacks, &vk_device));

	return vk_device;
}
inline void														GreatVEngine::Vulkan::DestroyDevice(VkDevice vk_device, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyDevice(vk_device, vk_allocationCallbacks);
}
inline void														GreatVEngine::Vulkan::DeviceWaitIdle(VkDevice vk_device)
{
	ErrorTest(vkDeviceWaitIdle(vk_device));
}

inline GreatVEngine::Vector<VkLayerProperties>					GreatVEngine::Vulkan::EnumerateDeviceLayerProperties(VkPhysicalDevice vk_physicalDevice)
{
	uint32_t vk_layersPropertiesCount;
	ErrorTest(vkEnumerateDeviceLayerProperties(vk_physicalDevice, &vk_layersPropertiesCount, nullptr));

	Vector<VkLayerProperties> vk_layersProperties(vk_layersPropertiesCount);
	ErrorTest(vkEnumerateDeviceLayerProperties(vk_physicalDevice, &vk_layersPropertiesCount, vk_layersProperties.data()));

	return std::move(vk_layersProperties);
}
inline GreatVEngine::Vector<VkExtensionProperties>				GreatVEngine::Vulkan::EnumerateDeviceExtensionProperties(VkPhysicalDevice vk_physicalDevice, const String& vk_layerName)
{
	uint32_t vk_deviceExtensionPropertiesCount;
	ErrorTest(vkEnumerateDeviceExtensionProperties(vk_physicalDevice, vk_layerName.c_str(), &vk_deviceExtensionPropertiesCount, nullptr));

	Vector<VkExtensionProperties> vk_extensionProperties(vk_deviceExtensionPropertiesCount);
	ErrorTest(vkEnumerateDeviceExtensionProperties(vk_physicalDevice, vk_layerName.c_str(), &vk_deviceExtensionPropertiesCount, vk_extensionProperties.data()));

	return std::move(vk_extensionProperties);
}

inline VkQueue													GreatVEngine::Vulkan::GetDeviceQueue(VkDevice vk_device, uint32_t vk_queueFamilyIndex, uint32_t vk_queueIndex)
{
	VkQueue vk_queue = 0;

	vkGetDeviceQueue(
		vk_device,
		vk_queueFamilyIndex, // less than vk_deviceCreateInfo.queueCreateInfoCount
		vk_queueIndex, // less than vk_deviceQueueCreateInfos[i].queueCount
		&vk_queue);

	return vk_queue;
}
inline void														GreatVEngine::Vulkan::QueueSubmit(VkQueue vk_queue, Vector<VkSubmitInfo>& vk_submitInfos, VkFence vk_fence)
{
	ErrorTest(vkQueueSubmit(vk_queue, vk_submitInfos.size(), vk_submitInfos.data(), vk_fence));
}
inline void														GreatVEngine::Vulkan::QueueWaitIdle(VkQueue vk_queue)
{
	ErrorTest(vkQueueWaitIdle(vk_queue));
}
inline void														GreatVEngine::Vulkan::QueuePresentKHR(VkQueue vk_queue, VkPresentInfoKHR* vk_presentInfoKHR)
{
	ErrorTest(vkQueuePresentKHR(vk_queue, vk_presentInfoKHR));
}

inline VkCommandPool											GreatVEngine::Vulkan::CreateCommandPool(VkDevice vk_device, VkCommandPoolCreateInfo* vk_commandPoolCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkCommandPool vk_commandPool;

	ErrorTest(vkCreateCommandPool(vk_device, vk_commandPoolCreateInfo, vk_allocationCallbacks, &vk_commandPool));

	return vk_commandPool;
}
inline void														GreatVEngine::Vulkan::DestroyCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyCommandPool(vk_device, vk_commandPool, vk_allocationCallbacks);
}
inline void														GreatVEngine::Vulkan::ResetCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkCommandPoolResetFlags vk_commandPoolResetFlags)
{
	ErrorTest(vkResetCommandPool(vk_device, vk_commandPool, vk_commandPoolResetFlags));
}

inline GreatVEngine::Vector<VkCommandBuffer>					GreatVEngine::Vulkan::AllocateCommandBuffers(VkDevice vk_device, VkCommandBufferAllocateInfo* vk_CommandBufferAllocateInfo)
{
	Vector<VkCommandBuffer> vk_commandBuffers(vk_CommandBufferAllocateInfo->commandBufferCount);

	ErrorTest(vkAllocateCommandBuffers(vk_device, vk_CommandBufferAllocateInfo, vk_commandBuffers.data()));

	return std::move(vk_commandBuffers);
}
inline void														GreatVEngine::Vulkan::FreeCommandBuffers(VkDevice vk_device, VkCommandPool vk_commandPool, Vector<VkCommandBuffer>& vk_commandBuffers)
{
	vkFreeCommandBuffers(vk_device, vk_commandPool, vk_commandBuffers.size(), vk_commandBuffers.data());
}
inline void														GreatVEngine::Vulkan::ResetCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferResetFlags vk_commandBufferResetFlags)
{
	ErrorTest(vkResetCommandBuffer(vk_commandBuffer, vk_commandBufferResetFlags));
}
inline void														GreatVEngine::Vulkan::BeginCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferBeginInfo *vk_commandBufferBeginInfo)
{
	ErrorTest(vkBeginCommandBuffer(vk_commandBuffer, vk_commandBufferBeginInfo));
}
inline void														GreatVEngine::Vulkan::EndCommandBuffer(VkCommandBuffer vk_commandBuffer)
{
	ErrorTest(vkEndCommandBuffer(vk_commandBuffer));
}

inline VkSurfaceKHR												GreatVEngine::Vulkan::CreateWin32SurfaceKHR(VkInstance vk_instance, VkWin32SurfaceCreateInfoKHR* vk_win32SurfaceCreateInfoKHR, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkSurfaceKHR vk_surfaceKHR;

	ErrorTest(vkCreateWin32SurfaceKHR(vk_instance, vk_win32SurfaceCreateInfoKHR, vk_allocationCallbacks, &vk_surfaceKHR));

	return vk_surfaceKHR;
}
inline void														GreatVEngine::Vulkan::DestroySurfaceKHR(VkInstance vk_instance, VkSurfaceKHR vk_surface, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroySurfaceKHR(vk_instance, vk_surface, vk_allocationCallbacks);
}
inline GreatVEngine::Vector<VkSurfaceFormatKHR>					GreatVEngine::Vulkan::GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface)
{
	uint32_t vk_surfaceFormatsCount;
	ErrorTest(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface, &vk_surfaceFormatsCount, nullptr));

	Vector<VkSurfaceFormatKHR> vk_surfaceFormats(vk_surfaceFormatsCount);
	ErrorTest(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface, &vk_surfaceFormatsCount, vk_surfaceFormats.data()));

	return std::move(vk_surfaceFormats);
}
inline VkSurfaceCapabilitiesKHR									GreatVEngine::Vulkan::GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface)
{
	VkSurfaceCapabilitiesKHR vk_surfaceCapabilities;

	ErrorTest(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicalDevice, vk_surface, &vk_surfaceCapabilities));

	return vk_surfaceCapabilities;
}
inline GreatVEngine::Vector<VkPresentModeKHR>					GreatVEngine::Vulkan::GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface)
{
	uint32_t vk_physicalDeviceSurfacePresentModesCount;
	ErrorTest(vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physicalDevice, vk_surface, &vk_physicalDeviceSurfacePresentModesCount, nullptr));

	Vector<VkPresentModeKHR> vk_physicalDeviceSurfacePresentModes(vk_physicalDeviceSurfacePresentModesCount);
	ErrorTest(vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physicalDevice, vk_surface, &vk_physicalDeviceSurfacePresentModesCount, vk_physicalDeviceSurfacePresentModes.data()));

	return std::move(vk_physicalDeviceSurfacePresentModes);
}
inline bool														GreatVEngine::Vulkan::GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surfaceKHR, uint32_t queueFamilyIndex)
{
	VkBool32 isSupported;

	ErrorTest(vkGetPhysicalDeviceSurfaceSupportKHR(vk_physicalDevice, queueFamilyIndex, vk_surfaceKHR, &isSupported));

	return isSupported == VK_TRUE;
}

inline VkSwapchainKHR											GreatVEngine::Vulkan::CreateSwapchainKHR(VkDevice vk_device, VkSwapchainCreateInfoKHR* vk_SwapchainCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkSwapchainKHR vk_swapchain;

	ErrorTest(vkCreateSwapchainKHR(vk_device, vk_SwapchainCreateInfo, vk_allocationCallbacks, &vk_swapchain));

	return vk_swapchain;
}
inline void														GreatVEngine::Vulkan::DestroySwapchainKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroySwapchainKHR(vk_device, vk_swapchain, vk_allocationCallbacks);
}
inline GreatVEngine::Vector<VkImage>							GreatVEngine::Vulkan::GetSwapchainImagesKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain)
{
	uint32_t vk_imagesCount;
	ErrorTest(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_imagesCount, nullptr));

	Vector<VkImage> vk_images(vk_imagesCount);
	ErrorTest(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_imagesCount, vk_images.data()));

	return std::move(vk_images);
}

inline VkDeviceMemory											GreatVEngine::Vulkan::AllocateMemory(VkDevice vk_device, VkMemoryAllocateInfo* vk_memoryAllocateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDeviceMemory vk_deviceMemory;

	ErrorTest(vkAllocateMemory(vk_device, vk_memoryAllocateInfo, vk_allocationCallbacks, &vk_deviceMemory));

	return vk_deviceMemory;
}
inline void														GreatVEngine::Vulkan::FreeMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkFreeMemory(vk_device, vk_deviceMemory, vk_allocationCallbacks);
}
inline void														GreatVEngine::Vulkan::BindImageMemory(VkDevice vk_device, VkImage vk_image, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_deviceSize)
{
	ErrorTest(vkBindImageMemory(vk_device, vk_image, vk_deviceMemory, vk_deviceSize));
}
inline void														GreatVEngine::Vulkan::BindBufferMemory(VkDevice vk_device, VkBuffer vk_buffer, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_memoryOffset)
{
	ErrorTest(vkBindBufferMemory(vk_device, vk_buffer, vk_deviceMemory, vk_memoryOffset));
}
inline void*													GreatVEngine::Vulkan::MapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags vk_memoryMapFlags)
{
	void* data;

	ErrorTest(vkMapMemory(vk_device, vk_deviceMemory, offset, size, vk_memoryMapFlags, &data));

	return data;
}
inline void														GreatVEngine::Vulkan::UnmapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory)
{
	vkUnmapMemory(vk_device, vk_deviceMemory);
}

inline VkBuffer													GreatVEngine::Vulkan::CreateBuffer(VkDevice vk_device, VkBufferCreateInfo* vk_bufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkBuffer vk_buffer;

	ErrorTest(vkCreateBuffer(vk_device, vk_bufferCreateInfo, vk_allocationCallbacks, &vk_buffer));

	return vk_buffer;
}
inline void														GreatVEngine::Vulkan::DestroyBuffer(VkDevice vk_device, VkBuffer vk_buffer, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyBuffer(vk_device, vk_buffer, vk_allocationCallbacks);
}
inline VkMemoryRequirements										GreatVEngine::Vulkan::GetBufferMemoryRequirements(VkDevice vk_device, VkBuffer vk_buffer)
{
	VkMemoryRequirements vk_memoryRequirements;

	vkGetBufferMemoryRequirements(vk_device, vk_buffer, &vk_memoryRequirements);

	return vk_memoryRequirements;
}

inline VkRenderPass												GreatVEngine::Vulkan::CreateRenderPass(VkDevice vk_device, VkRenderPassCreateInfo* vk_renderPassCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkRenderPass vk_renderPass;

	ErrorTest(vkCreateRenderPass(vk_device, vk_renderPassCreateInfo, vk_allocationCallbacks, &vk_renderPass));

	return vk_renderPass;
}
inline void														GreatVEngine::Vulkan::DestroyRenderPass(VkDevice vk_device, VkRenderPass vk_renderPass, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyRenderPass(vk_device, vk_renderPass, vk_allocationCallbacks);
}

inline VkImage													GreatVEngine::Vulkan::CreateImage(VkDevice vk_device, VkImageCreateInfo* vk_imageCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkImage vk_image;

	ErrorTest(vkCreateImage(vk_device, vk_imageCreateInfo, vk_allocationCallbacks, &vk_image));

	return vk_image;
}
inline VkMemoryRequirements										GreatVEngine::Vulkan::GetImageMemoryRequirements(VkDevice vk_device, VkImage vk_image)
{
	VkMemoryRequirements vk_memoryRequirements;

	vkGetImageMemoryRequirements(vk_device, vk_image, &vk_memoryRequirements);

	return vk_memoryRequirements;
}
inline void														GreatVEngine::Vulkan::DestroyImage(VkDevice vk_device, VkImage vk_image, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyImage(vk_device, vk_image, vk_allocationCallbacks);
}

inline VkImageView												GreatVEngine::Vulkan::CreateImageView(VkDevice vk_device, VkImageViewCreateInfo* vk_imageViewCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkImageView vk_imageView;

	ErrorTest(vkCreateImageView(vk_device, vk_imageViewCreateInfo, vk_allocationCallbacks, &vk_imageView));

	return vk_imageView;
}
inline void														GreatVEngine::Vulkan::DestroyImageView(VkDevice vk_device, VkImageView vk_imageView, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyImageView(vk_device, vk_imageView, vk_allocationCallbacks);
}

inline VkSampler												GreatVEngine::Vulkan::CreateSampler(VkDevice vk_device, VkSamplerCreateInfo* vk_samplerCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkSampler vk_sampler;

	ErrorTest(vkCreateSampler(vk_device, vk_samplerCreateInfo, vk_allocationCallbacks, &vk_sampler));

	return vk_sampler;
}
inline void														GreatVEngine::Vulkan::DestroySampler(VkDevice vk_device, VkSampler vk_sampler, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroySampler(vk_device, vk_sampler, vk_allocationCallbacks);
}

inline VkFramebuffer											GreatVEngine::Vulkan::CreateFramebuffer(VkDevice vk_device, VkFramebufferCreateInfo* vk_framebufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkFramebuffer vk_framebuffer;

	ErrorTest(vkCreateFramebuffer(vk_device, vk_framebufferCreateInfo, vk_allocationCallbacks, &vk_framebuffer));

	return vk_framebuffer;
}
inline void														GreatVEngine::Vulkan::DestroyFramebuffer(VkDevice vk_device, VkFramebuffer vk_framebuffer, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyFramebuffer(vk_device, vk_framebuffer, vk_allocationCallbacks);
}

inline VkFence													GreatVEngine::Vulkan::CreateFence(VkDevice vk_device, VkFenceCreateInfo* vk_fenceCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkFence vk_fence;

	ErrorTest(vkCreateFence(vk_device, vk_fenceCreateInfo, vk_allocationCallbacks, &vk_fence));

	return vk_fence;
}
inline void														GreatVEngine::Vulkan::DestroyFence(VkDevice vk_device, VkFence vk_fence, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyFence(vk_device, vk_fence, vk_allocationCallbacks);
}
inline bool														GreatVEngine::Vulkan::GetFenceStatus(VkDevice vk_device, VkFence vk_fence)
{
	ErrorTest(vkGetFenceStatus(vk_device, vk_fence));
}
inline void														GreatVEngine::Vulkan::ResetFence(VkDevice vk_device, const Vector<VkFence>& vk_fences)
{
	ErrorTest(vkResetFences(vk_device, vk_fences.size(), vk_fences.data()));
}
inline bool														GreatVEngine::Vulkan::WaitForFences(VkDevice vk_device, const Vector<VkFence>& vk_fences, bool waitAll, uint64_t timeout)
{
	try
	{
		ErrorTest(vkWaitForFences(vk_device, vk_fences.size(), vk_fences.data(), waitAll ? VK_TRUE : VK_FALSE, timeout));
	}
	catch(Exception e)
	{
		if(e.GetCode() == VkResult::VK_TIMEOUT)
		{
			return false;
		}

		throw e;
	}

	return true;
}

inline VkSemaphore												GreatVEngine::Vulkan::CreateSemaphore(VkDevice vk_device, VkSemaphoreCreateInfo* vk_semaphoreCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkSemaphore vk_semaphore;
	
	ErrorTest(vkCreateSemaphore(vk_device, vk_semaphoreCreateInfo, vk_allocationCallbacks, &vk_semaphore));

	return vk_semaphore;
}
inline void														GreatVEngine::Vulkan::DestroySemaphore(VkDevice vk_device, VkSemaphore vk_semaphore, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroySemaphore(vk_device, vk_semaphore, vk_allocationCallbacks);
}

inline VkShaderModule											GreatVEngine::Vulkan::CreateShaderModule(VkDevice vk_device, VkShaderModuleCreateInfo* vk_shaderModuleCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkShaderModule vk_ShaderModule;

	ErrorTest(vkCreateShaderModule(vk_device, vk_shaderModuleCreateInfo, vk_allocationCallbacks, &vk_ShaderModule));

	return vk_ShaderModule;
}
inline void														GreatVEngine::Vulkan::DestroyShaderModule(VkDevice vk_device, VkShaderModule vk_shaderModule, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyShaderModule(vk_device, vk_shaderModule, vk_allocationCallbacks);
}

inline VkPipelineCache											GreatVEngine::Vulkan::CreatePipelineCache(VkDevice vk_device, VkPipelineCacheCreateInfo* vk_pipelineCacheCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkPipelineCache vk_pipelineCache;

	ErrorTest(vkCreatePipelineCache(vk_device, vk_pipelineCacheCreateInfo, vk_allocationCallbacks, &vk_pipelineCache));

	return vk_pipelineCache;
}
inline void														GreatVEngine::Vulkan::DestroyPipelineCache(VkDevice vk_device, VkPipelineCache vk_pipelineCache, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyPipelineCache(vk_device, vk_pipelineCache, vk_allocationCallbacks);
}

inline VkPipelineLayout											GreatVEngine::Vulkan::CreatePipelineLayout(VkDevice vk_device, VkPipelineLayoutCreateInfo* vk_pipelineLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkPipelineLayout vk_pipelineLayout;

	ErrorTest(vkCreatePipelineLayout(vk_device, vk_pipelineLayoutCreateInfo, vk_allocationCallbacks, &vk_pipelineLayout));

	return vk_pipelineLayout;
}
inline void														GreatVEngine::Vulkan::DestroyPipelineLayout(VkDevice vk_device, VkPipelineLayout vk_pipelineLayout, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyPipelineLayout(vk_device, vk_pipelineLayout, vk_allocationCallbacks);
}

inline GreatVEngine::Vector<VkPipeline>							GreatVEngine::Vulkan::CreateGraphicsPipelines(VkDevice vk_device, VkPipelineCache vk_pipelineCache, Vector<VkGraphicsPipelineCreateInfo>& vk_graphicsPipelineCreateInfos, VkAllocationCallbacks* vk_allocationCallbacks)
{
	Vector<VkPipeline> vk_pipelines(vk_graphicsPipelineCreateInfos.size());

	ErrorTest(vkCreateGraphicsPipelines(vk_device, vk_pipelineCache, vk_graphicsPipelineCreateInfos.size(), vk_graphicsPipelineCreateInfos.data(), vk_allocationCallbacks, vk_pipelines.data()));

	return std::move(vk_pipelines);
}
inline void														GreatVEngine::Vulkan::DestroyPipeline(VkDevice vk_device, VkPipeline vk_pipeline, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyPipeline(vk_device, vk_pipeline, vk_allocationCallbacks);
}

inline VkDescriptorSetLayout									GreatVEngine::Vulkan::CreateDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayoutCreateInfo* vk_descriptorSetLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDescriptorSetLayout vk_descriptorSetLayout;

	ErrorTest(vkCreateDescriptorSetLayout(vk_device, vk_descriptorSetLayoutCreateInfo, vk_allocationCallbacks, &vk_descriptorSetLayout));

	return vk_descriptorSetLayout;
}
inline void														GreatVEngine::Vulkan::DestroyDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayout vk_descriptorSetLayout, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyDescriptorSetLayout(vk_device, vk_descriptorSetLayout, vk_allocationCallbacks);
}

inline VkDescriptorPool											GreatVEngine::Vulkan::CreateDescriptorPool(VkDevice vk_device, VkDescriptorPoolCreateInfo* vk_descriptorPoolCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDescriptorPool vk_descriptorPool;

	ErrorTest(vkCreateDescriptorPool(vk_device, vk_descriptorPoolCreateInfo, vk_allocationCallbacks, &vk_descriptorPool));

	return vk_descriptorPool;
}
inline void														GreatVEngine::Vulkan::DestroyDescriptorPool(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyDescriptorPool(vk_device, vk_descriptorPool, vk_allocationCallbacks);
}

inline GreatVEngine::Vector<VkDescriptorSet>					GreatVEngine::Vulkan::AllocateDescriptorSets(VkDevice vk_device, VkDescriptorSetAllocateInfo* vk_descriptorSetAllocateInfo)
{
	Vector<VkDescriptorSet> vk_descriptorSets(vk_descriptorSetAllocateInfo->descriptorSetCount);

	ErrorTest(vkAllocateDescriptorSets(vk_device, vk_descriptorSetAllocateInfo, vk_descriptorSets.data()));

	return std::move(vk_descriptorSets);
}
inline void														GreatVEngine::Vulkan::FreeDescriptorSets(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, const std::vector<VkDescriptorSet>& vk_descriptorSets)
{
	vkFreeDescriptorSets(vk_device, vk_descriptorPool, vk_descriptorSets.size(), vk_descriptorSets.data());
}


inline uint32_t													GreatVEngine::Vulkan::GetCorrectMemoryType(const VkPhysicalDeviceMemoryProperties& vk_physicalDeviceMemoryProperties, uint32_t vk_memoryTypeBits, VkFlags flags)
{
	// returns VkMemoryAllocateInfo.memoryTypeIndex

	//for(size_t i = 0; i < VK_MAX_MEMORY_TYPES; ++i)
	for(size_t i = 0; i < vk_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
	{
		auto bit = ((uint32_t)1 << i);
		if((vk_memoryTypeBits & bit) != 0) // check i-th bit
		{
			if((vk_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & flags) == flags)
			{
				return i;
			}
		}
	}

	throw Exception("Failed to get correct memory type");
}
