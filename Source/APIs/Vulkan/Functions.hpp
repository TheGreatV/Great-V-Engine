#pragma region Include
#pragma once


#include "Header.hpp"
#include "ErrorHandling.hpp"
#pragma endregion


namespace GreatVEngine
{
	namespace Vulkan
	{
		inline Vector<VkLayerProperties>						EnumerateInstanceLayerProperties();
		inline Vector<VkExtensionProperties>					EnumerateInstanceExtensionProperties(const String& layerName_);

		inline VkInstance										CreateInstance(VkInstanceCreateInfo* vk_instanceInfo, const VkAllocationCallbacks* vk_allocator);
		inline void												DestroyInstance(VkInstance vk_instance, const VkAllocationCallbacks* vk_allocator);
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

inline VkInstance												GreatVEngine::Vulkan::CreateInstance(VkInstanceCreateInfo* vk_instanceInfo, const VkAllocationCallbacks* vk_allocator)
{
	VkInstance vk_instance;
	ErrorTest(vkCreateInstance(vk_instanceInfo, vk_allocator, &vk_instance));
	return vk_instance;
}
inline void														GreatVEngine::Vulkan::DestroyInstance(VkInstance vk_instance, const VkAllocationCallbacks* vk_allocator)
{
	vkDestroyInstance(vk_instance, vk_allocator);
}













