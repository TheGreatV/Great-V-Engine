#pragma region Include
#pragma once


#include <cstdint>


#include "Header.hpp"
#include "ErrorHandling.hpp"


// #include <APIs/OpenGL/Wrapper.hpp>
// #include <APIs/Vulkan/Wrapper.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace OpenIL
	{
		class Initer
		{
		private:
			static Initer initer;
		private:
			inline Initer()
			{
				if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION) throw Exception("Invalid IL version");
				if(iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION) throw Exception("Invalid ILU version");
				if(ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) throw Exception("Invalid ILUT version");

				ilInit();
				ErrorTest();
				iluInit();
				ErrorTest();
				ilutInit();
				ErrorTest();

				ilEnable(IL_FILE_OVERWRITE); ErrorTest();
				// ilSetInteger(IL_KEEP_DXTC_DATA, IL_TRUE);
				// ErrorTest();
				// ilutRenderer(ILUT_OPENGL);
				// ErrorTest();
				// ilutEnable(ILUT_GL_AUTODETECT_TEXTURE_TARGET);
				// ErrorTest();
				// ilutEnable(ILUT_OPENGL_CONV);
				// ErrorTest();
				// ilutEnable(ILUT_GL_USE_S3TC);
				// ErrorTest();
			}
		};

		class Image
		{
		public:
			using Handle = ILuint;
			using Filename = std::string;
			using Size = std::size_t;
			using RawData = std::uint8_t*;
			using Data = Vector<std::uint8_t>;
		public:
			enum class ComponentType: GLenum
			{
				SInt8			= GL_BYTE,
				UInt8			= GL_UNSIGNED_BYTE,
				SInt16			= GL_SHORT,
				UInt16			= GL_UNSIGNED_SHORT,
				SInt32			= GL_INT,
				UInt32			= GL_UNSIGNED_INT,
				SFloat32		= GL_FLOAT,
				Data8			= GL_2_BYTES,
				Data16			= GL_3_BYTES,
				Data32			= GL_4_BYTES,
				SFloat64		= GL_DOUBLE,
				Byte			= SInt8,
				UByte			= UInt8,
				Short			= SInt16,
				UShort			= UInt16,
				Int				= SInt32,
				UInt			= UInt32,
				Float			= SFloat32,
				Double			= SFloat64
			};
			enum class Format: GLenum
			{
				R				= IL_LUMINANCE,
				RGB				= IL_RGB,
				BGR				= IL_BGR,
				RGBA			= IL_RGBA,
				BGRA			= IL_BGRA,
			};
			enum class Origin
			{
				LowerLeft,
				UpperLeft
			};
			enum class Face
			{
				NegativeX = IL_CUBEMAP_NEGATIVEX,
				PositiveX = IL_CUBEMAP_POSITIVEX,
				NegativeY = IL_CUBEMAP_NEGATIVEY,
				PositiveY = IL_CUBEMAP_POSITIVEY,
				NegativeZ = IL_CUBEMAP_NEGATIVEZ,
				PositiveZ = IL_CUBEMAP_POSITIVEZ,
			};
		public:
			class Mipmap
			{
			protected:
				const Size width;
				const Size height;
				const Size depth;
				const Size bytesPerPixel;
				const Data data;
			public:
				inline Mipmap(
					const Size& width_,
					const Size& height_,
					const Size& depth_,
					const Size& bytesPerPixel_,
					Data&& data_):
					width(width_),
					height(height_),
					depth(depth_),
					bytesPerPixel(bytesPerPixel_),
					data(std::move(data_))
				{
				}
				inline Mipmap(const Mipmap&) = default;
				inline Mipmap(Mipmap&& mipmap_):
					width(mipmap_.width),
					height(mipmap_.height),
					depth(mipmap_.depth),
					bytesPerPixel(mipmap_.bytesPerPixel),
					data(std::move(mipmap_.data))
				{
				}
				inline ~Mipmap() = default;
			public:
				inline Mipmap& operator = (const Mipmap&) = delete;
			public:
				inline Size GetWidth() const
				{
					return width;
				}
				inline Size GetHeight() const
				{
					return height;
				}
				inline Size GetDepth() const
				{
					return depth;
				}
				inline Size GetBytesPerPixel() const
				{
					return bytesPerPixel;
				}
				inline const Data& GetData() const
				{
					return data;
				}
				inline RawData GetRawData() const
				{
					return (RawData)data.data();
				}
			public:
				inline void Flip()
				{
					auto p = data.data();
					auto d = bytesPerPixel * width;
					auto t = malloc(d);

					for(Size z = 0; z < depth; ++z)
					{
						for(Size y = 0; y < height / 2; ++y)
						{
							auto m1 = (std::uint8_t*)p + ((y + z*height)*width) * bytesPerPixel;
							auto m2 = (std::uint8_t*)p + (((height - 1 - y) + z*height)*width) * bytesPerPixel;
							memcpy(t, m1, d);
							memcpy(m1, m2, d);
							memcpy(m2, t, d);
						}
					}

					free(t);
				}
			};
			using Mipmaps = Vector<Mipmap>;
		public:
			using CubeImages = Dictionary<Face, Reference<Image>>;
		public:
			inline static Reference<Image> Load2D(const Filename& filename) throw(...);
			inline static CubeImages LoadCube(const Filename& filename) throw(...);
		protected:
			const Size width;
			const Size height;
			const Size depth;
			const Format format;
			const ComponentType componentType;
			const Size bytesPerPixel;
			const Size bitsPerPixel;
			const Origin originMode;
			Mipmaps mipmaps;
		public:
			inline Image(
				const Size&				width_,
				const Size&				height_,
				const Size&				depth_,
				const Format&			format_,
				const ComponentType&	componentType_,
				const Size&				bytesPerPixel_,
				const Size&				bitsPerPixel_,
				const Origin&			originMode_,
				Data&&					data_
			);
			inline Image(
				const Size&				width_,
				const Size&				height_,
				const Size&				depth_,
				const Format&			format_,
				const ComponentType&	componentType_,
				const Size&				bytesPerPixel_,
				const Size&				bitsPerPixel_,
				const Origin&			originMode_,
				Mipmaps&&				mipmaps_
			);
			inline ~Image() = default;
		public:
			inline Image& operator = (const Image&) = delete;
		public:
			inline void Flip()
			{
				for(auto &mipmap : mipmaps)
				{
					mipmap.Flip();
				}
			}
		public:
			inline void Save2D(const Filename & filename) const;
		public:
			inline Size GetWidth() const
			{
				return width;
			}
			inline Size GetHeight() const
			{
				return height;
			}
			inline Size GetDepth() const
			{
				return depth;
			}
			inline Format GetFormat() const
			{
				return format;
			}
			inline ComponentType GetComponentType() const
			{
				return componentType;
			}
			inline Size GetBytesPerPixel() const
			{
				return bytesPerPixel;
			}
			inline Size GetBitsPerPixel() const
			{
				return bitsPerPixel;
			}
			inline Origin GetOriginMode() const
			{
				return originMode;
			}
			inline const Data& GetData() const
			{
				return mipmaps[0].GetData();
				// return data;
			}
			inline RawData GetRawData() const
			{
				return mipmaps[0].GetRawData();
				// return (RawData)data.data();
			}
			inline const Mipmaps& GetMipmaps() const
			{
				return mipmaps;
			}
		public:
			inline const Mipmap& operator [] (const Size& i)
			{
				return mipmaps[i];
			}
		};

		inline Format GetFormat(const Image::Format& format_, const Image::ComponentType& componentType_)
		{
			switch(format_)
			{
				case GreatVEngine::OpenIL::Image::Format::R:
				{
					switch(componentType_)
					{
						case GreatVEngine::OpenIL::Image::ComponentType::UInt8: return Format::R8_UInt;
						case GreatVEngine::OpenIL::Image::ComponentType::UInt16: return Format::R16_UInt;
						case GreatVEngine::OpenIL::Image::ComponentType::SFloat32: return Format::R32_SFloat;
						default: throw Exception("Unsupported component type");
					}
				} break;
				case GreatVEngine::OpenIL::Image::Format::RGB:
				{
					switch(componentType_)
					{
						case GreatVEngine::OpenIL::Image::ComponentType::UInt8: return Format::R8G8B8_UInt;
						case GreatVEngine::OpenIL::Image::ComponentType::UInt16: return Format::R16G16B16_UInt;
						case GreatVEngine::OpenIL::Image::ComponentType::SFloat32: return Format::R32G32B32_SFloat;
						default: throw Exception("Unsupported component type");
					}
				} break;
				case GreatVEngine::OpenIL::Image::Format::BGR:
				{
					switch(componentType_)
					{
						case GreatVEngine::OpenIL::Image::ComponentType::UInt8: return Format::B8G8R8_UInt;
						default: throw Exception("Unsupported component type");
					}
				} break;
				case GreatVEngine::OpenIL::Image::Format::RGBA:
				{
					switch(componentType_)
					{
						case GreatVEngine::OpenIL::Image::ComponentType::UInt8: return Format::R8G8B8A8_UInt;
						case GreatVEngine::OpenIL::Image::ComponentType::UInt16: return Format::R16G16B16A16_UInt;
						case GreatVEngine::OpenIL::Image::ComponentType::SFloat32: return Format::R32G32B32A32_SFloat;
						default: throw Exception("Unsupported component type");
					}
				} break;
				case GreatVEngine::OpenIL::Image::Format::BGRA:
				{
					switch(componentType_)
					{
						case GreatVEngine::OpenIL::Image::ComponentType::UInt8: return Format::B8G8R8A8_UInt;
						default: throw Exception("Unsupported component type");
					}
				} break;
				default: throw Exception("Unsupported format");
			}
		}
		inline Image::Format GetFormat(const Format& format_)
		{
			switch(format_)
			{
				case GreatVEngine::Format::R8G8B8_UInt:
				case GreatVEngine::Format::R32G32B32_SFloat:
					return Image::Format::RGB;
				case GreatVEngine::Format::R8G8B8A8_UInt:
				case GreatVEngine::Format::R32G32B32A32_SFloat:
					return Image::Format::RGBA;
				case GreatVEngine::Format::B8G8R8_UInt:
					return Image::Format::BGR;
				case GreatVEngine::Format::B8G8R8A8_UInt:
					return Image::Format::BGRA;
				default: throw Exception("Unsupported format");
			}
		}
		inline Image::ComponentType GetComponentType(const Format& format_)
		{
			switch(format_)
			{
				case GreatVEngine::Format::R8G8B8_UInt: 
				case GreatVEngine::Format::R8G8B8A8_UInt:
				case GreatVEngine::Format::B8G8R8_UInt:
				case GreatVEngine::Format::B8G8R8A8_UInt:
					return Image::ComponentType::UInt8;
				case GreatVEngine::Format::R32G32B32_SFloat: 
				case GreatVEngine::Format::R32G32B32A32_SFloat:
					return Image::ComponentType::SFloat32;
				default: throw Exception("Unsupported format");
			}
		}
	}
}


