#pragma region Include
#pragma once


#include <Header/Header.hpp>


#if GVE_PLATFORM == GVE_PLATFORM_WIN
#include <Windows.h>

#define VK_USE_PLATFORM_WIN32_KHR 1
#endif

#include <vulkan/vulkan.h>
#pragma endregion


#pragma region Macro
#define GVE_VULKAN_VERSION VK_MAKE_VERSION(1, 0, 37)
#pragma endregion


namespace GreatVEngine
{
	namespace Vulkan
	{
	}
}


#pragma region
#pragma endregion
