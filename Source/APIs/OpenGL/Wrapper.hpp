#pragma region Include
#pragma once


#include "ErrorHandling.hpp"
#include "Functions.hpp"


#include <Infrastructure/Infrastructure.hpp>
#pragma endregion


namespace GreatVEngine
{
	namespace OpenGL
	{
		class Shader;
		class Program;
		
		class Texture;

		class Buffer;
		namespace Buffers
		{
			class Array;
			class Index;
			class Attribute;
		}

		class Context:
			public Graphics::Context
		{
		protected:
			Buffers::Array* currentBufferArray = nullptr;
			Buffers::Index* currentBufferIndex = nullptr;
			Buffers::Attribute* currentBufferAttribute = nullptr;
			Vector<Texture*> currentTexture;
			Program* currentProgram = nullptr;
		public:
			inline Context(const Size& texturesCount_):
				currentTexture(texturesCount_, nullptr)
			{
			}
			inline Context(const Context&) = delete;
			virtual ~Context() override = 0;
		public:
			inline Context& operator = (const Context&) = delete;
		public:
			virtual void Set() override = 0;
			virtual void Reset() override = 0;
		public:
			inline void SetBufferArray(Buffers::Array* buffer_);
			inline void ResetBufferArray(Buffers::Array* buffer_);
			inline Buffers::Array* GetBufferArray() const
			{
				return currentBufferArray;
			}
		public:
			inline void SetBufferAttribute(Buffers::Attribute* buffer_);
			inline void ResetBufferAttribute(Buffers::Attribute* buffer_);
			inline Buffers::Attribute* GetBufferAttribute() const
			{
				return currentBufferAttribute;
			}
		public:
			inline void SetBufferIndex(Buffers::Index* buffer_);
			inline void ResetBufferIndex(Buffers::Index* buffer_);
			inline Buffers::Index* GetBufferIndex() const
			{
				return currentBufferIndex;
			}
		public:
			inline void SetProgram(Program* buffer_);
			inline void ResetProgram(Program* buffer_);
			inline Program* GetProgram() const
			{
				return currentProgram;
			}
		public:
			inline Size GetTexturesCount() const
			{
				return currentTexture.size();
			}
			inline void SetTexture(const Size& slot_, Texture* texture_);
			inline void ResetTexture(const Size& slot_, Texture* texture_);
			inline Texture* GetTexture(const Size& slot_) const
			{
				return currentTexture[slot_];
			}
		};
		class ContextDependent
		{
		protected:
			Context*const context;
		public:
			ContextDependent(Context* context_):
				context(context_)
			{
			}
		public:
			Context* GetContext() const
			{
				return context;
			}
		};

