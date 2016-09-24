#pragma region Include
#pragma once


#include "Graphics.hpp"


#include <APIs/Vulkan/Wrapper.hpp>
#include <Utilities/OpenIL/Functions.hpp>
#pragma endregion


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
		protected:
			Instance* instance;
			PhysicalDevice* physicalDevice;
			Device* device;
			SwapchainKHR* swapchain;
			SurfaceKHR* surface;
			Queue* queue;
			CommandPool* commandPool;
		public:
			inline Engine(HINSTANCE winInstance_, HWND winWindow_);
			inline ~Engine();
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

		Extension::CNames instanceExtensionsNames =
#if GVE_DEBUG
			Instance::GetExtensionsCNames(extensionProperties);
#else
			{};
#endif

		instance = new Instance(instanceLayersNames, instanceExtensionsNames);

#if GVE_DEBUG
		instance->Subscribe_OnDebug([](String message)
		{
			// cout << message << endl;
			// Log::Write(message + "\n");
		});
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
		Extension::CNames deviceExtensionsNames =
#if GVE_DEBUG
			Vulkan::Device::GetExtensionsCNames(extensionProperties);
#else
			{};
#endif

		device = new Device(physicalDevice, deviceLayersNames, deviceExtensionsNames);
	}

	surface = new Vulkan::SurfaceKHR(physicalDevice, winInstance_, winWindow_);
	swapchain = new Vulkan::SwapchainKHR(
		device, surface,
		surface->GetCapabilities().currentExtent,
		surface->GetFormats()[0].format, surface->GetFormats()[0].colorSpace);
	// auto swapchainImages = swapchain->GetImages();

	commandPool = new Vulkan::CommandPool(device, 0);
}
inline GreatVEngine::Vulkan::Engine::~Engine()
{
	delete swapchain;
	delete surface;
	delete device;
	delete instance;
}
#pragma endregion


#pragma region
#pragma endregion

