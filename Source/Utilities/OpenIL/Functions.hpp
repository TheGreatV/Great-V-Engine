#pragma region Include
#pragma once


#include <cstdint>


#include "Header.hpp"
#include "ErrorHandling.hpp"


#include <APIs/OpenGL/Wrapper.hpp>
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
			using Data = void*;
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
				RGB				= GL_RGB,
				RGBA			= GL_RGBA,
			};
			enum class Origin
			{
				LowerLeft,
				UpperLeft
			};
		public:
			inline static Reference<Image> Load2D(const Filename& filename) throw(...);
		protected:
			Size width;
			Size height;
			Size depth;
			Format format;
			ComponentType componentType;
			Size bytesPerPixel;
			Size bitsPerPixel;
			Size mipmapsCount;
			Origin originMode;
			Data data;
		public:
			inline Image(
				const Size&				width_,
				const Size&				height_,
				const Size&				depth_,
				const Format&			format_,
				const ComponentType&	componentType_,
				const Size&				bytesPerPixel_,
				const Size&				bitsPerPixel_,
				const Size&				mipmapsCount_,
				const Origin&			originMode_,
				const Data&				data_
			);
		public:
			inline ~Image();
		public:
			inline void Flip()
			{
				auto d = bytesPerPixel * width;
				auto t = malloc(d);

				for(Size z = 0; z < depth; ++z)
				{
					for(Size y = 0; y < height / 2; ++y)
					{
						auto m1 = (std::uint8_t*)data + ((y + z*height)*width) * bytesPerPixel;
						auto m2 = (std::uint8_t*)data + (((height - 1 - y) + z*height)*width) * bytesPerPixel;
						memcpy(t, m1, d);
						memcpy(m1, m2, d);
						memcpy(m2, t, d);
					}
				}

				free(t);
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
			inline Size GetMipmapsCount() const
			{
				return mipmapsCount;
			}
			inline Origin GetOriginMode() const
			{
				return originMode;
			}
			inline Data GetData() const
			{
				return data;
			}
		};

		inline OpenGL::Texture::Format GetOpenGLFormat(const Image::Format& format_)
		{
			switch(format_)
			{
				case GreatVEngine::OpenIL::Image::Format::RGB: return OpenGL::Texture::Format::RGB;
				case GreatVEngine::OpenIL::Image::Format::RGBA: return OpenGL::Texture::Format::RGBA;
				default: throw Exception("Unknown format");
			}
		}
		inline OpenGL::Texture::ComponentType GetOpenGLComponentType(const Image::ComponentType& componentType_)
		{
			switch(componentType_)
			{
				case GreatVEngine::OpenIL::Image::ComponentType::SInt8: return OpenGL::Texture::ComponentType::SInt8;
				case GreatVEngine::OpenIL::Image::ComponentType::UInt8: return OpenGL::Texture::ComponentType::UInt8;
				case GreatVEngine::OpenIL::Image::ComponentType::SInt16: return OpenGL::Texture::ComponentType::SInt16;
				case GreatVEngine::OpenIL::Image::ComponentType::UInt16: return OpenGL::Texture::ComponentType::UInt16;
				case GreatVEngine::OpenIL::Image::ComponentType::SInt32: return OpenGL::Texture::ComponentType::SInt32;
				case GreatVEngine::OpenIL::Image::ComponentType::UInt32: return OpenGL::Texture::ComponentType::UInt32;
				case GreatVEngine::OpenIL::Image::ComponentType::SFloat32: return OpenGL::Texture::ComponentType::SFloat32;
				case GreatVEngine::OpenIL::Image::ComponentType::Data8: return OpenGL::Texture::ComponentType::UInt8;
				case GreatVEngine::OpenIL::Image::ComponentType::Data16: return OpenGL::Texture::ComponentType::UInt16;
				case GreatVEngine::OpenIL::Image::ComponentType::Data32: return OpenGL::Texture::ComponentType::UInt32;
				default: throw Exception("Unknown component type");
			}
		}
	}
}


GreatVEngine::OpenIL::Initer GreatVEngine::OpenIL::Initer::initer;


inline GreatVEngine::Reference<GreatVEngine::OpenIL::Image> GreatVEngine::OpenIL::Image::Load2D(const Filename& filename)
{
	// initer.Initer::Initer();

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
				case GreatVEngine::OpenIL::Image::Format::RGB:
				case GreatVEngine::OpenIL::Image::Format::RGBA:
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

		Data data = ilData != nullptr ? ilData :
			throw Exception("Invalid data(null)");

		auto image = new Image(
			width,
			height,
			depth,
			format,
			componentType,
			bytesPerPixel,
			bitsPerPixel,
			mipmapsCount,
			originMode,
			data
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

		return MakeReference(image);
	}
	else
	{
		ErrorTest();
	}

	ilDeleteImage(handle); ErrorTest();

	return nullptr;
}
inline GreatVEngine::OpenIL::Image::Image(
	const Size& width_,
	const Size& height_,
	const Size& depth_,
	const Format& format_,
	const ComponentType& componentType_,
	const Size& bytesPerPixel_,
	const Size& bitsPerPixel_,
	const Size& mipmapsCount_,
	const Origin& originMode_,
	const Data& data_
):
	width(width_),
	height(height_),
	depth(depth_),
	format(format_),
	componentType(componentType_),
	bytesPerPixel(bytesPerPixel_),
	bitsPerPixel(bitsPerPixel_),
	mipmapsCount(mipmapsCount_),
	originMode(originMode_),
	data(malloc(bytesPerPixel*width*height*depth))
{
	memcpy(data, data_, bytesPerPixel*width*height*depth);
}
inline GreatVEngine::OpenIL::Image::~Image()
{
	free(data);
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

	ilTexImage(width, height, depth, ilComponentCount, (ILenum)format, (ILenum)componentType, data); ErrorTest();

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