		class Shader
		{
		public:
			using Handle = GLuint;
			using Data = String;
		public:
			enum class Type: GLenum
			{
				Vertex = GL_VERTEX_SHADER,
#if GVE_OPENGL_VERSION >= 410
				TessellationControl = GL_TESS_CONTROL_SHADER,
				TessellationEvaluation = GL_TESS_EVALUATION_SHADER,
#endif
#if GVE_OPENGL_VERSION >= 330
				Geometry = GL_GEOMETRY_SHADER,
#endif
				Fragment = GL_FRAGMENT_SHADER,
#if GVE_OPENGL_VERSION >= 430
				Compute = GL_COMPUTE_SHADER,
#endif
			};
		protected:
			const Type type;
			const Handle handle;
		public:
			inline Shader(const Type& type_, const Data& data_):
				type(type_),
				handle(glCreateShader((GLenum)type))
			{
				DebugTest();

				auto data = data_.data();
				GLint length = data_.size();
				glShaderSource(handle, 1, &data, &length); DebugTest();

				glCompileShader(handle);

				GLint compileResult;
				glGetShaderiv(handle, GL_COMPILE_STATUS, &compileResult);
				if(compileResult != GL_TRUE)
				{
					GLint errorCodeLength;
					glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &errorCodeLength);
					char *buffer = (char*)malloc(errorCodeLength);
					int len;
					glGetShaderInfoLog(handle, errorCodeLength, &len, buffer);
					String errorText(buffer, errorCodeLength);
					free(buffer);

					throw Exception(errorText);
				}
			}
			inline Shader(const Shader&) = delete;
			inline ~Shader()
			{
				glDeleteShader(handle); DebugTest();
			}
		public:
			inline Type GetType() const
			{
				return type;
			}
			inline Handle GetHandle() const
			{
				return handle;
			}
		};
		class Program:
			public ContextDependent
		{
		public:
			using Handle = GLuint;
			using Attribute = GLint;
			using AttributeName = String;
			using Uniform = GLint;
			using UniformName = String;
		protected:
			friend Context;
			const Handle handle;
		public:
			inline Program(Context* context_, const Initializer<Shader*>& shaders):
				ContextDependent(context_),
				handle(glCreateProgram())
			{
				DebugTest();

				for(auto &shader : shaders)
				{
					glAttachShader(handle, shader->GetHandle());
					DebugTest();
				}

				glLinkProgram(handle);

				GLint result;
				glGetProgramiv(handle, GL_LINK_STATUS, &result);
				if(result != GL_TRUE)
				{
					GLint errorCodeLength;
					glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &errorCodeLength);
					char *buffer = (char*)malloc(errorCodeLength);
					int len = errorCodeLength;
					glGetProgramInfoLog(handle, errorCodeLength, &len, buffer);
					buffer[errorCodeLength] = '\0';
					String errorText(buffer, errorCodeLength);
					free(buffer);

					throw Exception(errorText);
				}
			}
			inline Program(const Program&) = delete;
			inline ~Program()
			{
				glDeleteProgram(handle); DebugTest();
			}
		protected:
			inline void DebugIsActiveTest() const
			{
#if GVE_DEBUG
				if(context->GetProgram() != this)
				{
					throw Exception("Program is not set");
				}
#endif
			}
		public:
			inline void Set()
			{
				context->SetProgram(this);
			}
			inline void Reset()
			{
				context->ResetProgram(this);
			}
		public:
			inline Attribute GetAttribute(const AttributeName& attributeName_) const
			{
				DebugIsActiveTest();
				Attribute attribute = glGetAttribLocation(handle, attributeName_.c_str()); DebugTest();
				return attribute;
			}
			inline Uniform GetUniform(const UniformName& uniformName_) const
			{
				DebugIsActiveTest();

				auto uniform = glGetUniformLocation(handle, uniformName_.c_str());
				DebugTest();
				return uniform;
			}
			inline void SetMat4(const Uniform& uniform_, const Mat4& mat_)
			{
				glUniformMatrix4fv(uniform_, 1, GL_FALSE, (const GLfloat*)&mat_);
			}
			inline void SetMat4(const UniformName& uniformName_, const Mat4& mat_)
			{
				auto uniform = GetUniform(uniformName_);
				if(uniform != -1)
				{
					SetMat4(uniform, mat_);
				}
			}
		};

		class Texture:
			public ContextDependent
		{
		public:
			using Handle = GLuint;
			using Data = void*;
		public:
			enum class Type: GLenum
			{
				D1								= GL_TEXTURE_1D,
				D1Array							= GL_TEXTURE_1D_ARRAY,
				D2								= GL_TEXTURE_2D,
				D2Array							= GL_TEXTURE_2D_ARRAY,
				D3								= GL_TEXTURE_3D,
				Cube							= GL_TEXTURE_CUBE_MAP,
				CubeArray						= GL_TEXTURE_CUBE_MAP_ARRAY
			};
			enum class InternalFormat: GLint
			{
				R8								= GL_R8,
				R16								= GL_R16,
				R16F							= GL_R16F,
				R32F							= GL_R32F,
				RG8								= GL_RG8,
				RG16							= GL_RG16,
				RG16F							= GL_RG16F,
				RG32F							= GL_RG32F,
				RGB8							= GL_RGB8,
				RGB16							= GL_RGB16,
				RGB16F							= GL_RGB16F,
				RGB32F							= GL_RGB32F,
				RGBA8							= GL_RGBA8,
				RGBA16							= GL_RGBA16,
				RGBA16F							= GL_RGBA16F,
				RGBA32F							= GL_RGBA32F,

				Depth16							= GL_DEPTH_COMPONENT16,
				Depth24							= GL_DEPTH_COMPONENT24,
				Depth32							= GL_DEPTH_COMPONENT32,
				Depth32F						= GL_DEPTH_COMPONENT32F,

				Depth24Stencil8					= GL_DEPTH24_STENCIL8,
				Depth32FStencil8				= GL_DEPTH32F_STENCIL8
			};
			enum class Format: GLenum
			{
				R								= GL_RED,
				G								= GL_GREEN,
				B								= GL_BLUE,
				A								= GL_ALPHA,
				L								= GL_LUMINANCE,
				RG								= GL_RG,
				RGB								= GL_RGB,
				BGR								= GL_BGR,
				RGBA							= GL_RGBA,
				BGRA							= GL_BGRA,
				Depth							= GL_DEPTH_COMPONENT,
				DepthStencil					= GL_DEPTH_STENCIL
			};
			enum class ComponentType: GLenum
			{
				SInt8							= GL_BYTE,
				UInt8							= GL_UNSIGNED_BYTE,
				SInt16							= GL_SHORT,
				UInt16							= GL_UNSIGNED_SHORT,
				SInt32							= GL_INT,
				UInt32							= GL_UNSIGNED_INT,
				SFloat16						= GL_HALF_FLOAT,
				SFloat32						= GL_FLOAT,
				UInt24_8						= GL_UNSIGNED_INT_24_8,

				Byte							= SInt8,
				UByte							= UInt8,
				Short							= SInt16,
				UShort							= UInt16,
				Int								= SInt32,
				UInt							= UInt32,
				Half							= SFloat16,
				Float							= SFloat32,
			};
			enum class Wrap: GLint
			{
				Clamp							= GL_CLAMP_TO_EDGE,
				Repeat							= GL_REPEAT,
				Border							= GL_CLAMP_TO_BORDER,
				Mirror							= GL_MIRRORED_REPEAT
			};
			struct Filter
			{
			public:
				enum class Minification: GLint
				{
					Off						= GL_NEAREST,
					Linear					= GL_LINEAR,
					MipmapNearestNearest	= GL_NEAREST_MIPMAP_NEAREST,
					MipmapLinearNearest		= GL_LINEAR_MIPMAP_NEAREST,
					MipmapNearestLinear		= GL_NEAREST_MIPMAP_LINEAR,
					MipmapLinearLinear		= GL_LINEAR_MIPMAP_LINEAR,
					Mipmap					= MipmapLinearLinear
				};
				enum class Magnification: GLint
				{
					Off						= GL_NEAREST,
					Linear					= GL_LINEAR
				};
			public:
				typedef Minification		Min;
				typedef Magnification		Mag;
			public:
				Minification				min = Minification::Mipmap;
				Magnification				mag = Magnification::Linear;
			public:
				inline Filter() = default;
				inline Filter(const Filter&) = default;
				inline Filter(const Minification& min_,const Magnification& mag_):
					min(min_),mag(mag_)
				{
				}
			public:
				static const Filter			Off;
				static const Filter			Linear;
				static const Filter			Mipmap;
			};
		protected:
			const Size width, height, depth;
			const Type type;
			const InternalFormat internalFormat;
			const ComponentType componentType;
			Wrap wrap;
			Filter filter;
			const Handle handle;
		public:
			inline Texture(
				Context* context_,
				const Size& width_, const Size& height_, const Size& depth_,
				const Type& type_, const InternalFormat& internalFormat_, const Format& format_, const ComponentType& componentType_,
				const Wrap& wrap_, const Filter& filter_, Data data_):
				ContextDependent(context_),
				width(width_), height(height_), depth(depth_),
				type(type_), internalFormat(internalFormat_), componentType(componentType_),
				wrap(wrap_), filter(filter_),
				handle([&]()
				{
					Handle handle;

					glGenTextures(1, &handle); DebugTest();
					context->SetTexture(0, this);
					//glBindTexture((GLenum)type, handle); DebugTest();

					glTexParameteri((GLenum)type, GL_TEXTURE_MIN_FILTER, (GLint)filter.min); DebugTest();
					glTexParameteri((GLenum)type, GL_TEXTURE_MAG_FILTER, (GLint)filter.mag); DebugTest();

					glTexParameteri((GLenum)type, GL_TEXTURE_WRAP_S, (GLint)wrap); DebugTest();
					glTexParameteri((GLenum)type, GL_TEXTURE_WRAP_T, (GLint)wrap); DebugTest();
					glTexParameteri((GLenum)type, GL_TEXTURE_WRAP_R, (GLint)wrap); DebugTest();

					// glTexParameterfv((GLenum)type, GL_TEXTURE_BORDER_COLOR, );

					switch(type)
					{
						case Type::D1:
						{
							glTexImage1D((GLenum)type, 0, (GLint)internalFormat, width, 0, (GLenum)format_, (GLenum)componentType, data_); DebugTest();
						} break;
						case Type::D2:
						{
							glTexImage2D((GLenum)type, 0, (GLint)internalFormat, width, height, 0, (GLenum)format_, (GLenum)componentType, data_); DebugTest();
						} break;
						case Type::D3:
						{
							glTexImage3D((GLenum)type, 0, (GLint)internalFormat, width, height, depth, 0, (GLenum)format_, (GLenum)componentType, data_); DebugTest();
						} break;
						case Type::Cube:
						{
							glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, (GLint)internalFormat, width, width, 0, (GLenum)format_, (GLenum)componentType, data_ ? ((void**)data_)[0] : NULL); DebugTest();
							glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, (GLint)internalFormat, width, width, 0, (GLenum)format_, (GLenum)componentType, data_ ? ((void**)data_)[1] : NULL); DebugTest();
							glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, (GLint)internalFormat, width, width, 0, (GLenum)format_, (GLenum)componentType, data_ ? ((void**)data_)[2] : NULL); DebugTest();
							glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, (GLint)internalFormat, width, width, 0, (GLenum)format_, (GLenum)componentType, data_ ? ((void**)data_)[3] : NULL); DebugTest();
							glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, (GLint)internalFormat, width, width, 0, (GLenum)format_, (GLenum)componentType, data_ ? ((void**)data_)[4] : NULL); DebugTest();
							glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, (GLint)internalFormat, width, width, 0, (GLenum)format_, (GLenum)componentType, data_ ? ((void**)data_)[5] : NULL); DebugTest();
						} break;
						default: throw Exception("Unsupported texture type");
					}

					if(filter.min != Filter::Min::Off && filter.min != Filter::Min::Linear)
					{
						glGenerateMipmap((GLenum)type); DebugTest();
					}

					return handle;
				}())
			{
			}
			inline ~Texture();
		public:
			inline void Set(const Size& slot_)
			{
				context->SetTexture(slot_, this);
			}
			inline void Reset(const Size& slot_)
			{
				context->ResetTexture(slot_, this);
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
			inline InternalFormat GetInternalFormat() const
			{
				return internalFormat;
			}
			inline ComponentType GetComponents() const
			{
				return componentType;
			}
			inline Wrap GetWrap() const
			{
				return wrap;
			}
			inline Filter GetFilter() const
			{
				return filter;
			}
		};

		class Buffer
		{
		public:
			using Input = const void*;
			using Data = void*;
		public:
			enum class Usage: GLenum
			{
				Static = GL_STATIC_DRAW,
				Dynamic = GL_DYNAMIC_DRAW,
				Stream = GL_STREAM_DRAW
			};
		protected:
			const Usage usage;
			const Size size;
		public:
			inline Buffer(const Size& size_, const Usage& usage_):
				size(size_),
				usage(usage_)
			{
			}
			inline ~Buffer() = default;
		public:
			inline Size GetSize() const
			{
				return size;
			}
			inline Usage GetUsage() const
			{
				return usage;
			}
		};
		namespace Buffers
		{
			class Array:
				public ContextDependent,
				public Buffer
			{
			protected:
				using Handle = GLuint;
			protected:
				friend Context;
				static const GLenum target = GL_ARRAY_BUFFER;
			public:
				const Handle handle;
			public:
				inline Array(Context* context_, const Size& size_, Input input_, const Usage& usage_ = Usage::Static):
					ContextDependent(context_),
					Buffer(size_, usage_),
					handle([](){
						Handle handle;
						glGenBuffers(1, &handle); DebugTest();
						return handle;
					}())
				{
					context->SetBufferArray(this);
					glBufferData(target, size, input_, (GLenum)usage); DebugTest();
				}
				template<class Container>
				inline Array(Context* context_, const Container& container_, const Usage& usage_ = Usage::Static):
					Array(context_, container_.size() * sizeof(Container::value_type), container_.data(), usage_)
				{
				}
				inline Array(Context* context_, const Size& size_, const Usage& usage_ = Usage::Static):
					Array(context_, size_, nullptr, usage_)
				{
				}
				inline ~Array()
				{
					context->ResetBufferArray(this);
					glDeleteBuffers(1, &handle); DebugTest();
				}
			protected:
				inline void DebugIsActiveTest() const
				{
#if GVE_DEBUG
					if(context->GetBufferArray() != this)
					{
						throw Exception("Array buffer is not set");
					}
#endif
				}
			public:
				inline void Set()
				{
					context->SetBufferArray(this);
				}
				inline void Reset()
				{
					context->ResetBufferArray(this);
				}
				inline Data Map()
				{
					DebugIsActiveTest();
				
					Data data = glMapBuffer(target, GL_READ_WRITE); DebugTest();

					return data;
				}
				inline void Unmap()
				{
					DebugIsActiveTest();

					glUnmapBuffer(target);
				}
			};
			class Index:
				public ContextDependent,
				public Buffer
			{
			protected:
				using Handle = GLuint;
			protected:
				friend Context;
				static const GLenum target = GL_ELEMENT_ARRAY_BUFFER;
			public:
				const Handle handle;
			public:
				inline Index(Context* context_, const Size& size_, Input input_, const Usage& usage_ = Usage::Static):
					ContextDependent(context_),
					Buffer(size_, usage_),
					handle([](){
						Handle handle;
						glGenBuffers(1, &handle); DebugTest();
						return handle;
					}())
				{
					context->SetBufferIndex(this);
					glBufferData(target, size, input_, (GLenum)usage); DebugTest();
				}
				template<class Container>
				inline Index(Context* context_, const Container& container_, const Usage& usage_ = Usage::Static):
					Index(context_, container_.size() * sizeof(Container::value_type), container_.data(), usage_)
				{
				}
				inline Index(Context* context_, const Size& size_, const Usage& usage_ = Usage::Static):
					Index(context_, size_, nullptr, usage_)
				{
				}
				inline ~Index()
				{
					context->ResetBufferIndex(this);
					glDeleteBuffers(1, &handle); DebugTest();
				}
			protected:
				inline void DebugIsActiveTest() const
				{
#if GVE_DEBUG
					if(context->GetBufferIndex() != this)
					{
						throw Exception("Index buffer is not set");
					}
#endif
				}
			public:
				inline void Set()
				{
					context->SetBufferIndex(this);
				}
				inline void Reset()
				{
					context->ResetBufferIndex(this);
				}
				inline Data Map()
				{
					DebugIsActiveTest();
				
					Data data = glMapBuffer(target, GL_READ_WRITE); DebugTest();

					return data;
				}
				inline void Unmap()
				{
					DebugIsActiveTest();

					glUnmapBuffer(target);
				}
			};
			class Uniform:
				public Buffer
			{
			};
			class Attribute:
				public ContextDependent
			{
			public:
				using Handle = GLuint;
			public:
				struct Data
				{
				public:
					enum struct Type: GLenum
					{
						Int8 = GL_BYTE,
						UInt8 = GL_UNSIGNED_BYTE,
						Int16 = GL_SHORT,
						UInt16 = GL_UNSIGNED_SHORT,
						Int32 = GL_INT,
						UInt32 = GL_UNSIGNED_INT,
						Float16 = GL_HALF_FLOAT,
						Float32 = GL_FLOAT,
						Float64 = GL_DOUBLE
					};
				public:
					using Location = Program::Attribute;
				public:
					const Location location;
					const Type type;
					const Size componentType;
					const bool isNormalized;
					const Size stride;
					const Size offset;
				public:
					inline Data(
						const Location location_,
						const Type type_,
						const Size componentType_,
						const bool isNormalized_,
						const Size stride_,
						const Size offset_
					):
						location(location_),
						type(type_),
						componentType(componentType_),
						isNormalized(isNormalized_),
						stride(stride_),
						offset(offset_)
					{
					}
				};
			protected:
				friend Context;
				const Handle handle;
			public:
				inline Attribute(Context* context_, Program* program_, Buffers::Array* bufferArray_, Buffers::Index* bufferIndex_, const Initializer<Data>& datas_):
					ContextDependent(context_),
					handle([](){
						Handle handle;
						glGenVertexArrays(1, &handle); DebugTest();
						return handle;
					}())
				{
					context->SetBufferAttribute(this);

					if(context->GetProgram() != program_)
					{
						throw Exception("Program is not set");
					}

					if(context->GetBufferArray() != bufferArray_)
					{
						throw Exception("Array buffer is not set");
					}
					
					if(bufferIndex_)
					{
						bufferIndex_->Set();
					}

					for(auto data : datas_)
					{
						if(data.location != -1)
						{
							glVertexAttribPointer(
								data.location,
								data.componentType,
								(GLenum)data.type,
								data.isNormalized,
								data.stride,
								(const void*)data.offset); DebugTest();
							glEnableVertexAttribArray(data.location); DebugTest();
						}
					}
				}
				inline ~Attribute()
				{
					context->ResetBufferAttribute(this);
					glDeleteVertexArrays(1, &handle); DebugTest();
				}
			protected:
				inline void DebugIsActiveTest() const
				{
#if GVE_DEBUG
					if(context->GetBufferAttribute() != this)
					{
						throw Exception("Attribute buffer is not set");
					}
#endif
				}
			public:
				inline void Set()
				{
					context->SetBufferAttribute(this);
				}
				inline void Reset()
				{
					context->ResetBufferAttribute(this);
				}
			};
		}
	}
}


