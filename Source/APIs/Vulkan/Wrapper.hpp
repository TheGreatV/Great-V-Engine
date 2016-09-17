#pragma region Include
#pragma once


#include "Header.hpp"
#include "ErrorHandling.hpp"
#include "Functions.hpp"


#include <Logic/Helpers.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace Vulkan
	{
		class Instance;
		class PhysicalDevice;
		class Device;
		class Buffer;
		class BasicImage;
		class Image;
		class SurfaceKHR;
		class SwapchainKHR;
		class ImageView;
		class RenderPass;
		class Framebuffer;
		class Queue;
		class CommandPool;
		class CommandBuffer;
		class GraphicPipeline;
		class Fence;
		class Semaphore;

		namespace Layer
		{
			using Name = String;
			using CName = const char*;

			class Property
			{
			public:
				const Name name;
				const String description;
				const uint32_t implementationVersion;
				const uint32_t specVersion;
			public:
				inline explicit Property(const VkLayerProperties& vk_layerProperties_):
					name(vk_layerProperties_.layerName, strlen(vk_layerProperties_.layerName)),
					description(vk_layerProperties_.description, strlen(vk_layerProperties_.description)),
					implementationVersion(vk_layerProperties_.implementationVersion),
					specVersion(vk_layerProperties_.specVersion)
				{
				}
				inline Property(const Property&) = default;
			};

			using Names = Vector<Name>;
			using CNames = Vector<CName>;
			using Properties = Vector<Property>;
		}
		namespace Extension
		{
			using Name = String;
			using CName = const char*;

			class Property
			{
			public:
				const Name name;
				const uint32_t specVersion;
			public:
				inline explicit Property(const VkExtensionProperties& vk_extensionProperties_):
					name(vk_extensionProperties_.extensionName),
					specVersion(vk_extensionProperties_.specVersion)
				{
				}
				inline Property(const Property&) = default;
			};

			using Names = Vector<Name>;
			using CNames = Vector<CName>;
			using Properties = Vector<Property>;
		}
		class Instance
		{
		public:
			class Dependent
			{
			protected:
				Instance*const instance;
			public:
				inline Dependent(Instance* instance_):
					instance(instance_)
				{
				}
				inline Dependent(const Dependent&) = default;
			public:
				inline Instance* GetInstance() const
				{
					return instance;
				}
			};
		public:
			using Handle = VkInstance;
		public:
			inline static Layer::Properties EnumerateLayersProperties()
			{
				auto vk_layersProperties = EnumerateInstanceLayerProperties();

				Layer::Properties layerProperties;
				{
					layerProperties.reserve(vk_layersProperties.size());
				}

				for(auto &vk_layerProperties : vk_layersProperties)
				{
					layerProperties.push_back(Layer::Property(vk_layerProperties));
				}

				return std::move(layerProperties);
			}
			inline static Dictionary<Layer::Name, Extension::Properties> EnumerateExtensionsProperties(const Layer::Properties& layersProperties_)
			{
				Dictionary<Layer::Name, Extension::Properties> extensionsProperties;

				for(auto &layerProperties : layersProperties_)
				{
					auto &extensionProperties = extensionsProperties[layerProperties.name];

					auto vk_extensionProperties = EnumerateInstanceExtensionProperties(layerProperties.name);

					for(auto vk_extensionProperty : vk_extensionProperties)
					{
						extensionProperties.push_back(Extension::Property(vk_extensionProperty));
					}
				}


				auto &extensionProperties = extensionsProperties[""];

				auto vk_extensionProperties = EnumerateInstanceExtensionProperties("");

				for(auto vk_extensionProperty : vk_extensionProperties)
				{
					extensionProperties.push_back(Extension::Property(vk_extensionProperty));
				}

				return std::move(extensionsProperties);
			}
			inline static Layer::CNames GetLayersCNames(const Layer::Properties& layersProperties_, const Initializer<String> filters_ = {})
			{
				Layer::CNames layersNames;
				{
					for(auto &layerProperties : layersProperties_)
					{
						if([&](){
							for(auto &filter : filters_)
							{
								if(layerProperties.name == filter)
								{
									return false;
								}
							}

							return true;
						}())
						{
							layersNames.push_back(layerProperties.name.c_str());
						}
					}
				}

				return std::move(layersNames);
			}
			inline static Extension::CNames GetExtensionsCNames(const Dictionary<Layer::Name, Extension::Properties>& extensionsProperties_)
			{
				Extension::CNames extensionsNames;
				{
					for(auto &extensionProperties : extensionsProperties_)
					{
						for(auto &extensionProperty : extensionProperties.second)
						{
							extensionsNames.push_back(extensionProperty.name.c_str());
						}
					}
				}

				return std::move(extensionsNames);
			}
		protected:
			static VkBool32 __stdcall DebugCallback(
				VkDebugReportFlagsEXT                       flags,
				VkDebugReportObjectTypeEXT                  objectType,
				uint64_t                                    object,
				size_t                                      location,
				int32_t                                     messageCode,
				const char*                                 pLayerPrefix,
				const char*                                 pMessage,
				void*                                       pUserData);
		protected:
			const Handle handle;
			const Vector<PhysicalDevice*> physicalDevices;
			Helper::Subscrption::OnEvent<String> onDebug;
#if GVE_DEBUG
			VkDebugReportCallbackEXT vk_debugReportCallbackEXT;
#endif
		public:
			inline Instance(const Layer::CNames& layersCNames_, const Extension::CNames& extensionsCNames_);
			inline Instance(const Instance&) = delete;
			inline ~Instance();
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Vector<PhysicalDevice*> GetPhysicalDevices() const
			{
				return physicalDevices;
			}
		public:
			inline void Subscribe_OnDebug(Helper::Subscrption::OnEvent<String>::Subscriber subscriber_)
			{
				onDebug += subscriber_;
			}
			inline void Unsubscribe_OnDebug(Helper::Subscrption::OnEvent<String>::Subscriber subscriber_)
			{
				onDebug -= subscriber_;
			}
		};
		class PhysicalDevice:
			public Instance::Dependent
		{
		public:
			class Dependent
			{
			protected:
				PhysicalDevice*const physicalDevice;
			public:
				inline Dependent(PhysicalDevice* physicalDevice_):
					physicalDevice(physicalDevice_)
				{
				}
				inline Dependent(const Dependent&) = default;
			public:
				inline PhysicalDevice* GetPhysicalDevice() const
				{
					return physicalDevice;
				}
			};
		public:
			using Handle = VkPhysicalDevice;
			using Properties = VkPhysicalDeviceProperties;
			using MemoryProperties = VkPhysicalDeviceMemoryProperties;
			using QueueFamilyProperties = VkQueueFamilyProperties;
			using MemoryProperties = VkPhysicalDeviceMemoryProperties;
		public:
			inline static Vector<PhysicalDevice*> Enumerate(Instance* instance_);
		protected:
			const Handle handle;
			const Properties properties;
			const Vector<QueueFamilyProperties> queueFamilyProperties;
			const MemoryProperties memoryProperties;
		protected:
			inline PhysicalDevice(Instance* instance_, Handle handle_):
				Instance::Dependent(instance_),
				handle(handle_),
				properties(GetPhysicalDeviceProperties(handle)),
				queueFamilyProperties(GetPhysicalDeviceQueueFamilyProperties(handle)),
				memoryProperties(GetPhysicalDeviceMemoryProperties(handle))
			{
			}
		public:
			inline PhysicalDevice(const PhysicalDevice&) = default;
			inline ~PhysicalDevice() = default;
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Properties GetProperties() const
			{
				return properties;
			}
			inline Vector<QueueFamilyProperties> GetQueueFamilyProperties() const
			{
				return queueFamilyProperties;
			}
			inline MemoryProperties GetMemoryProperties() const
			{
				return memoryProperties;
			}
		};
		class Device:
			public PhysicalDevice::Dependent
		{
		public:
			class Dependent
			{
			protected:
				Device*const device;
			public:
				inline Dependent(Device* device_):
					device(device_)
				{
				}
				inline Dependent(const Dependent&) = default;
			public:
				inline Device* GetDevice() const
				{
					return device;
				}
			};
		public:
			using Handle = VkDevice;
		protected:
			const Handle handle;
		public:
			inline static Layer::Properties EnumerateLayersProperties(PhysicalDevice* physicalDevice_)
			{
				auto vk_layersProperties = EnumerateDeviceLayerProperties(physicalDevice_->GetHandle());

				Layer::Properties layerProperties;
				{
					layerProperties.reserve(vk_layersProperties.size());
				}

				for(auto &vk_layerProperties : vk_layersProperties)
				{
					layerProperties.push_back(Layer::Property(vk_layerProperties));
				}

				return std::move(layerProperties);
			}
			inline static Dictionary<Layer::Name, Extension::Properties> EnumerateExtensionsProperties(PhysicalDevice* physicalDevice_, const Layer::Properties& layersProperties_)
			{
				Dictionary<Layer::Name, Extension::Properties> extensionsProperties;

				for(auto &layerProperties : layersProperties_)
				{
					auto &extensionProperties = extensionsProperties[layerProperties.name];

					auto vk_extensionProperties = EnumerateDeviceExtensionProperties(physicalDevice_->GetHandle(), layerProperties.name);

					for(auto vk_extensionProperty : vk_extensionProperties)
					{
						extensionProperties.push_back(Extension::Property(vk_extensionProperty));
					}
				}


				auto &extensionProperties = extensionsProperties[""];

				auto vk_extensionProperties = EnumerateDeviceExtensionProperties(physicalDevice_->GetHandle(), "");

				for(auto vk_extensionProperty : vk_extensionProperties)
				{
					extensionProperties.push_back(Extension::Property(vk_extensionProperty));
				}

				return std::move(extensionsProperties);
			}
			inline static Layer::CNames GetLayersCNames(const Layer::Properties& layersProperties_, const Initializer<String> filters_ = {})
			{
				Layer::CNames layersNames;
				{
					for(auto &layerProperties : layersProperties_)
					{
						if([&](){
							for(auto &filter : filters_)
							{
								if(layerProperties.name == filter)
								{
									return false;
								}
							}

							return true;
						}())
						{
							layersNames.push_back(layerProperties.name.c_str());
						}
					}
				}

				return std::move(layersNames);
			}
			inline static Extension::CNames GetExtensionsCNames(const Dictionary<Layer::Name, Extension::Properties>& extensionsProperties_)
			{
				Extension::CNames extensionsNames;
				{
					for(auto &extensionProperties : extensionsProperties_)
					{
						for(auto &extensionProperty : extensionProperties.second)
						{
							extensionsNames.push_back(extensionProperty.name.c_str());
						}
					}
				}

				return std::move(extensionsNames);
			}
		public:
			inline Device(PhysicalDevice* physicalDevice_, const Layer::CNames& layerCNames_, const Extension::CNames& extensionCNames_):
				PhysicalDevice::Dependent(physicalDevice_),
				handle([&](){
					Vector<VkDeviceQueueCreateInfo> vk_deviceQueueCreateInfos(1);
					Vector<Vector<float>> vk_deviceQueuesPriorities(vk_deviceQueueCreateInfos.size(), Vector<float>(1, 0.0f));
					{
						for(Size i = 0; i < vk_deviceQueueCreateInfos.size(); ++i)
						{
							auto &vk_deviceQueueCreateInfo = vk_deviceQueueCreateInfos[i];
							auto &vk_deviceQueuePriorities = vk_deviceQueuesPriorities[i];

							vk_deviceQueueCreateInfos[i].sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
							vk_deviceQueueCreateInfos[i].pNext = nullptr;
							vk_deviceQueueCreateInfos[i].flags = 0;
							vk_deviceQueueCreateInfos[i].queueFamilyIndex = i;
							vk_deviceQueueCreateInfos[i].queueCount = vk_deviceQueuePriorities.size();
							vk_deviceQueueCreateInfos[i].pQueuePriorities = vk_deviceQueuePriorities.data();
						}
					}

					VkPhysicalDeviceFeatures vk_physicalDeviceFeatures;
					{
						vk_physicalDeviceFeatures.robustBufferAccess = VK_FALSE;
						vk_physicalDeviceFeatures.fullDrawIndexUint32 = VK_FALSE;
						vk_physicalDeviceFeatures.imageCubeArray = VK_FALSE;
						vk_physicalDeviceFeatures.independentBlend = VK_FALSE;
						vk_physicalDeviceFeatures.geometryShader = VK_TRUE; // VK_FALSE;
						vk_physicalDeviceFeatures.tessellationShader = VK_FALSE;
						vk_physicalDeviceFeatures.sampleRateShading = VK_FALSE;
						vk_physicalDeviceFeatures.dualSrcBlend = VK_FALSE;
						vk_physicalDeviceFeatures.logicOp = VK_FALSE;
						vk_physicalDeviceFeatures.multiDrawIndirect = VK_FALSE;
						vk_physicalDeviceFeatures.drawIndirectFirstInstance = VK_FALSE;
						vk_physicalDeviceFeatures.depthClamp = VK_FALSE;
						vk_physicalDeviceFeatures.depthBiasClamp = VK_FALSE;
						vk_physicalDeviceFeatures.fillModeNonSolid = VK_FALSE;
						vk_physicalDeviceFeatures.depthBounds = VK_FALSE;
						vk_physicalDeviceFeatures.wideLines = VK_FALSE;
						vk_physicalDeviceFeatures.largePoints = VK_FALSE;
						vk_physicalDeviceFeatures.alphaToOne = VK_FALSE;
						vk_physicalDeviceFeatures.multiViewport = VK_FALSE;
						vk_physicalDeviceFeatures.samplerAnisotropy = VK_FALSE;
						vk_physicalDeviceFeatures.textureCompressionETC2 = VK_FALSE;
						vk_physicalDeviceFeatures.textureCompressionASTC_LDR = VK_FALSE;
						vk_physicalDeviceFeatures.textureCompressionBC = VK_FALSE;
						vk_physicalDeviceFeatures.occlusionQueryPrecise = VK_FALSE;
						vk_physicalDeviceFeatures.pipelineStatisticsQuery = VK_FALSE;
						vk_physicalDeviceFeatures.vertexPipelineStoresAndAtomics = VK_FALSE;
						vk_physicalDeviceFeatures.fragmentStoresAndAtomics = VK_FALSE;
						vk_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize = VK_TRUE; // VK_FALSE;
						vk_physicalDeviceFeatures.shaderImageGatherExtended = VK_FALSE;
						vk_physicalDeviceFeatures.shaderStorageImageExtendedFormats = VK_FALSE;
						vk_physicalDeviceFeatures.shaderStorageImageMultisample = VK_FALSE;
						vk_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat = VK_FALSE;
						vk_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat = VK_FALSE;
						vk_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing = VK_FALSE;
						vk_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing = VK_FALSE;
						vk_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing = VK_FALSE;
						vk_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing = VK_FALSE;
						vk_physicalDeviceFeatures.shaderClipDistance = VK_TRUE; // VK_FALSE;
						vk_physicalDeviceFeatures.shaderCullDistance = VK_TRUE; // VK_FALSE;
						vk_physicalDeviceFeatures.shaderFloat64 = VK_FALSE;
						vk_physicalDeviceFeatures.shaderInt64 = VK_FALSE;
						vk_physicalDeviceFeatures.shaderInt16 = VK_FALSE;
						vk_physicalDeviceFeatures.shaderResourceResidency = VK_FALSE;
						vk_physicalDeviceFeatures.shaderResourceMinLod = VK_FALSE;
						vk_physicalDeviceFeatures.sparseBinding = VK_FALSE;
						vk_physicalDeviceFeatures.sparseResidencyBuffer = VK_FALSE;
						vk_physicalDeviceFeatures.sparseResidencyImage2D = VK_FALSE;
						vk_physicalDeviceFeatures.sparseResidencyImage3D = VK_FALSE;
						vk_physicalDeviceFeatures.sparseResidency2Samples = VK_FALSE;
						vk_physicalDeviceFeatures.sparseResidency4Samples = VK_FALSE;
						vk_physicalDeviceFeatures.sparseResidency8Samples = VK_FALSE;
						vk_physicalDeviceFeatures.sparseResidency16Samples = VK_FALSE;
						vk_physicalDeviceFeatures.sparseResidencyAliased = VK_FALSE;
						vk_physicalDeviceFeatures.variableMultisampleRate = VK_FALSE;
						vk_physicalDeviceFeatures.inheritedQueries = VK_FALSE;
					}

					VkDeviceCreateInfo vk_deviceCreateInfo;
					{
						vk_deviceCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
						vk_deviceCreateInfo.pNext = nullptr;
						vk_deviceCreateInfo.flags = 0;
						vk_deviceCreateInfo.queueCreateInfoCount = vk_deviceQueueCreateInfos.size();
						vk_deviceCreateInfo.pQueueCreateInfos = vk_deviceQueueCreateInfos.data();
						vk_deviceCreateInfo.enabledLayerCount = layerCNames_.size();
						vk_deviceCreateInfo.ppEnabledLayerNames = layerCNames_.data();
						vk_deviceCreateInfo.enabledExtensionCount = extensionCNames_.size();
						vk_deviceCreateInfo.ppEnabledExtensionNames = extensionCNames_.data();
						vk_deviceCreateInfo.pEnabledFeatures = &vk_physicalDeviceFeatures;
					}
					
					return CreateDevice(physicalDevice_->GetHandle(), &vk_deviceCreateInfo, nullptr);
				}())
			{
			}
			inline ~Device()
			{
				DestroyDevice(handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class DeviceMemory:
			public Device::Dependent
		{
		public:
			using Handle = VkDeviceMemory;
			using Size = VkDeviceSize;
			using Property = VkMemoryPropertyFlags;
			using Properties = VkMemoryPropertyFlagBits;
			using Requirements = VkMemoryRequirements;
		public:
			class User
			{
			protected:
				DeviceMemory* deviceMemory = nullptr;
			public:
				inline void SetDeviceMemory(DeviceMemory* deviceMemory_)
				{
					deviceMemory = deviceMemory_;
				}
				inline DeviceMemory* GetSetDeviceMemory() const
				{
					return deviceMemory;
				}
			};
		protected:
			struct HandleSize
			{
				const Handle handle;
				const Size size;
			};
		protected:
			const Handle handle;
			const Size size;
		protected:
			inline DeviceMemory(Device* device_, const Requirements& requirements_, const Property& property_);
		public:
			inline DeviceMemory(BasicImage* image_, const Property& property_ = 0);
			inline DeviceMemory(Buffer* buffer_, const Property& property_ = Properties::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			inline ~DeviceMemory()
			{
				FreeMemory(device->GetHandle(), handle, nullptr);
			}
		public:
			template<class T = void>
			inline T* Map(Size size_, Size offset_)
			{
				return (T*)MapMemory(device->GetHandle(), handle, offset_, size_, 0);
			}
			template<class T = void>
			inline T* Map()
			{
				return Map<T>(size, 0);
			}
			inline void Unmap()
			{
				UnmapMemory(device->GetHandle(), handle);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class Buffer:
			public Device::Dependent,
			protected DeviceMemory::User
		{
		public:
			using Handle = VkBuffer;
			using Usage = VkBufferUsageFlags;
			using Usages = VkBufferUsageFlagBits;
			using Sharing = VkSharingMode;
			// template<class T> using Data = Vector<T>;
		protected:
			const Handle handle;
			const Size size;
			const Usage usage;
			const Sharing sharing;
		public:
			inline Buffer(Device* device_, const Size& size_, const Usage& usage_, const Sharing& sharing_):
				Dependent(device_),
				handle([&]()
				{
					VkBufferCreateInfo vk_bufferCreateInfo;
					{
						vk_bufferCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
						vk_bufferCreateInfo.pNext = nullptr;
						vk_bufferCreateInfo.flags = 0;
						vk_bufferCreateInfo.size = size_;
						vk_bufferCreateInfo.usage = (VkBufferUsageFlags)usage_; // VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
						vk_bufferCreateInfo.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
						vk_bufferCreateInfo.queueFamilyIndexCount = 0;
						vk_bufferCreateInfo.pQueueFamilyIndices = nullptr;
					}

					return CreateBuffer(device->GetHandle(), &vk_bufferCreateInfo, nullptr);
				}()),
				size(size_),
				usage(usage_),
				sharing(sharing_)
			{
			}
			inline ~Buffer()
			{
				DestroyBuffer(device->GetHandle(), handle, nullptr);
			}
		public:
			inline void SetDeviceMemory(DeviceMemory* deviceMemory_)
			{
				DeviceMemory::User::SetDeviceMemory(deviceMemory_);

				BindBufferMemory(device->GetHandle(), handle, deviceMemory ? deviceMemory->GetHandle() : VK_NULL_HANDLE, 0);
			}
			inline DeviceMemory* GetSetDeviceMemory() const
			{
				return DeviceMemory::User::GetSetDeviceMemory();
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class BasicImage:
			public Device::Dependent
			// public DeviceMemoryUser
		{
		public:
			class Dependent
			{
			protected:
				BasicImage*const basicImage;
			public:
				inline Dependent(BasicImage* basicImage_):
					basicImage(basicImage_)
				{
				}
				inline Dependent(const Dependent&) = default;
			public:
				inline BasicImage* GetBasicImage() const
				{
					return basicImage;
				}
			};
		public:
			using Handle = VkImage;
			using Size = VkExtent3D;
			using Type = VkImageType;
			using Format = VkFormat;
			using Tiling = VkImageTiling;
			using Usage = VkImageUsageFlags;
			using UsageBits = VkImageUsageFlagBits;
			using Layout = VkImageLayout;
		protected:
			const Handle handle;
			const Size size;
			const Type type;
			const Format format;
			const Usage usage;
		protected:
			inline BasicImage(Device* device_, Handle handle_, Size size_, Type type_, Format format_, Usage usage_):
				Device::Dependent(device_),
				handle(handle_),
				size(size_),
				type(type_),
				format(format_),
				usage(usage_)
			{
			}
			inline ~BasicImage() = default;
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Size GetSize() const
			{
				return size;
			}
			inline Type GetType() const
			{
				return type;
			}
			inline Format GetFormat() const
			{
				return format;
			}
			inline Usage GetUsage() const
			{
				return usage;
			}
		};
		class Image:
			public BasicImage
		{
		protected:
			inline Image(Device* device_, Handle handle_, Size size_, Type type_, Format format_, Usage usage_):
				BasicImage(device_, handle_, size_, type_, format_, usage_)
			{
			}
			inline ~Image() = default;
		};
		class SurfaceKHR:
			public PhysicalDevice::Dependent
		{
		public:
			class Dependent
			{
			protected:
				SurfaceKHR*const surfaceKHR;
			public:
				inline Dependent(SurfaceKHR* surfaceKHR_):
					surfaceKHR(surfaceKHR_)
				{
				}
				inline Dependent(const Dependent&) = default;
			public:
				inline SurfaceKHR* GetSurfaceKHR() const
				{
					return surfaceKHR;
				}
			};
		public:
			using Handle = VkSurfaceKHR;
			using WinInstance = HINSTANCE;
			using WinWindow = HWND;
			using Capabilities = VkSurfaceCapabilitiesKHR;
			using PresentMode = VkPresentModeKHR;
			using Format = VkSurfaceFormatKHR;
		protected:
			const Handle handle;
			const Capabilities capabilities;
			const Vector<PresentMode> presentModes;
			const Vector<Format> formats;
		public:
			inline SurfaceKHR(PhysicalDevice* physicalDevice_, WinInstance winInstance_, WinWindow winWindow_):
				PhysicalDevice::Dependent(physicalDevice_),
				handle([&]()
				{
					VkWin32SurfaceCreateInfoKHR vk_win32SurfaceCreateInfoKHR;
					{
						vk_win32SurfaceCreateInfoKHR.sType = VkStructureType::VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
						vk_win32SurfaceCreateInfoKHR.pNext = nullptr;
						vk_win32SurfaceCreateInfoKHR.flags = 0;
						vk_win32SurfaceCreateInfoKHR.hinstance = winInstance_;
						vk_win32SurfaceCreateInfoKHR.hwnd = winWindow_;
					}

					return CreateWin32SurfaceKHR(physicalDevice->GetInstance()->GetHandle(), &vk_win32SurfaceCreateInfoKHR, nullptr);
				}()),
				capabilities(GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->GetHandle(), handle)),
				presentModes(GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->GetHandle(), handle)),
				formats(GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->GetHandle(), handle))
			{
				if(!GetPhysicalDeviceSurfaceSupportKHR(physicalDevice->GetHandle(), handle, 0))
				{
					throw Exception("Surface not supported");
				}
			}
			inline ~SurfaceKHR()
			{
				DestroySurfaceKHR(physicalDevice->GetInstance()->GetHandle(), handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Capabilities GetCapabilities() const
			{
				return capabilities;
			}
			const Vector<PresentMode> GetPresentModes() const
			{
				return presentModes;
			}
			const Vector<Format> GetFormats() const
			{
				return formats;
			}
		};
		class SwapchainKHR:
			public Device::Dependent,
			public SurfaceKHR::Dependent
		{
		public:
			class Dependent
			{
			protected:
				SwapchainKHR*const swapchainKHR;
			public:
				inline Dependent(SwapchainKHR* swapchainKHR_):
					swapchainKHR(swapchainKHR_)
				{
				}
				inline Dependent(const Dependent&) = default;
			public:
				inline SwapchainKHR* GetSwapchainKHR() const
				{
					return swapchainKHR;
				}
			};
			class Image:
				public Dependent,
				public BasicImage
			{
			public:
				inline Image(SwapchainKHR* swapchainKHR_, Handle handle_, Size size_, Type type_, Format format_, Usage usage_):
					SwapchainKHR::Dependent(swapchainKHR_),
					BasicImage(swapchainKHR->GetDevice(), handle_, size_, type_, format_, usage_)
				{
				}
				inline ~Image() = default;
			};
		public:
			using Handle = VkSwapchainKHR;
			using Format = VkFormat;
			using ColorSpace = VkColorSpaceKHR;
			using Extent = VkExtent2D;
		protected:
			const Handle handle;
			const Vector<Image*> images;
		public:
			inline SwapchainKHR(Device* device_, SurfaceKHR* surfaceKHR_, const Extent& extent_, const Format& format_, const ColorSpace& colorSpace_):
				Device::Dependent(device_),
				SurfaceKHR::Dependent(surfaceKHR_),
				handle([&]()
				{
					VkSwapchainCreateInfoKHR vk_SwapchainCreateInfo;
					{
						vk_SwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
						vk_SwapchainCreateInfo.pNext = nullptr;
						vk_SwapchainCreateInfo.flags = 0;
						vk_SwapchainCreateInfo.surface = surfaceKHR->GetHandle();
						vk_SwapchainCreateInfo.minImageCount = 2;
						vk_SwapchainCreateInfo.imageFormat = format_;
						vk_SwapchainCreateInfo.imageColorSpace = colorSpace_;
						vk_SwapchainCreateInfo.imageExtent = extent_;
						vk_SwapchainCreateInfo.imageArrayLayers = 1;
						vk_SwapchainCreateInfo.imageUsage = VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
						vk_SwapchainCreateInfo.imageSharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
						vk_SwapchainCreateInfo.queueFamilyIndexCount = 0;
						vk_SwapchainCreateInfo.pQueueFamilyIndices = nullptr;
						vk_SwapchainCreateInfo.preTransform = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
						vk_SwapchainCreateInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
						vk_SwapchainCreateInfo.presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
						vk_SwapchainCreateInfo.clipped = true;
						vk_SwapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
					}

					return CreateSwapchainKHR(device->GetHandle(), &vk_SwapchainCreateInfo, nullptr);
				}()),
				images([&]()
				{
					auto vk_images = std::move(GetSwapchainImagesKHR(device->GetHandle(), handle));

					Vector<Image*> images(vk_images.size());

					for(Size i = 0; i < vk_images.size(); ++i)
					{
						images[i] = new Image(
							this,
							vk_images[i],
							Image::Size{extent_.width, extent_.height, 1},
							Image::Type::VK_IMAGE_TYPE_2D,
							format_,
							Image::UsageBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
						);
					}

					return images;
				}())
			{
			}
			inline ~SwapchainKHR()
			{
				for(auto &image : images)
				{
					delete image;
				}

				DestroySwapchainKHR(device->GetHandle(), handle, nullptr);
			}
		public:
			inline Size AccuireNextImage(Fence* fence_, Semaphore* semaphore_, Size timeout_ = (Size)UINT64_MAX);
			inline Size AccuireNextImage(Fence* fence_)
			{
				return AccuireNextImage(fence_, nullptr);
			}
			inline Size AccuireNextImage(Semaphore* semaphore_)
			{
				return AccuireNextImage(nullptr, semaphore_);
			}
			inline Size AccuireNextImage(Fence* fence_, Size timeout_)
			{
				return AccuireNextImage(fence_, nullptr, timeout_);
			}
			inline Size AccuireNextImage(Semaphore* semaphore_, Size timeout_)
			{
				return AccuireNextImage(nullptr, semaphore_, timeout_);
			}
			inline void Preset(Queue* queue_, Size swapchainImage_);
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Vector<Image*> GetImages() const
			{
				return images;
			}
		};
		class ImageView:
			public Device::Dependent,
			public BasicImage::Dependent
		{
		public:
			using Handle = VkImageView;
			using Type = VkImageViewType;
		protected:
			const Handle handle;
			const Type type;
		public:
			inline ImageView(Device* device_, BasicImage* basicImage_, const Type& type_):
				Device::Dependent(device_),
				BasicImage::Dependent(basicImage_),
				handle([&]()
				{
					VkImageViewCreateInfo vk_imageViewCreateInfo;
					{
						vk_imageViewCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
						vk_imageViewCreateInfo.pNext = nullptr;
						vk_imageViewCreateInfo.flags = 0;
						vk_imageViewCreateInfo.image = basicImage->GetHandle();
						vk_imageViewCreateInfo.viewType = type_;
						vk_imageViewCreateInfo.format = basicImage->GetFormat();
						vk_imageViewCreateInfo.components = {
							VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R,
							VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G,
							VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B,
							VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A
						};
						vk_imageViewCreateInfo.subresourceRange;
						{
							vk_imageViewCreateInfo.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
							vk_imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
							vk_imageViewCreateInfo.subresourceRange.levelCount = 1;
							vk_imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
							vk_imageViewCreateInfo.subresourceRange.layerCount = 1;
						}
					}

					return CreateImageView(device->GetHandle(), &vk_imageViewCreateInfo, nullptr);
				}()),
				type(type_)
			{
			}
			inline ~ImageView()
			{
				DestroyImageView(device->GetHandle(), handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Type GetType() const
			{
				return type;
			}
		};
		class RenderPass:
			public Device::Dependent
		{
		public:
			class Dependent
			{
			protected:
				RenderPass*const renderPass;
			public:
				inline Dependent(RenderPass* renderPass_):
					renderPass(renderPass_)
				{
				}
				inline Dependent(const Dependent&) = default;
			public:
				inline RenderPass* GetRenderPass() const
				{
					return renderPass;
				}
			};
		public:
			// class Attachment
			// {
			// };
			// class Reference
			// {
			// public:
			// 	enum class Type
			// 	{
			// 		None,
			// 		Input,
			// 		Color,
			// 		DepthStencil
			// 	};
			// public:
			// 	const Type type;
			// 	Attachment*const attachment;
			// public:
			// 	inline Reference(const Type& type_, Attachment* attachment_):
			// 		type(type_),
			// 		attachment(attachment_)
			// 	{
			// 	}
			// };
			// class Subpass
			// {
			// public:
			// 	const Vector<Reference*> references;
			// public:
			// 	inline Subpass(const Initializer<Reference*>& references_):
			// 		references(references_)
			// 	{
			// 	}
			// };
		public:
			using Handle = VkRenderPass;
			using Rect = VkRect2D;
		protected:
			const Handle handle;
		public:
			inline RenderPass(Device* device_, const Vector<VkAttachmentDescription>& vk_attachmentDescriptions_, const Vector<VkSubpassDescription>& vk_subpassDescriptions_):
				Device::Dependent(device_),
				handle([&]()
				{
					Vector<VkSubpassDependency> vk_subpassDependencies;
					{
						// vk_subpassDependency.srcSubpass;
						// vk_subpassDependency.dstSubpass;
						// vk_subpassDependency.srcStageMask;
						// vk_subpassDependency.dstStageMask;
						// vk_subpassDependency.srcAccessMask;
						// vk_subpassDependency.dstAccessMask;
						// vk_subpassDependency.dependencyFlags;
					}

					VkRenderPassCreateInfo vk_renderPassCreateInfo;
					{
						vk_renderPassCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
						vk_renderPassCreateInfo.pNext = nullptr;
						vk_renderPassCreateInfo.flags = 0;
						vk_renderPassCreateInfo.attachmentCount = vk_attachmentDescriptions_.size();
						vk_renderPassCreateInfo.pAttachments = vk_attachmentDescriptions_.data();
						vk_renderPassCreateInfo.subpassCount = vk_subpassDescriptions_.size();
						vk_renderPassCreateInfo.pSubpasses = vk_subpassDescriptions_.data();
						vk_renderPassCreateInfo.dependencyCount = vk_subpassDependencies.size();
						vk_renderPassCreateInfo.pDependencies = vk_subpassDependencies.data();
					}

					return CreateRenderPass(device->GetHandle(), &vk_renderPassCreateInfo, nullptr);
				}())
			{
			}
			inline ~RenderPass()
			{
				DestroyRenderPass(device->GetHandle(), handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class Framebuffer:
			public Device::Dependent,
			public RenderPass::Dependent
		{
		public:
			using Handle = VkFramebuffer;
			using Size = VkExtent2D;
		protected:
			const Handle handle;
			const Size size;
		public:
			inline Framebuffer(Device* device_, RenderPass* renderPass_, const Size& size_, const Vector<ImageView*>& imageViews_):
				Device::Dependent(device_),
				RenderPass::Dependent(renderPass_),
				handle([&](){
					Vector<ImageView::Handle> imageViews(imageViews_.size());
					{
						for(GreatVEngine::Size i = 0; i < imageViews_.size(); ++i)
						{
							imageViews[i] = imageViews_[i]->GetHandle();
						}
					}

					VkFramebufferCreateInfo vk_framebufferCreateInfo;
					{
						vk_framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
						vk_framebufferCreateInfo.pNext = nullptr;
						vk_framebufferCreateInfo.flags = 0;
						vk_framebufferCreateInfo.renderPass = renderPass->GetHandle();
						vk_framebufferCreateInfo.attachmentCount = imageViews.size();
						vk_framebufferCreateInfo.pAttachments = imageViews.data();
						vk_framebufferCreateInfo.width = size_.width;
						vk_framebufferCreateInfo.height = size_.height;
						vk_framebufferCreateInfo.layers = 1;
					}
					
					return CreateFramebuffer(device->GetHandle(), &vk_framebufferCreateInfo, nullptr);
				}()),
				size(size_)
			{
			}
			inline ~Framebuffer()
			{
				DestroyFramebuffer(device->GetHandle(), handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class Queue:
			Device::Dependent
		{
		public:
			using Handle = VkQueue;
			using Index = uint32_t;
		protected:
			const Handle handle;
			const Index familyIndex;
			const Index index;
		public:
			inline Queue(Device* device_, Index familyIndex_, Index index_):
				Device::Dependent(device_),
				handle(GetDeviceQueue(device->GetHandle(), familyIndex_, index_)),
				familyIndex(familyIndex_),
				index(index_)
			{
			}
			inline ~Queue() = default;
		public:
			inline void Submit(CommandBuffer* commandBuffers_);
			inline void Wait()
			{
				QueueWaitIdle(handle);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Index GetFamilyIndex() const
			{
				return familyIndex;
			}
			inline Index GetIndex() const
			{
				return index;
			}
		};
		class Command
		{
		public:
			inline Command() = default;
			virtual ~Command() = default;
		public:
			virtual void Parse(CommandBuffer* commandBuffer_) = 0;
		};
		namespace Commands
		{
			class BindGraphicPipeline:
				public Command
			{
			protected:
				const GraphicPipeline* graphicPipeline;
			public:
				inline BindGraphicPipeline(GraphicPipeline* graphicPipeline_):
					graphicPipeline(graphicPipeline_)
				{
				}
			public:
				virtual void Parse(CommandBuffer* commandBuffer_) override;
			};
			class BindVertexBuffers:
				public Command
			{
			protected:
				const Vector<Buffer*> buffers;
				const Vector<VkDeviceSize> offsets;
			public:
				inline BindVertexBuffers(const Vector<Buffer*>& buffers_, const Vector<VkDeviceSize>& offsets_):
					buffers(buffers_), offsets(offsets_)
				{
					if(buffers.size() != offsets.size())
					{
						throw Exception("Buffers count not equal to offsets count");
					}
				}
			public:
				virtual void Parse(CommandBuffer* commandBuffer_) override;
			};
			class RenderPass:
				public Command
			{
			public:
				using Content = VkSubpassContents;
				using Area = VkRect2D;
			protected:
				Vulkan::RenderPass*const renderPass;
				Framebuffer*const framebuffer;
				const Vector<Command*> commands;
				const Content content;
				const Area area;
				const Vector<VkClearValue> clearValues;
			public:
				inline RenderPass(
					Vulkan::RenderPass* renderPass_,
					Framebuffer*const framebuffer_,
					const Content& content_,
					const Area area_,
					const Vector<VkClearValue> clearValues_,
					const Vector<Command*> commands_):
					renderPass(renderPass_),
					framebuffer(framebuffer_),
					content(content_),
					area(area_),
					clearValues(clearValues_),
					commands(commands_)
				{
				}
			public:
				virtual void Parse(CommandBuffer* commandBuffer_) override;
			};
			class Draw:
				public Command
			{
			protected:
				const Size vertivesBegin, vertivesCount;
				const Size instancesBegin, instancesCount;
			public:
				inline Draw(const Size& vertivesBegin_, const Size& vertivesCount_, const Size& instancesBegin_, const Size& instancesCount_):
					vertivesBegin(vertivesBegin_), vertivesCount(vertivesCount_), instancesBegin(instancesBegin_), instancesCount(instancesCount_)
				{
				}
			public:
				virtual void Parse(CommandBuffer* commandBuffer_) override;
			};
		}
		class CommandPool:
			public Device::Dependent
		{
		public:
			class Dependent
			{
			protected:
				CommandPool*const commandPool;
			public:
				inline Dependent(CommandPool* commandPool_):
					commandPool(commandPool_)
				{
				}
				inline Dependent(const Dependent&) = default;
			public:
				inline CommandPool* GetCommandPool() const
				{
					return commandPool;
				}
			};
		public:
			using Handle = VkCommandPool;
		protected:
			const Handle handle;
			const Queue::Index queueFamilyIndex;
		public:
			inline CommandPool(Device* device_, Queue::Index queueFamilyIndex_):
				Device::Dependent(device_),
				handle([&]()
				{
					VkCommandPoolCreateInfo vk_commandPoolCreateInfo;
					{
						vk_commandPoolCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
						vk_commandPoolCreateInfo.pNext = nullptr;
						vk_commandPoolCreateInfo.flags = VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
						vk_commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex_;
					}

					return CreateCommandPool(device->GetHandle(), &vk_commandPoolCreateInfo, nullptr);
				}()),
				queueFamilyIndex(queueFamilyIndex_)
			{
			}
			inline ~CommandPool()
			{
				DestroyCommandPool(device->GetHandle(), handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Queue::Index GetQueueFamilyIndex() const
			{
				return queueFamilyIndex;
			}
		};
		class CommandBuffer:
			public CommandPool::Dependent
		{
		public:
			enum class Level: VkFlags
			{
				Primary = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				Secondary = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_SECONDARY
			};
		public:
			using Handle = VkCommandBuffer;
		protected:
			const Handle handle;
			const Level level;
		public:
			inline CommandBuffer(CommandPool* commandPool_, const Level& level_):
				CommandPool::Dependent(commandPool_),
				handle([&]()
				{
					VkCommandBufferAllocateInfo vk_CommandBufferAllocateInfo;
					{
						vk_CommandBufferAllocateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
						vk_CommandBufferAllocateInfo.pNext = nullptr;
						vk_CommandBufferAllocateInfo.commandPool = commandPool->GetHandle();
						vk_CommandBufferAllocateInfo.level = (VkCommandBufferLevel)level_;
						vk_CommandBufferAllocateInfo.commandBufferCount = 1;
					}

					return AllocateCommandBuffers(commandPool_->GetDevice()->GetHandle(), &vk_CommandBufferAllocateInfo)[0];
				}()),
				level(level_)
			{
			}
			inline ~CommandBuffer()
			{
				FreeCommandBuffers(commandPool->GetDevice()->GetHandle(), commandPool->GetHandle(), Vector<Handle>({handle}));
			}
		public:
			inline void Record(RenderPass* renderPass_, Size subpass_, Framebuffer* framebuffer_, Vector<Command*> commands)
			{
				VkCommandBufferInheritanceInfo vk_commandBufferInheritanceInfo;
				{
					vk_commandBufferInheritanceInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
					vk_commandBufferInheritanceInfo.pNext = nullptr;
					vk_commandBufferInheritanceInfo.renderPass = renderPass_ ? renderPass_->GetHandle() : VK_NULL_HANDLE;
					vk_commandBufferInheritanceInfo.subpass = subpass_;
					vk_commandBufferInheritanceInfo.framebuffer = framebuffer_ ? framebuffer_->GetHandle() : VK_NULL_HANDLE;
					vk_commandBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
					vk_commandBufferInheritanceInfo.queryFlags = 0;
					vk_commandBufferInheritanceInfo.pipelineStatistics = 0;
				}
				VkCommandBufferBeginInfo vk_commandBufferBeginInfo;
				{
					vk_commandBufferBeginInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					vk_commandBufferBeginInfo.pNext = nullptr;
					vk_commandBufferBeginInfo.flags = 0;
					vk_commandBufferBeginInfo.pInheritanceInfo = &vk_commandBufferInheritanceInfo;
				}

				BeginCommandBuffer(handle, &vk_commandBufferBeginInfo);

				for(auto &command : commands)
				{
					command->Parse(this);
				}

				EndCommandBuffer(handle);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
			inline Level GetLevel() const
			{
				return level;
			}
		};
		class Shader:
			Device::Dependent
		{
		public:
			using Handle = VkShaderModule;
			using Source = Vector<UInt32>;
		protected:
			const Handle handle;
		public:
			inline Shader(Device* device_, const Source& source_):
				Dependent(device_),
				handle([&]()
				{
					VkShaderModuleCreateInfo vk_shaderModuleCreateInfo;
					{
						vk_shaderModuleCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
						vk_shaderModuleCreateInfo.pNext = nullptr;
						vk_shaderModuleCreateInfo.flags = 0;
						vk_shaderModuleCreateInfo.codeSize = source_.size();
						vk_shaderModuleCreateInfo.pCode = source_.data();
					}

					return CreateShaderModule(device->GetHandle(), &vk_shaderModuleCreateInfo, nullptr);
				}())
			{
			}
			inline ~Shader()
			{
				DestroyShaderModule(device->GetHandle(), handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class PipelineLayout:
			Device::Dependent
		{
		public:
			using Handle = VkPipelineLayout;
		protected:
			const Handle handle;
		public:
			inline PipelineLayout(Device* device_):
				Dependent(device_),
				handle([&]()
				{
					VkPipelineLayoutCreateInfo vk_pipelineLayoutCreateInfo;
					{
						vk_pipelineLayoutCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
						vk_pipelineLayoutCreateInfo.pNext = nullptr;
						vk_pipelineLayoutCreateInfo.flags = 0;
						vk_pipelineLayoutCreateInfo.setLayoutCount = 0;
						vk_pipelineLayoutCreateInfo.pSetLayouts = nullptr;
						vk_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
						vk_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
					}

					return CreatePipelineLayout(device->GetHandle(), &vk_pipelineLayoutCreateInfo, nullptr);
				}())
			{
			}
			inline ~PipelineLayout()
			{
				DestroyPipelineLayout(device->GetHandle(), handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class PipelineCache:
			Device::Dependent
		{
		public:
			using Handle = VkPipelineCache;
		protected:
			const Handle handle;
		public:
			inline PipelineCache(Device* device_):
				Dependent(device_),
				handle([&]()
				{
					VkPipelineCacheCreateInfo vk_pipelineCacheCreateInfo;
					{
						vk_pipelineCacheCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
						vk_pipelineCacheCreateInfo.pNext = nullptr;
						vk_pipelineCacheCreateInfo.flags = 0;
						vk_pipelineCacheCreateInfo.initialDataSize = 0;
						vk_pipelineCacheCreateInfo.pInitialData = nullptr;
					}
				
					return CreatePipelineCache(device->GetHandle(), &vk_pipelineCacheCreateInfo, nullptr);
				}())
			{
			}
			inline ~PipelineCache()
			{
				DestroyPipelineCache(device->GetHandle(), handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class GraphicPipeline:
			Device::Dependent
		{
		public:
			struct Shaders
			{
			public:
				Shader* vertex = nullptr;
				Shader* tessellationControl = nullptr;
				Shader* tessellationEvaluation = nullptr;
				Shader* geometry = nullptr;
				Shader* fragment = nullptr;
			public:
				inline Shaders(
					Shader* vertex_,
					Shader* tessellationControl_,
					Shader* tessellationEvaluation_,
					Shader* geometry_,
					Shader* fragment_):
					vertex(vertex_),
					tessellationControl(tessellationControl_),
					tessellationEvaluation(tessellationEvaluation_),
					geometry(geometry_),
					fragment(fragment_)
				{
				}
			public:
				inline Vector<VkPipelineShaderStageCreateInfo> Get() const
				{
					Vector<VkPipelineShaderStageCreateInfo> vk_pipelineShaderStageCreateInfos;

					if(vertex)
					{
						VkPipelineShaderStageCreateInfo vk_pipelineShaderStageCreateInfo;
						{
							vk_pipelineShaderStageCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
							vk_pipelineShaderStageCreateInfo.pNext = nullptr;
							vk_pipelineShaderStageCreateInfo.flags = 0;
							vk_pipelineShaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
							vk_pipelineShaderStageCreateInfo.module = vertex->GetHandle();
							vk_pipelineShaderStageCreateInfo.pName = "main";
							vk_pipelineShaderStageCreateInfo.pSpecializationInfo = nullptr;
						}

						vk_pipelineShaderStageCreateInfos.push_back(vk_pipelineShaderStageCreateInfo);
					}

					if(tessellationControl)
					{
						VkPipelineShaderStageCreateInfo vk_pipelineShaderStageCreateInfo;
						{
							vk_pipelineShaderStageCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
							vk_pipelineShaderStageCreateInfo.pNext = nullptr;
							vk_pipelineShaderStageCreateInfo.flags = 0;
							vk_pipelineShaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
							vk_pipelineShaderStageCreateInfo.module = tessellationControl->GetHandle();
							vk_pipelineShaderStageCreateInfo.pName = "main";
							vk_pipelineShaderStageCreateInfo.pSpecializationInfo = nullptr;
						}

						vk_pipelineShaderStageCreateInfos.push_back(vk_pipelineShaderStageCreateInfo);
					}

					if(tessellationEvaluation)
					{
						VkPipelineShaderStageCreateInfo vk_pipelineShaderStageCreateInfo;
						{
							vk_pipelineShaderStageCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
							vk_pipelineShaderStageCreateInfo.pNext = nullptr;
							vk_pipelineShaderStageCreateInfo.flags = 0;
							vk_pipelineShaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
							vk_pipelineShaderStageCreateInfo.module = tessellationEvaluation->GetHandle();
							vk_pipelineShaderStageCreateInfo.pName = "main";
							vk_pipelineShaderStageCreateInfo.pSpecializationInfo = nullptr;
						}

						vk_pipelineShaderStageCreateInfos.push_back(vk_pipelineShaderStageCreateInfo);
					}

					if(geometry)
					{
						VkPipelineShaderStageCreateInfo vk_pipelineShaderStageCreateInfo;
						{
							vk_pipelineShaderStageCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
							vk_pipelineShaderStageCreateInfo.pNext = nullptr;
							vk_pipelineShaderStageCreateInfo.flags = 0;
							vk_pipelineShaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
							vk_pipelineShaderStageCreateInfo.module = geometry->GetHandle();
							vk_pipelineShaderStageCreateInfo.pName = "main";
							vk_pipelineShaderStageCreateInfo.pSpecializationInfo = nullptr;
						}

						vk_pipelineShaderStageCreateInfos.push_back(vk_pipelineShaderStageCreateInfo);
					}

					if(fragment)
					{
						VkPipelineShaderStageCreateInfo vk_pipelineShaderStageCreateInfo;
						{
							vk_pipelineShaderStageCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
							vk_pipelineShaderStageCreateInfo.pNext = nullptr;
							vk_pipelineShaderStageCreateInfo.flags = 0;
							vk_pipelineShaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
							vk_pipelineShaderStageCreateInfo.module = fragment->GetHandle();
							vk_pipelineShaderStageCreateInfo.pName = "main";
							vk_pipelineShaderStageCreateInfo.pSpecializationInfo = nullptr;
						}

						vk_pipelineShaderStageCreateInfos.push_back(vk_pipelineShaderStageCreateInfo);
					}

					return vk_pipelineShaderStageCreateInfos;
				}
			};
		public:
			using Handle = VkPipeline;
		protected:
			const Handle handle;
		public:
			inline GraphicPipeline(
				Device* device_,
				PipelineCache* pipelineCache_, PipelineLayout* pipelineLayout_, RenderPass* renderPass_,
				const Shaders& shaders_,
				Vector<VkViewport> viewports, Vector<VkRect2D> scissors):
				Dependent(device_),
				handle([&]()
				{
					auto vk_pipelineShaderStageCreateInfos = shaders_.Get();

					VkVertexInputBindingDescription vk_vertexInputBindingDescription;
					{
						vk_vertexInputBindingDescription.binding = 0;
						vk_vertexInputBindingDescription.stride = sizeof(float)*2;
						vk_vertexInputBindingDescription.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
					}
					VkVertexInputAttributeDescription vk_vertexInputAttributeDescription;
					{
						vk_vertexInputAttributeDescription.location = 0;
						vk_vertexInputAttributeDescription.binding = 0;
						vk_vertexInputAttributeDescription.format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
						vk_vertexInputAttributeDescription.offset = 0;
					}
					VkPipelineVertexInputStateCreateInfo vk_pipelineVertexInputStateCreateInfo;
					{
						vk_pipelineVertexInputStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
						vk_pipelineVertexInputStateCreateInfo.pNext = nullptr;
						vk_pipelineVertexInputStateCreateInfo.flags = 0;
						vk_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
						vk_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &vk_vertexInputBindingDescription;
						vk_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 1;
						vk_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = &vk_vertexInputAttributeDescription;
					}

					VkPipelineInputAssemblyStateCreateInfo vk_pipelineInputAssemblyStateCreateInfo;
					{
						vk_pipelineInputAssemblyStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
						vk_pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
						vk_pipelineInputAssemblyStateCreateInfo.flags = 0;
						vk_pipelineInputAssemblyStateCreateInfo.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
						vk_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
					}

					VkPipelineViewportStateCreateInfo vk_pipelineViewportStateCreateInfo;
					{
						vk_pipelineViewportStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
						vk_pipelineViewportStateCreateInfo.pNext = nullptr;
						vk_pipelineViewportStateCreateInfo.flags = 0;
						vk_pipelineViewportStateCreateInfo.viewportCount = viewports.size();
						vk_pipelineViewportStateCreateInfo.pViewports = viewports.data();
						vk_pipelineViewportStateCreateInfo.scissorCount = scissors.size();
						vk_pipelineViewportStateCreateInfo.pScissors = scissors.data();
					}

					VkPipelineRasterizationStateCreateInfo vk_pipelineRasterizationStateCreateInfo;
					{
						vk_pipelineRasterizationStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
						vk_pipelineRasterizationStateCreateInfo.pNext = nullptr;
						vk_pipelineRasterizationStateCreateInfo.flags = 0;
						vk_pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
						vk_pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
						vk_pipelineRasterizationStateCreateInfo.polygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
						vk_pipelineRasterizationStateCreateInfo.cullMode = VkCullModeFlagBits::VK_CULL_MODE_NONE;
						vk_pipelineRasterizationStateCreateInfo.frontFace = VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
						vk_pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
						vk_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
						vk_pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
						vk_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
						vk_pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
					}

					VkPipelineMultisampleStateCreateInfo vk_pipelineMultisampleStateCreateInfo;
					{
						vk_pipelineMultisampleStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
						vk_pipelineMultisampleStateCreateInfo.pNext = nullptr;
						vk_pipelineMultisampleStateCreateInfo.flags = 0;
						vk_pipelineMultisampleStateCreateInfo.rasterizationSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
						vk_pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
						vk_pipelineMultisampleStateCreateInfo.minSampleShading = 0;
						vk_pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
						vk_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
						vk_pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
					}

					VkPipelineDepthStencilStateCreateInfo vk_pipelineDepthStencilStateCreateInfo;
					{
						vk_pipelineDepthStencilStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
						vk_pipelineDepthStencilStateCreateInfo.pNext = nullptr;
						vk_pipelineDepthStencilStateCreateInfo.flags = 0;
						vk_pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_FALSE;
						vk_pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_FALSE;
						vk_pipelineDepthStencilStateCreateInfo.depthCompareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
						vk_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
						vk_pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
						vk_pipelineDepthStencilStateCreateInfo.front;
						{
							vk_pipelineDepthStencilStateCreateInfo.front.failOp = VkStencilOp::VK_STENCIL_OP_KEEP;
							vk_pipelineDepthStencilStateCreateInfo.front.passOp = VkStencilOp::VK_STENCIL_OP_KEEP;
							vk_pipelineDepthStencilStateCreateInfo.front.depthFailOp = VkStencilOp::VK_STENCIL_OP_KEEP;
							vk_pipelineDepthStencilStateCreateInfo.front.compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
							vk_pipelineDepthStencilStateCreateInfo.front.compareMask = 0;
							vk_pipelineDepthStencilStateCreateInfo.front.writeMask = 0;
							vk_pipelineDepthStencilStateCreateInfo.front.reference = 0;
						}
						vk_pipelineDepthStencilStateCreateInfo.back;
						{
							vk_pipelineDepthStencilStateCreateInfo.back.failOp = VkStencilOp::VK_STENCIL_OP_KEEP;
							vk_pipelineDepthStencilStateCreateInfo.back.passOp = VkStencilOp::VK_STENCIL_OP_KEEP;
							vk_pipelineDepthStencilStateCreateInfo.back.depthFailOp = VkStencilOp::VK_STENCIL_OP_KEEP;
							vk_pipelineDepthStencilStateCreateInfo.back.compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
							vk_pipelineDepthStencilStateCreateInfo.back.compareMask = 0;
							vk_pipelineDepthStencilStateCreateInfo.back.writeMask = 0;
							vk_pipelineDepthStencilStateCreateInfo.back.reference = 0;
						}
						vk_pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
						vk_pipelineDepthStencilStateCreateInfo.maxDepthBounds = 0.0f;
					}

					VkPipelineColorBlendAttachmentState vk_pipelineColorBlendAttachmentState;
					{
						vk_pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
						vk_pipelineColorBlendAttachmentState.srcColorBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;
						vk_pipelineColorBlendAttachmentState.dstColorBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;
						vk_pipelineColorBlendAttachmentState.colorBlendOp = VkBlendOp::VK_BLEND_OP_ADD;
						vk_pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;
						vk_pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;
						vk_pipelineColorBlendAttachmentState.alphaBlendOp = VkBlendOp::VK_BLEND_OP_ADD;
						vk_pipelineColorBlendAttachmentState.colorWriteMask = 
							VkColorComponentFlagBits::VK_COLOR_COMPONENT_R_BIT |
							VkColorComponentFlagBits::VK_COLOR_COMPONENT_G_BIT |
							VkColorComponentFlagBits::VK_COLOR_COMPONENT_B_BIT;
					}
					VkPipelineColorBlendStateCreateInfo vk_pipelineColorBlendStateCreateInfo;
					{
						vk_pipelineColorBlendStateCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
						vk_pipelineColorBlendStateCreateInfo.pNext = nullptr;
						vk_pipelineColorBlendStateCreateInfo.flags = 0;
						vk_pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
						vk_pipelineColorBlendStateCreateInfo.logicOp = VkLogicOp::VK_LOGIC_OP_CLEAR;
						vk_pipelineColorBlendStateCreateInfo.attachmentCount = 1;
						vk_pipelineColorBlendStateCreateInfo.pAttachments = &vk_pipelineColorBlendAttachmentState;
						vk_pipelineColorBlendStateCreateInfo.blendConstants;
						{
							vk_pipelineColorBlendStateCreateInfo.blendConstants[0] = 1.0f;
							vk_pipelineColorBlendStateCreateInfo.blendConstants[1] = 1.0f;
							vk_pipelineColorBlendStateCreateInfo.blendConstants[2] = 1.0f;
							vk_pipelineColorBlendStateCreateInfo.blendConstants[3] = 1.0f;
						}
					}

					VkGraphicsPipelineCreateInfo vk_graphicsPipelineCreateInfo;
					{
						vk_graphicsPipelineCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
						vk_graphicsPipelineCreateInfo.pNext = nullptr;
						vk_graphicsPipelineCreateInfo.flags = 0;
						vk_graphicsPipelineCreateInfo.stageCount = vk_pipelineShaderStageCreateInfos.size();
						vk_graphicsPipelineCreateInfo.pStages = vk_pipelineShaderStageCreateInfos.data();
						vk_graphicsPipelineCreateInfo.pVertexInputState = &vk_pipelineVertexInputStateCreateInfo;
						vk_graphicsPipelineCreateInfo.pInputAssemblyState = &vk_pipelineInputAssemblyStateCreateInfo;
						vk_graphicsPipelineCreateInfo.pTessellationState = VK_NULL_HANDLE;
						vk_graphicsPipelineCreateInfo.pViewportState = &vk_pipelineViewportStateCreateInfo;
						vk_graphicsPipelineCreateInfo.pRasterizationState = &vk_pipelineRasterizationStateCreateInfo;
						vk_graphicsPipelineCreateInfo.pMultisampleState = &vk_pipelineMultisampleStateCreateInfo;
						vk_graphicsPipelineCreateInfo.pDepthStencilState = &vk_pipelineDepthStencilStateCreateInfo;
						vk_graphicsPipelineCreateInfo.pColorBlendState = &vk_pipelineColorBlendStateCreateInfo;
						vk_graphicsPipelineCreateInfo.pDynamicState = nullptr;
						vk_graphicsPipelineCreateInfo.layout = pipelineLayout_ ? pipelineLayout_->GetHandle() : VK_NULL_HANDLE;
						vk_graphicsPipelineCreateInfo.renderPass = renderPass_ ? renderPass_->GetHandle() : VK_NULL_HANDLE;
						vk_graphicsPipelineCreateInfo.subpass = 0;
						vk_graphicsPipelineCreateInfo.basePipelineIndex = 0;
						vk_graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
					}

					return CreateGraphicsPipelines(
						device->GetHandle(),
						pipelineCache_->GetHandle(),
						Vector<VkGraphicsPipelineCreateInfo>({vk_graphicsPipelineCreateInfo}),
						nullptr)[0];
				}())
			{
			}
			inline ~GraphicPipeline()
			{
				DestroyPipeline(device->GetHandle(), handle, nullptr);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class Fence:
			public Device::Dependent
		{
		public:
			using Handle = VkFence;
		protected:
			const Handle handle;
		public:
			inline Fence(Device* device_):
				Dependent(device_),
				handle([&]()
				{
					VkFenceCreateInfo vk_fenceCreateInfo;
					{
						vk_fenceCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
						vk_fenceCreateInfo.pNext = nullptr;
						vk_fenceCreateInfo.flags = 0;
					}

					return CreateFence(device->GetHandle(), &vk_fenceCreateInfo, nullptr);
				}())
			{
			}
			inline ~Fence()
			{
				DestroyFence(device->GetHandle(), handle, nullptr);
			}
		public:
			inline void Reset()
			{
				ResetFence(device->GetHandle(), Vector<Handle>{handle});
			}
			inline void Wait(Size timeout_ = (Size)UINT64_MAX)
			{
				WaitForFences(device->GetHandle(), Vector<Handle>{handle}, true, timeout_);
			}
		public:
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
	}
}


#pragma region Instance
inline GreatVEngine::Vulkan::Instance::Instance(const Layer::CNames& layersCNames_, const Extension::CNames& extensionsCNames_):
	handle([&]()
	{
		VkApplicationInfo vk_applicationInfo;
		{
			vk_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			vk_applicationInfo.pNext = nullptr;
			vk_applicationInfo.pApplicationName = "Appication Name Here";
			vk_applicationInfo.applicationVersion = 0;
			vk_applicationInfo.pEngineName = "Engine Name Here";
			vk_applicationInfo.engineVersion = 0;
			vk_applicationInfo.apiVersion = GVE_VULKAN_VERSION;
		}
		VkInstanceCreateInfo vk_instanceCreateInfo;
		{
			vk_instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			vk_instanceCreateInfo.pNext = nullptr;
			vk_instanceCreateInfo.flags = 0;
			vk_instanceCreateInfo.pApplicationInfo = &vk_applicationInfo;
			vk_instanceCreateInfo.enabledLayerCount = layersCNames_.size();
			vk_instanceCreateInfo.ppEnabledLayerNames = layersCNames_.data();
			vk_instanceCreateInfo.enabledExtensionCount = extensionsCNames_.size();
			vk_instanceCreateInfo.ppEnabledExtensionNames = extensionsCNames_.data();
		}

		return CreateInstance(&vk_instanceCreateInfo, nullptr);
	}()),
	physicalDevices(PhysicalDevice::Enumerate(this))
{
	#if GVE_DEBUG
		auto vk_vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(handle, "vkCreateDebugReportCallbackEXT");

		VkDebugReportCallbackCreateInfoEXT vk_debugReportCallbackCreateInfoEXT;
		{
			vk_debugReportCallbackCreateInfoEXT.sType = VkStructureType::VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
			vk_debugReportCallbackCreateInfoEXT.pNext = nullptr;
			vk_debugReportCallbackCreateInfoEXT.flags = 
				VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
				VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT |
				VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
				VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT |
				VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT;
			vk_debugReportCallbackCreateInfoEXT.pfnCallback = DebugCallback;
			vk_debugReportCallbackCreateInfoEXT.pUserData = this;
		}
		ErrorTest(vk_vkCreateDebugReportCallbackEXT(handle, &vk_debugReportCallbackCreateInfoEXT, nullptr, &vk_debugReportCallbackEXT));
	#endif
}
inline GreatVEngine::Vulkan::Instance::~Instance()
{
	for(auto &physicalDevice : physicalDevices)
	{
		delete physicalDevice;
	}

#if GVE_DEBUG
	auto vk_vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(handle, "vkDestroyDebugReportCallbackEXT");
	vk_vkDestroyDebugReportCallbackEXT(handle, vk_debugReportCallbackEXT, nullptr);
#endif

	DestroyInstance(handle, nullptr);
}
#pragma endregion
#pragma region PhysicalDevice
inline GreatVEngine::Vector<GreatVEngine::Vulkan::PhysicalDevice*> GreatVEngine::Vulkan::PhysicalDevice::Enumerate(Instance* instance_)
{
	auto vk_physicalDevices = Vulkan::EnumeratePhysicalDevices(instance_->GetHandle());

	Vector<PhysicalDevice*> physicalDevices(vk_physicalDevices.size());
	{
		for(Size i = 0; i < vk_physicalDevices.size(); ++i)
		{
			physicalDevices[i] = new PhysicalDevice(instance_, vk_physicalDevices[i]);
		}
	}

	return std::move(physicalDevices);
}
#pragma endregion
#pragma region DeviceMemory
inline GreatVEngine::Vulkan::DeviceMemory::DeviceMemory(Device* device_, const Requirements& requirements_, const Property& property_):
	Dependent(device_),
	handle([&]()
	{
		VkMemoryAllocateInfo vk_memoryAllocateInfo;
		{
			vk_memoryAllocateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			vk_memoryAllocateInfo.pNext = nullptr;
			vk_memoryAllocateInfo.allocationSize = requirements_.size;
			vk_memoryAllocateInfo.memoryTypeIndex = GetCorrectMemoryType(
				device->GetPhysicalDevice()->GetMemoryProperties(),
				requirements_.memoryTypeBits,
				property_);
		}

		return AllocateMemory(device->GetHandle(), &vk_memoryAllocateInfo, nullptr);
	}()),
	size(requirements_.size)
{
}
inline GreatVEngine::Vulkan::DeviceMemory::DeviceMemory(BasicImage* image_, const Property& property_):
	DeviceMemory(image_->GetDevice(), GetImageMemoryRequirements(image_->GetDevice()->GetHandle(), image_->GetHandle()), property_)
{
}
inline GreatVEngine::Vulkan::DeviceMemory::DeviceMemory(Buffer* buffer_, const Property& property_) :
	DeviceMemory(buffer_->GetDevice(), GetBufferMemoryRequirements(buffer_->GetDevice()->GetHandle(), buffer_->GetHandle()), property_)
{
}
#pragma endregion
#pragma region SwapchainKHR
inline GreatVEngine::Size GreatVEngine::Vulkan::SwapchainKHR::AccuireNextImage(Fence* fence_, Semaphore* semaphore_, Size timeout_)
{
	uint32_t vk_swapchainImage;

	ErrorTest(vkAcquireNextImageKHR(
		device->GetHandle(),
		handle,
		timeout_,
		VK_NULL_HANDLE, // semaphore_ ? semaphore_->GetHandle() : VK_NULL_HANDLE
		fence_ ? fence_->GetHandle() : VK_NULL_HANDLE,
		&vk_swapchainImage));

	return vk_swapchainImage;
}
inline void GreatVEngine::Vulkan::SwapchainKHR::Preset(Queue* queue_, Size swapchainImage_)
{
	uint32_t vk_swapchainImage = swapchainImage_;

	VkResult vk_result;
	VkPresentInfoKHR vk_presentInfoKHR;
	{
		vk_presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		vk_presentInfoKHR.pNext = nullptr;
		vk_presentInfoKHR.waitSemaphoreCount = 0;
		vk_presentInfoKHR.pWaitSemaphores = nullptr;
		vk_presentInfoKHR.swapchainCount = 1;
		vk_presentInfoKHR.pSwapchains = &handle;
		vk_presentInfoKHR.pImageIndices = &vk_swapchainImage;
		vk_presentInfoKHR.pResults = &vk_result;
	}
	ErrorTest(vkQueuePresentKHR(queue_->GetHandle(), &vk_presentInfoKHR));
	ErrorTest(vk_result);
}
#pragma endregion
#pragma region Queue
inline void GreatVEngine::Vulkan::Queue::Submit(CommandBuffer* commandBuffers_)
{
	auto vk_commandBuffer = commandBuffers_->GetHandle();

	VkPipelineStageFlags vk_waitMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	VkSubmitInfo vk_submitInfo;
	{
		vk_submitInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SUBMIT_INFO;
		vk_submitInfo.pNext = nullptr;
		vk_submitInfo.waitSemaphoreCount = 0;
		vk_submitInfo.pWaitSemaphores = nullptr;
		vk_submitInfo.pWaitDstStageMask = &vk_waitMask;
		vk_submitInfo.commandBufferCount = 1;
		vk_submitInfo.pCommandBuffers = &vk_commandBuffer;
		vk_submitInfo.signalSemaphoreCount = 0;
		vk_submitInfo.pSignalSemaphores = nullptr;
	}

	QueueSubmit(handle, Vector<VkSubmitInfo>({vk_submitInfo}), VK_NULL_HANDLE);
}
#pragma endregion


#pragma region
#pragma endregion
