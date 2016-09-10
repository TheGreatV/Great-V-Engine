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
#define GVE_VULKAN_VERSION VK_MAKE_VERSION(1, 0, 21)
#pragma endregion


namespace GreatVEngine
{
	namespace Vulkan
	{
		class Layer
		{
		public:
			using Name = String;
		protected:
			inline Layer() = default;
			inline ~Layer() = default;
		};

		class Instance
		{
		public:
			using Handle = VkInstance;
		protected:
			const Handle handle;
		public:
			/*inline Instance()
			{
				vkCreateInstance()
			}
			inline Instance()
			{
			}*/
		};
	}
}


#pragma region
#pragma endregion