#pragma region Context
inline void GreatVEngine::OpenGL::Context::SetBufferArray(Buffers::Array* buffer_)
{
	if(currentBufferArray != buffer_)
	{
		currentBufferArray = buffer_;
		glBindBuffer(Buffers::Array::target, buffer_ ? buffer_->handle : NULL); DebugTest();
	}
}
inline void GreatVEngine::OpenGL::Context::ResetBufferArray(Buffers::Array* buffer_)
{
	if(currentBufferArray == buffer_)
	{
		currentBufferArray = nullptr;
		glBindBuffer(Buffers::Array::target, NULL); DebugTest();
	}
}
inline void GreatVEngine::OpenGL::Context::SetBufferIndex(Buffers::Index* buffer_)
{
	if(currentBufferIndex != buffer_)
	{
		currentBufferIndex = buffer_;
		glBindBuffer(Buffers::Index::target, buffer_ ? buffer_->handle : NULL); DebugTest();
	}
}
inline void GreatVEngine::OpenGL::Context::ResetBufferIndex(Buffers::Index* buffer_)
{
	if(currentBufferIndex == buffer_)
	{
		currentBufferIndex = nullptr;
		glBindBuffer(Buffers::Index::target, NULL); DebugTest();
	}
}
inline void GreatVEngine::OpenGL::Context::SetBufferAttribute(Buffers::Attribute* buffer_)
{
	if(currentBufferAttribute != buffer_)
	{
		currentBufferAttribute = buffer_;
		glBindVertexArray(currentBufferAttribute ? currentBufferAttribute->handle : NULL); DebugTest();

		currentBufferIndex = nullptr; // openGL "reset" index buffer after set VAO
	}
}
inline void GreatVEngine::OpenGL::Context::ResetBufferAttribute(Buffers::Attribute* buffer_)
{
	if(currentBufferAttribute == buffer_)
	{
		currentBufferAttribute = nullptr;
		glBindVertexArray(NULL); DebugTest();
	}
}
inline void GreatVEngine::OpenGL::Context::SetProgram(Program* program_)
{
	if(currentProgram != program_)
	{
		currentProgram = program_;
		glUseProgram(currentProgram ? currentProgram->handle : NULL); DebugTest();
	}
}
inline void GreatVEngine::OpenGL::Context::ResetProgram(Program* program_)
{
	if(currentProgram == program_)
	{
		currentProgram = nullptr;
		glUseProgram(NULL); DebugTest();
	}
}
inline void GreatVEngine::OpenGL::Context::SetTexture(const Size& slot_, Texture* texture_)
{
	if(currentTexture[slot_] != texture_)
	{
		glActiveTexture(GL_TEXTURE0 + slot_); DebugTest();

		if(texture_)
		{
			glBindTexture((GLenum)texture_->GetType(), texture_->GetHandle()); DebugTest();
		}
		else
		{
			glBindTexture((GLenum)currentTexture[slot_]->GetType(), 0); DebugTest();
		}

		currentTexture[slot_] = texture_;
	}
}
inline void GreatVEngine::OpenGL::Context::ResetTexture(const Size& slot_, Texture* texture_)
{
	if(currentTexture[slot_] == texture_ && texture_)
	{
		glActiveTexture(GL_TEXTURE0 + slot_); DebugTest();
		glBindTexture((GLenum)currentTexture[slot_]->GetType(), 0); DebugTest();

		currentTexture[slot_] = nullptr;
	}
}
#pragma endregion
#pragma region Texture
inline GreatVEngine::OpenGL::Texture::~Texture()
{
	for(Size i = 0; i < context->GetTexturesCount(); ++i)
	{
		context->ResetTexture(i, this);
	}
}
#pragma endregion


#pragma region
#pragma endregion