// // #ifndef __CPP_SHIT_OPENIL__
// // #define __CPP_SHIT_OPENIL__ 1
// GreatVEngine::OpenIL::Initer GreatVEngine::OpenIL::Initer::initer;
// // #endif


inline GreatVEngine::Reference<GreatVEngine::OpenIL::Image> GreatVEngine::OpenIL::Image::Load2D(const Filename& filename)
{
	Handle handle = ilGenImage(); ErrorTest();
	ilBindImage(handle); ErrorTest();

	if(ilLoadImage(filename.c_str()))
	{
		ErrorTest();

		auto ilWidth = ilGetInteger(IL_IMAGE_WIDTH); ErrorTest();
		auto ilHeight = ilGetInteger(IL_IMAGE_HEIGHT); ErrorTest();
		auto ilDepth = ilGetInteger(IL_IMAGE_DEPTH); ErrorTest();
		auto ilFormat = ilGetInteger(IL_IMAGE_FORMAT); ErrorTest(); // GL_RGBA
		auto ilType = ilGetInteger(IL_IMAGE_TYPE); ErrorTest();
		auto ilBytes = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); ErrorTest();
		auto ilBits = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL); ErrorTest();
		auto ilMipmap = ilGetInteger(IL_NUM_MIPMAPS); ErrorTest();
		auto ilOriginMode = ilGetInteger(IL_IMAGE_ORIGIN); ErrorTest();	// IL_ORIGIN_LOWER_LEFT, IL_ORIGIN_UPPER_LEFT
		auto ilDataSize = ilGetInteger(IL_IMAGE_SIZE_OF_DATA); ErrorTest();
		auto ilData = ilGetData(); ErrorTest();

		Size width = ilWidth > 0 ? ilWidth :
			throw Exception("Invalid width: " + std::to_string(ilWidth));

		Size height = ilHeight > 0 ? ilHeight :
			throw Exception("Invalid height: " + std::to_string(ilHeight));

		Size depth = ilDepth > 0 ? ilDepth :
			throw Exception("Invalid depth: " + std::to_string(ilDepth));

		Format format = (Format)ilFormat;
		{
			switch(format)
			{
				case GreatVEngine::OpenIL::Image::Format::R:
				case GreatVEngine::OpenIL::Image::Format::RGB:
				case GreatVEngine::OpenIL::Image::Format::BGR:
				case GreatVEngine::OpenIL::Image::Format::RGBA:
				case GreatVEngine::OpenIL::Image::Format::BGRA:
					break;
				default:
					throw Exception("Invalid component format: " + (decltype(ilFormat))format);
			}
		}

		ComponentType componentType = (ComponentType)ilType;
		{
			switch(componentType)
			{
				case GreatVEngine::OpenIL::Image::ComponentType::SInt8:
				case GreatVEngine::OpenIL::Image::ComponentType::UInt8:
				case GreatVEngine::OpenIL::Image::ComponentType::SInt16:
				case GreatVEngine::OpenIL::Image::ComponentType::UInt16:
				case GreatVEngine::OpenIL::Image::ComponentType::SInt32:
				case GreatVEngine::OpenIL::Image::ComponentType::UInt32:
				case GreatVEngine::OpenIL::Image::ComponentType::SFloat32:
				case GreatVEngine::OpenIL::Image::ComponentType::Data8:
				case GreatVEngine::OpenIL::Image::ComponentType::Data16:
				case GreatVEngine::OpenIL::Image::ComponentType::Data32:
				case GreatVEngine::OpenIL::Image::ComponentType::SFloat64:
					break;
				default:
					throw Exception("Invalid component type: " + (decltype(ilType))componentType);
			}
		}

		Size bytesPerPixel = ilBytes >= 1 ? ilBytes :
			throw Exception("Invalid bytes per pixel: " + std::to_string(ilBytes));

		Size bitsPerPixel = ilBits >= 1 ? ilBits :
			throw Exception("Invalid bits per pixel: " + std::to_string(ilBits));

		Size mipmapsCount = ilMipmap >= 0 ? ilMipmap :
			throw Exception("Invalid mipmaps count: " + std::to_string(ilMipmap));

		Origin originMode = ilOriginMode == IL_ORIGIN_LOWER_LEFT ? Origin::LowerLeft :
			ilOriginMode == IL_ORIGIN_UPPER_LEFT ? Origin::UpperLeft :
			throw Exception("Invalid origin mode: " + std::to_string(ilOriginMode));

		Size dataSize = ilDataSize >= 0 ?
			(Size)ilDataSize :
			throw Exception("Data size is invalid");

		RawData data = ilData != nullptr ? ilData :
			throw Exception("Invalid data(null)");

		auto image = new Image(
			width,
			height,
			depth,
			format,
			componentType,
			bytesPerPixel,
			bitsPerPixel,
			// mipmapsCount,
			originMode,
			Data(data, data + dataSize)
		);

		try
		{
			ilDeleteImage(handle); ErrorTest();
		}
		catch(...)
		{
			delete image;

			throw;
		}

		return WrapReference(image);
	}
	else
	{
		ErrorTest();
	}

	ilDeleteImage(handle); ErrorTest();

	throw Exception("Failet to load image: " + filename);
	// return nullptr;
}
inline GreatVEngine::OpenIL::Image::CubeImages GreatVEngine::OpenIL::Image::LoadCube(const Filename& filename)
{
	Handle handle = ilGenImage(); ErrorTest();
	ilBindImage(handle); ErrorTest();

	if(ilLoadImage(filename.c_str()))
	{
		ErrorTest();

		CubeImages cubeImages;

		for(Size i = 0; i < 6; ++i) // iterate all faces
		{
			ilBindImage(handle); ErrorTest();
			ilActiveFace(i); ErrorTest();
			ilActiveMipmap(0); ErrorTest();

			auto ilFace = ilGetInteger(IL_IMAGE_CUBEFLAGS);
			auto ilWidth = ilGetInteger(IL_IMAGE_WIDTH); ErrorTest();
			auto ilHeight = ilGetInteger(IL_IMAGE_HEIGHT); ErrorTest();
			auto ilDepth = ilGetInteger(IL_IMAGE_DEPTH); ErrorTest();
			auto ilFormat = ilGetInteger(IL_IMAGE_FORMAT); ErrorTest(); // GL_RGBA
			auto ilType = ilGetInteger(IL_IMAGE_TYPE); ErrorTest();
			auto ilBytes = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); ErrorTest();
			auto ilBits = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL); ErrorTest();
			auto ilMipmap = ilGetInteger(IL_NUM_MIPMAPS); ErrorTest();
			auto ilOriginMode = ilGetInteger(IL_IMAGE_ORIGIN); ErrorTest();	// IL_ORIGIN_LOWER_LEFT, IL_ORIGIN_UPPER_LEFT

			Face face = (Face)ilFace;
			{
				switch(face)
				{
					case GreatVEngine::OpenIL::Image::Face::NegativeX:
					case GreatVEngine::OpenIL::Image::Face::PositiveX:
					case GreatVEngine::OpenIL::Image::Face::NegativeY:
					case GreatVEngine::OpenIL::Image::Face::PositiveY:
					case GreatVEngine::OpenIL::Image::Face::NegativeZ:
					case GreatVEngine::OpenIL::Image::Face::PositiveZ:
						break;
					default:
						throw Exception("Invalid face: " + std::to_string(ilFace));
				}
			}
			Size width = ilWidth > 0 ? ilWidth :
				throw Exception("Invalid width: " + std::to_string(ilWidth));
			Size height = ilHeight > 0 ? ilHeight :
				throw Exception("Invalid height: " + std::to_string(ilHeight));
			Size depth = ilDepth > 0 ? ilDepth :
				throw Exception("Invalid depth: " + std::to_string(ilDepth));
			Format format = (Format)ilFormat;
			{
				switch(format)
				{
					case GreatVEngine::OpenIL::Image::Format::RGB:
					case GreatVEngine::OpenIL::Image::Format::BGR:
					case GreatVEngine::OpenIL::Image::Format::RGBA:
					case GreatVEngine::OpenIL::Image::Format::BGRA:
						break;
					default:
						throw Exception("Invalid component format: " + (decltype(ilFormat))format);
				}
			}
			ComponentType componentType = (ComponentType)ilType;
			{
				switch(componentType)
				{
					case GreatVEngine::OpenIL::Image::ComponentType::SInt8:
					case GreatVEngine::OpenIL::Image::ComponentType::UInt8:
					case GreatVEngine::OpenIL::Image::ComponentType::SInt16:
					case GreatVEngine::OpenIL::Image::ComponentType::UInt16:
					case GreatVEngine::OpenIL::Image::ComponentType::SInt32:
					case GreatVEngine::OpenIL::Image::ComponentType::UInt32:
					case GreatVEngine::OpenIL::Image::ComponentType::SFloat32:
					case GreatVEngine::OpenIL::Image::ComponentType::Data8:
					case GreatVEngine::OpenIL::Image::ComponentType::Data16:
					case GreatVEngine::OpenIL::Image::ComponentType::Data32:
					case GreatVEngine::OpenIL::Image::ComponentType::SFloat64:
						break;
					default:
						throw Exception("Invalid component type: " + (decltype(ilType))componentType);
				}
			}
			Size bytesPerPixel = ilBytes >= 1 ? ilBytes :
				throw Exception("Invalid bytes per pixel: " + std::to_string(ilBytes));
			Size bitsPerPixel = ilBits >= 1 ? ilBits :
				throw Exception("Invalid bits per pixel: " + std::to_string(ilBits));
			Size mipmapsCount = ilMipmap >= 0 ? ilMipmap :
				throw Exception("Invalid mipmaps count: " + std::to_string(ilMipmap));
			Origin originMode = ilOriginMode == IL_ORIGIN_LOWER_LEFT ? Origin::LowerLeft :
				ilOriginMode == IL_ORIGIN_UPPER_LEFT ? Origin::UpperLeft :
				throw Exception("Invalid origin mode: " + std::to_string(ilOriginMode));


			Mipmaps mipmaps;
			{
				mipmaps.reserve(mipmapsCount);
			}

			for(Size mipmap = 0; mipmap < mipmapsCount; ++mipmap)
			{
				ilBindImage(handle); ErrorTest();
				ilActiveFace(i); ErrorTest();
				ilActiveMipmap(mipmap); ErrorTest();

				auto ilWidth = ilGetInteger(IL_IMAGE_WIDTH); ErrorTest();
				auto ilHeight = ilGetInteger(IL_IMAGE_HEIGHT); ErrorTest();
				auto ilDepth = ilGetInteger(IL_IMAGE_DEPTH); ErrorTest();
				auto ilDataSize = ilGetInteger(IL_IMAGE_SIZE_OF_DATA); ErrorTest();
				auto ilData = ilGetData(); ErrorTest();

				Size width = ilWidth > 0 ? ilWidth :
					throw Exception("Invalid width: " + std::to_string(ilWidth));
				Size height = ilHeight > 0 ? ilHeight :
					throw Exception("Invalid height: " + std::to_string(ilHeight));
				Size depth = ilDepth > 0 ? ilDepth :
					throw Exception("Invalid depth: " + std::to_string(ilDepth));
				Size dataSize = ilDataSize >= 0 ?
					(Size)ilDataSize :
					throw Exception("Data size is invalid");
				RawData data = ilData != nullptr ? ilData :
					throw Exception("Invalid data(null)");

				mipmaps.push_back(Mipmap(
					width,
					height,
					depth,
					bytesPerPixel,
					Data(data, data + dataSize)));
			}

			auto image = new Image(
				width,
				height,
				depth,
				format,
				componentType,
				bytesPerPixel,
				bitsPerPixel,
				originMode,
				std::move(mipmaps));

			cubeImages[face] = WrapReference(image);
		}

		ilDeleteImage(handle); ErrorTest();

		return cubeImages;
	}
	else
	{
		ErrorTest();
	}

	ilDeleteImage(handle); ErrorTest();

	throw Exception("Failet to load image: " + filename);
	// return CubeImages();
}
inline GreatVEngine::OpenIL::Image::Image(
	const Size& width_,
	const Size& height_,
	const Size& depth_,
	const Format& format_,
	const ComponentType& componentType_,
	const Size& bytesPerPixel_,
	const Size& bitsPerPixel_,
	const Origin& originMode_,
	Data&& data_
):
	width(width_),
	height(height_),
	depth(depth_),
	format(format_),
	componentType(componentType_),
	bytesPerPixel(bytesPerPixel_),
	bitsPerPixel(bitsPerPixel_),
	originMode(originMode_),
	mipmaps()
{
	mipmaps.push_back(Mipmap(width, height, depth, bytesPerPixel, std::move(data_)));
}
inline GreatVEngine::OpenIL::Image::Image(
	const Size& width_,
	const Size& height_,
	const Size& depth_,
	const Format& format_,
	const ComponentType& componentType_,
	const Size& bytesPerPixel_,
	const Size& bitsPerPixel_,
	const Origin& originMode_,
	Mipmaps&& mipmaps_
):
	width(width_),
	height(height_),
	depth(depth_),
	format(format_),
	componentType(componentType_),
	bytesPerPixel(bytesPerPixel_),
	bitsPerPixel(bitsPerPixel_),
	originMode(originMode_),
	mipmaps(std::move(mipmaps_))
{
}

