#pragma region Include
#pragma once


#include "Header.hpp"


#include <Header/ErrorHandling.hpp>


#include <string>
#pragma endregion


namespace GreatVEngine
{
	namespace Vulkan
	{
		class Exception:
			public GreatVEngine::Exception
		{
		public:
			using Code = VkResult;
		protected:
			Code code = VkResult::VK_SUCCESS;
		public:
			inline Exception(const Code& code_);
			inline Exception(const Text& text_);
		public:
			inline Code GetCode() const
			{
				return code;
			}
		};


		inline bool IsSuccessful(const Exception::Code& code_)
		{
			return code_ == Exception::Code::VK_SUCCESS;
		}
		inline Exception::Text GetErrorText(const Exception::Code& code_)
		{
			switch(code_)
			{
				case VK_SUCCESS: return "Success";
				case VK_NOT_READY: return "Not ready";
				case VK_TIMEOUT: return "Timeout";
				case VK_EVENT_SET: return "Event: set";
				case VK_EVENT_RESET: return "Event: reset";
				case VK_INCOMPLETE: return "Incomplete";
				case VK_ERROR_OUT_OF_HOST_MEMORY: return "Error: Out of host memory";
				case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "Error: Out of device memory";
				case VK_ERROR_INITIALIZATION_FAILED: return "Error: Initialization failed";
				case VK_ERROR_DEVICE_LOST: return "Error: Device lost";
				case VK_ERROR_MEMORY_MAP_FAILED: return "Error: Memory map failed";
				case VK_ERROR_LAYER_NOT_PRESENT: return "Error: Layer not present";
				case VK_ERROR_EXTENSION_NOT_PRESENT: return "Error: Extension not present";
				case VK_ERROR_FEATURE_NOT_PRESENT: return "Error: Feature not present";
				case VK_ERROR_INCOMPATIBLE_DRIVER: return "Error: Incompatiatible driver";
				case VK_ERROR_TOO_MANY_OBJECTS: return "Error: Too many objects";
				case VK_ERROR_FORMAT_NOT_SUPPORTED: return "Error: Format not supported";
				case VK_ERROR_SURFACE_LOST_KHR: return "Error KHR: Surface lost";
				case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "Error KHR: Native window in use";
				case VK_SUBOPTIMAL_KHR: return "Error KHR: Suboptimal";
				case VK_ERROR_OUT_OF_DATE_KHR: return "Error KHR: Out of date";
				case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "Error KHR: Incompatiable display";
				case VK_ERROR_VALIDATION_FAILED_EXT: return "Error KHR: Validation failed";
				case VK_ERROR_INVALID_SHADER_NV: return "Error NV: Invalid shader";
			}

			throw Exception("Unknown error code");
		}
		inline void ErrorTest(const Exception::Code& code_) throw(...)
		{
			if(!IsSuccessful(code_))
			{
				throw Exception(code_);
			}
		}
	}
}


#pragma region Exception
inline GreatVEngine::Vulkan::Exception::Exception(const Code& code_):
	GreatVEngine::Exception(GetErrorText(code_)),
	code(code_)
{
}
inline GreatVEngine::Vulkan::Exception::Exception(const Text& text_):
	GreatVEngine::Exception(text_)
{
}
#pragma endregion


#pragma region
#pragma endregion