inline void GreatVEngine::OpenIL::Image::Save2D(const Filename & filename) const
{
	Handle handle = ilGenImage(); ErrorTest();
	ilBindImage(handle); ErrorTest();

	ILubyte ilComponentCount;
	{
		switch(format)
		{
			case GreatVEngine::OpenIL::Image::Format::RGB: ilComponentCount = 3; break;
			case GreatVEngine::OpenIL::Image::Format::RGBA: ilComponentCount = 4; break;
			default: throw Exception("Can't calculate component count");
		}
	}

	ilTexImage(width, height, depth, ilComponentCount, (ILenum)format, (ILenum)componentType, (void*)mipmaps[0].GetRawData()); ErrorTest();

	// ilSetInteger(IL_IMAGE_WIDTH, (ILint)width); ErrorTest();
	// ilSetInteger(IL_IMAGE_HEIGHT, (ILint)height); ErrorTest();
	// ilSetInteger(IL_IMAGE_DEPTH, (ILint)depth); ErrorTest();
	// ilSetInteger(IL_IMAGE_FORMAT, (ILint)format); ErrorTest();
	// ilSetInteger(IL_IMAGE_TYPE, (ILint)componentType); ErrorTest();
	// ilSetInteger(IL_IMAGE_BYTES_PER_PIXEL, (ILint)bytesPerPixel); ErrorTest();
	// ilSetInteger(IL_IMAGE_BITS_PER_PIXEL, (ILint)bitsPerPixel); ErrorTest();
	// ilSetInteger(IL_NUM_MIPMAPS, (ILint)mipmapsCount); ErrorTest();
	// ilSetInteger(IL_IMAGE_ORIGIN, (ILint)originMode); ErrorTest();	// IL_ORIGIN_LOWER_LEFT, IL_ORIGIN_UPPER_LEFT
	// ilSetData(data); ErrorTest();

	ilSaveImage(filename.c_str()); ErrorTest();

	ilBindImage(0); ErrorTest();
	ilDeleteImage(handle); ErrorTest();
}

