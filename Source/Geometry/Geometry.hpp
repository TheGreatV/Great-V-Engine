#pragma region Include
#pragma once


#include "Header.hpp"


#include <System/System.hpp>
#include <Logic/Mathematics.hpp>
#include <Logic/Helpers.hpp>
#pragma endregion


namespace GreatVEngine
{
	class Geometry:
		public Helper::Transformation::Dimension3::BoundBox
	{
	public:
		enum class VertexPackMode
		{
			Pos32F,
			Pos32F_TBN32F_Tex32F,				// 4*3 + 4*3*3 + 4*2 = 56 bytes
			Pos32F_TN16F_Tex32F,				// 4*3 + 2*3*2 + 2*2 = 32 bytes
			Pos32F_TBN32F_Tex32F_Ind32U_Wei32F,	// 4*3 + 4*4*4 + 4*2 + 4*4*2 = 88 bytes
			Default = Pos32F_TBN32F_Tex32F
		};
		enum class IndexPackMode
		{
			UInt32,
			UInt16,
			UInt8,
			Default = UInt32
		};
		enum class Topology
		{
			Points,
			Lines,
			LineStrip,
			LineLoop,
			Triangles,
			TriangleStrip,
			TriangleFan,
			Patches2,
			Patches3,
			Patches4,
		};
	public:
		using Index = UInt32;
		using Bytes = Vector<UInt8>;
		using BoneIndex = UInt32;
		using BoneWeight = Float32;
	public:
		static const Size BONE_WEIGHT_COUNT = 4;
	public:
		struct Vertex
		{
			Vec3 pos;
			Vec3 tan;
			Vec3 bin;
			Vec3 nor;
			Vec2 tex;
			Array<BoneIndex,	BONE_WEIGHT_COUNT> boneIndices;
			Array<BoneWeight,	BONE_WEIGHT_COUNT> boneWeights;
		};
	public:
		inline static Reference<Geometry> LoadMesh(const Reference<System::BinaryFileReader>& reader_);
		inline static Reference<Geometry> LoadMesh(const Filename& filename_);
		inline static Reference<Geometry> CreatePlain(const Vec2& size_, const Vec2& tex_, const UVec2& seg_);
		inline static Reference<Geometry> CreateBox(const Vec3& size_, const Vec3& tex_, const UVec3& seg_);
		inline static Reference<Geometry> CreateSphere(const Float32& radius_, const Vec2& tex_, const UVec2& seg_);
		inline static Reference<Geometry> CreateCapsule(const Float32& radius_, const Float32& height_, const Vec2& tex_, const UVec2& seg_);
		inline static Reference<Geometry> CreateTorus(const Float32 radius_, const Float32 width_, const Vec2& tex_, const UVec2& seg_);
	public:
		Topology topology = Topology::Triangles;
		Vector<Vertex> vertices;
		Vector<Index> indices;
	public:
		inline Geometry() = default;
		inline Geometry(const Size& verticesCount, const Size& indicesCount):
			vertices(verticesCount),
			indices(indicesCount),
			BoundBox()
		{
		}
		inline Geometry(const Geometry&) = default;
		inline Geometry(Geometry&& source):
			vertices(std::move(source.vertices)),
			indices(std::move(source.indices)),
			BoundBox(std::move(source))
		{
		}
		inline ~Geometry() = default;
	public:
		inline Geometry& operator = (const Geometry&) = default;
		inline Geometry& operator = (Geometry&& source)
		{
			vertices = std::move(source.vertices);
			indices = std::move(source.indices);
			(BoundBox)*this = source;

			return *this;
		}
	public:
		inline Topology GetTopology() const
		{
			return topology;
		}
		inline void SetTopology(const Topology& topology_)
		{
			topology = topology_;
		}
		inline static Size GetVertexSize(const VertexPackMode& vertexPackMode_ = VertexPackMode::Default)
		{
			switch(vertexPackMode_)
			{
				case VertexPackMode::Pos32F:
					return sizeof(Float32)*3;
				case VertexPackMode::Pos32F_TBN32F_Tex32F:
					return sizeof(Float32) * (4 + 3*3 + 2);
				case VertexPackMode::Pos32F_TN16F_Tex32F:
					return  sizeof(Float32)* (3 + 2) + sizeof(Float16)* (3 * 2);
				case VertexPackMode::Pos32F_TBN32F_Tex32F_Ind32U_Wei32F:
					return sizeof(Float32) * (3 + 3 * 3 + 2) + BONE_WEIGHT_COUNT*(sizeof(UInt32)+sizeof(Float32));
				default:
					throw Exception("Unknown vertex packing mode");
			}
		}
		inline Size GetVerticesCount() const
		{
			return vertices.size();
		}
		inline Reference<Bytes> GetVertices(const VertexPackMode& vertexPackMode_ = VertexPackMode::Default) const
		{
			Size vertexSize = GetVertexSize(vertexPackMode_);
			auto bytes = MakeReference<Bytes>(vertices.size() * vertexSize, 0x00);

			switch(vertexPackMode_)
			{
				case VertexPackMode::Pos32F:
				{
					Size i = 0;
					for(auto &vertex : vertices)
					{
						auto byte = bytes->data() + i;
					
						*(Vec3*)(byte +(sizeof(Float32)* 3 * 0)) = vertex.pos;
					
						i += vertexSize;
					}

					return bytes;
				} break;
				case VertexPackMode::Pos32F_TBN32F_Tex32F:
				{
					Size i = 0;
					for(auto &vertex : vertices)
					{
						auto byte = bytes->data() + i;
					
						*(Vec3*)(byte +(sizeof(Float32)* 3 * 0)) = vertex.pos;
						*(Vec3*)(byte +(sizeof(Float32)* 3 * 1)) = vertex.tan;
						*(Vec3*)(byte +(sizeof(Float32)* 3 * 2)) = vertex.bin;
						*(Vec3*)(byte +(sizeof(Float32)* 3 * 3)) = vertex.nor;
						*(Vec2*)(byte +(sizeof(Float32)* 3 * 4)) = vertex.tex;
					
						i += vertexSize;
					}

					return bytes;
				} break;
				case VertexPackMode::Pos32F_TBN32F_Tex32F_Ind32U_Wei32F:
				{
					Size i = 0;
					for(auto &vertex : vertices)
					{
						auto data = bytes->data() + vertexSize * i;

						*(Vec3*)data = vertex.pos; data += sizeof(Float32)* 3;
						*(Vec3*)data = vertex.tan; data += sizeof(Float32) * 3;
						*(Vec3*)data = vertex.bin; data += sizeof(Float32) * 3;
						*(Vec3*)data = vertex.nor; data += sizeof(Float32) * 3;
						*(Vec2*)data = vertex.tex; data += sizeof(Float32) * 2;

						for(Size j = 0; j < vertex.boneIndices.size(); ++j)
						{
							*(Float32*)data = (Float32)vertex.boneIndices[j]; data += sizeof(Float32);
						}
						for(Size j = 0; j < vertex.boneWeights.size(); ++j)
						{
							*(Float32*)data = vertex.boneWeights[j]; data += sizeof(Float32);
						}

						++i;
					}

					return bytes;
				} break;
				default:
				{
					throw Exception("Not implemented");
				} break;
			}
		}
		inline Reference<Bytes> GetVertices(const Mat4& transform_, const VertexPackMode& vertexPackMode_ = VertexPackMode::Default) const
		{
			Size vertexSize = GetVertexSize(vertexPackMode_);
			auto bytes = MakeReference<Bytes>(vertices.size() * vertexSize, 0x00);

			auto rotate = Mat3(transform_);

			switch(vertexPackMode_)
			{
				case VertexPackMode::Pos32F:
				{
					Size i = 0;
					for(auto &vertex : vertices)
					{
						auto byte = bytes->data() + i;
					
						auto pos = VecXYZ(transform_ * Vec4(vertex.pos, 1.0f));

						*(Vec3*)(byte +(sizeof(Float32)* 3 * 0)) = pos;
					
						i += vertexSize;
					}

					return bytes;
				} break;
				case VertexPackMode::Pos32F_TBN32F_Tex32F:
				{
					Size i = 0;
					for(auto &vertex : vertices)
					{
						auto byte = bytes->data() + i;
					
						auto &pos = VecXYZ(transform_ * Vec4(vertex.pos, 1.0f));
						auto &tan = rotate * vertex.tan;
						auto &bin = rotate * vertex.bin;
						auto &nor = rotate * vertex.nor;
						auto &tex = vertex.tex;

						*(Vec3*)(byte + (sizeof(Float32)* 3 * 0)) = pos;
						*(Vec3*)(byte +(sizeof(Float32)* 3 * 1)) = tan;
						*(Vec3*)(byte +(sizeof(Float32)* 3 * 2)) = bin;
						*(Vec3*)(byte +(sizeof(Float32)* 3 * 3)) = nor;
						*(Vec2*)(byte +(sizeof(Float32)* 3 * 4)) = tex;
					
						i += vertexSize;
					}

					return bytes;
				} break;
				case VertexPackMode::Pos32F_TBN32F_Tex32F_Ind32U_Wei32F:
				{
					Size i = 0;
					for(auto &vertex : vertices)
					{
						auto data = bytes->data() + vertexSize * i;

						auto &pos = VecXYZ(transform_ * Vec4(vertex.pos, 1.0f));
						auto &tan = rotate * vertex.tan;
						auto &bin = rotate * vertex.bin;
						auto &nor = rotate * vertex.nor;
						auto &tex = vertex.tex;

						*(Vec3*)data = pos; data += sizeof(Float32)* 3;
						*(Vec3*)data = tan; data += sizeof(Float32) * 3;
						*(Vec3*)data = bin; data += sizeof(Float32) * 3;
						*(Vec3*)data = nor; data += sizeof(Float32) * 3;
						*(Vec2*)data = tex; data += sizeof(Float32) * 2;

						for(Size j = 0; j < vertex.boneIndices.size(); ++j)
						{
							*(Float32*)data = (Float32)vertex.boneIndices[j]; data += sizeof(Float32);
						}
						for(Size j = 0; j < vertex.boneWeights.size(); ++j)
						{
							*(Float32*)data = vertex.boneWeights[j]; data += sizeof(Float32);
						}

						++i;
					}

					return bytes;
				} break;
				default:
				{
					throw Exception("Not implemented");
				} break;
			}
		}
		inline static Size GetIndexSize(const IndexPackMode& indexPackMode_ = IndexPackMode::Default)
		{
			switch(indexPackMode_)
			{
				case IndexPackMode::UInt32: return 4;
				case IndexPackMode::UInt16: return 2;
				case IndexPackMode::UInt8: return 1;
				default: throw Exception("nknown index packing mode");
			}
		}
		inline Size GetIndicesCount() const
		{
			return indices.size();
		}
		inline Reference<Bytes> GetIndices(const IndexPackMode& indexPackMode_ = IndexPackMode::Default) const
		{
			switch(indexPackMode_)
			{
				case IndexPackMode::UInt32:
				{
					Size indexSize = sizeof(UInt32);
					auto bytes = MakeReference<Bytes>(indices.size() * indexSize);

					Size i = 0;
					for(auto &index : indices)
					{
						auto byte = bytes->data() + i;
					
						*(UInt32*)(byte + 0) = index;
					
						i += indexSize;
					}

					return bytes;
				} break;
				default:
				{
					throw Exception("Not implemented");
				} break;
			}
		}
	public:
		inline void GenBoundBox()
		{
			if(!vertices.empty())
			{
				minimal = maximal = vertices[0].pos;

				for(auto &vertex : vertices)
				{
					if(vertex.pos.x < minimal.x)
					{
						minimal.x = vertex.pos.x;
					}
					if(vertex.pos.y < minimal.y)
					{
						minimal.y = vertex.pos.y;
					}
					if(vertex.pos.z < minimal.z)
					{
						minimal.z = vertex.pos.z;
					}

					if(vertex.pos.x > maximal.x)
					{
						maximal.x = vertex.pos.x;
					}
					if(vertex.pos.y > maximal.y)
					{
						maximal.y = vertex.pos.y;
					}
					if(vertex.pos.z > maximal.z)
					{
						maximal.z = vertex.pos.z;
					}
				}
			}
		}
		inline void	GenTangentSpace(bool flip_t, bool flip_b)
		{
			Size id0, id1, id2;
			for(Size i = 0; i < indices.size(); i += 3)
			{
				id0 = indices[i + 0];
				id1 = indices[i + 1];
				id2 = indices[i + 2];

				Vec3 e0 = vertices[id1].pos - vertices[id0].pos;
				Vec3 e1 = vertices[id2].pos - vertices[id0].pos;
				Vec2 e0uv = vertices[id1].tex - vertices[id0].tex;
				Vec2 e1uv = vertices[id2].tex - vertices[id0].tex;
				Float32 cp = e0uv.y * e1uv.x - e0uv.x * e1uv.y;
				if(cp != 0.0f)
				{
					float32 k = 1.0f / cp;
					vertices[id0].tan = Normalize((e0 * -e1uv.y + e1 * e0uv.y) * k);
					vertices[id0].bin = Normalize((e0 * -e1uv.x + e1 * e0uv.x) * k);
				}

				vertices[id2].tan = vertices[id1].tan = vertices[id0].tan;
				vertices[id2].bin = vertices[id1].bin = vertices[id0].bin;

				if(flip_t)
				{
					vertices[id0].tan = -vertices[id0].tan;
					vertices[id1].tan = -vertices[id1].tan;
					vertices[id2].tan = -vertices[id2].tan;
				}
				if(flip_b)
				{
					vertices[id0].bin = -vertices[id0].bin;
					vertices[id1].bin = -vertices[id1].bin;
					vertices[id2].bin = -vertices[id2].bin;
				}
			}
		}
	};
}


#pragma region Geometry
inline GreatVEngine::Reference<GreatVEngine::Geometry> GreatVEngine::Geometry::LoadMesh(const Reference<System::BinaryFileReader>& reader_)
{
	enum class Type
	{
		Undefined = 0,
		UnsignedDiscrete = 1,	// integer
		SignedDiscrete = 2,		// integer
		UnsignedIntegral = 3,	// floating
		SignedIntegral = 4,		// floating
	};

	struct VerticesAttributes
	{
		UInt8	Bits;
		UInt8	Type;
		UInt8	Count;
		UInt8	Layers;
	};
	struct IndicesAttributes
	{
		UInt8	Bits;
		UInt8	Type;
	};

	auto verticesCount = (Size)reader_->Read<UInt32>();
	auto positionAttributes = *(VerticesAttributes*)(reader_->ReadRaw<UInt8>(4));
	auto tangentSpaceAttributes = *(VerticesAttributes*)(reader_->ReadRaw<UInt8>(4));
	auto texCoordAttributes = *(VerticesAttributes*)(reader_->ReadRaw<UInt8>(4));
	auto boneIndexAttributes = *(VerticesAttributes*)(reader_->ReadRaw<UInt8>(4));
	auto boneweightAttributes = *(VerticesAttributes*)(reader_->ReadRaw<UInt8>(4));
	// TODO: add other attributes
	
	Size positionSize = (positionAttributes.Bits / BITS_IN_BYTE) * positionAttributes.Count * positionAttributes.Layers;
	Size tangentSpaceSize = (tangentSpaceAttributes.Bits / BITS_IN_BYTE) * tangentSpaceAttributes.Count * tangentSpaceAttributes.Layers;
	Size texCoordSize = (texCoordAttributes.Bits / BITS_IN_BYTE) * texCoordAttributes.Count * texCoordAttributes.Layers;
	Size boneIndexSize = (boneIndexAttributes.Bits / BITS_IN_BYTE) * boneIndexAttributes.Count * boneIndexAttributes.Layers;
	Size boneWeightSize = (boneweightAttributes.Bits / BITS_IN_BYTE) * boneweightAttributes.Count * boneweightAttributes.Layers;
	Size vertexSize = positionSize + tangentSpaceSize + texCoordSize + boneIndexSize + boneWeightSize;
	Size verticesDataSize = vertexSize * verticesCount;

	auto verticesData = reader_->Read<UInt8>(verticesDataSize);

	Size indicesCount = (Size)reader_->Read<UInt32>();
	auto indicesAttributes = *(IndicesAttributes*)(reader_->ReadRaw<UInt8>(2));
	Size indexSize = (indicesAttributes.Bits / BITS_IN_BYTE);
	Size indicesDataSize = indexSize * indicesCount;

	auto indicesData = reader_->Read<UInt8>(indicesDataSize);

	auto mesh = new Geometry(verticesCount, indicesCount);
	{
		// TODO: vertices conversion
		for(Size i = 0; i < verticesCount; ++i)
		{
			auto data = verticesData.data() + vertexSize*i;

			mesh->vertices[i].pos = *(Vec3*)data; data += sizeof(Float32)* 3;
			mesh->vertices[i].tan = *(Vec3*)data; data += sizeof(Float32)* 3;
			mesh->vertices[i].bin = *(Vec3*)data; data += sizeof(Float32)* 3;
			mesh->vertices[i].nor = *(Vec3*)data; data += sizeof(Float32)* 3;
			mesh->vertices[i].tex = *(Vec2*)data; data += sizeof(Float32)* 2;
			for(Size j = 0; j < BONE_WEIGHT_COUNT; ++j)
			{
				mesh->vertices[i].boneIndices[j] = *(UInt32*)data;
				if(mesh->vertices[i].boneIndices[j] == UINT32_MAX)
				{
					mesh->vertices[i].boneIndices[j] = 0;
				}
				data += sizeof(UInt32);
			}
			for(Size j = 0; j < BONE_WEIGHT_COUNT; ++j)
			{
				mesh->vertices[i].boneWeights[j] = *(Float32*)data; data += sizeof(Float32);
			}
		}

		// TODO: indices conversion
		memcpy(mesh->indices.data(), indicesData.data(), indicesDataSize);

		mesh->GenTangentSpace(false, false);
	}

	mesh->GenBoundBox();

	return WrapReference(mesh);
}
inline GreatVEngine::Reference<GreatVEngine::Geometry> GreatVEngine::Geometry::LoadMesh(const Filename& filename_)
{
	auto reader = System::BinaryFileReader::LoadFile(filename_);

	return LoadMesh(reader);
}
inline GreatVEngine::Reference<GreatVEngine::Geometry> GreatVEngine::Geometry::CreatePlain(const Vec2& size_, const Vec2& tex_, const UVec2& seg_)
{
	if(seg_.x == 0 || seg_.y == 0)
	{
		throw Exception("Invalid segments count");
	}

	auto geometry = new Geometry(
		(seg_.x + 1) * (seg_.y + 1),
		seg_.x*seg_.y*6);

	Size id;
	auto maxPos = size_ * 0.5f;
	auto minPos = size_ * -0.5f;

	for(Size x = 0; x <= seg_.x; ++x)
	for(Size y = 0; y <= seg_.y; ++y)
	{
		Vec2 t(Float32(x)/Float32(seg_.x),Float32(y)/Float32(seg_.y));

		id = y*(seg_.x + 1) + x;
		geometry->vertices[id].pos = Vec3(Mix(minPos, maxPos, t), 0.0f);
		geometry->vertices[id].tex = t * Vec2(tex_.x, tex_.y);
		geometry->vertices[id].tan = Vec3(1.0f,0.0f,0.0f);
		geometry->vertices[id].bin = Vec3(0.0f,1.0f,0.0f);
		geometry->vertices[id].nor = Vec3(0.0f,0.0f,-1.0f);
	}

	for(Size x = 0; x < seg_.x; ++x)
	for(Size y = 0; y < seg_.y; ++y)
	{
		id = 6*(y*seg_.x + x);
		geometry->indices[id + 0] = (y + 0)*(seg_.x + 1) + (x + 0);
		geometry->indices[id + 1] = (y+0)*(seg_.x+1) + (x+1);
		geometry->indices[id + 2] = (y+1)*(seg_.x+1) + (x+0);
		geometry->indices[id + 3] = geometry->indices[id+2];
		geometry->indices[id + 4] = geometry->indices[id+1];
		geometry->indices[id + 5] = (y+1)*(seg_.x+1) + (x+1);
	}

	geometry->GenBoundBox();

	return WrapReference(geometry);
}
inline GreatVEngine::Reference<GreatVEngine::Geometry> GreatVEngine::Geometry::CreateBox(const Vec3& size_, const Vec3& tex_, const UVec3& seg_)
{
	if(seg_.x == 0 || seg_.y == 0 || seg_.z == 0)
	{
		throw Exception("Invalid segments count");
	}

	auto geometry = new Geometry(
		2*((seg_.x+1)*(seg_.y+1) + (seg_.y+1)*(seg_.z+1) + (seg_.z+1)*(seg_.x+1)),
		2*6*(seg_.x*seg_.y + seg_.y*seg_.z + seg_.z*seg_.x));

	Size id;
	Vec3 maxPos = size_ * 0.5f;
	Vec3 minPos = size_ * -0.5f;

	const Size offsetVertexBack = 0;
	const Size offsetVertexFront = offsetVertexBack + (seg_.x+1)*(seg_.y+1);
	const Size offsetVertexLeft = offsetVertexFront + (seg_.x+1)*(seg_.y+1);
	const Size offsetVertexRight = offsetVertexLeft + (seg_.y+1)*(seg_.z+1);
	const Size offsetVertexBottom = offsetVertexRight + (seg_.y+1)*(seg_.z+1);
	const Size offsetVertexTop = offsetVertexBottom + (seg_.z+1)*(seg_.x+1);
	const Size offsetIndexBack = 0;
	const Size offsetIndexFront = offsetIndexBack + 6*(seg_.x*seg_.y);
	const Size offsetIndexLeft = offsetIndexFront + 6*(seg_.x*seg_.y);
	const Size offsetIndexRight = offsetIndexLeft + 6*(seg_.y*seg_.z);
	const Size offsetIndexBottom = offsetIndexRight + 6*(seg_.y*seg_.z);
	const Size offsetIndexTop = offsetIndexBottom + 6*(seg_.z*seg_.y);

	for(Size x = 0; x <= seg_.x; ++x)
	for(Size y = 0; y <= seg_.y; ++y)
	{
		Vec2 t(Float32(x)/Float32(seg_.x),Float32(y)/Float32(seg_.y));

		id = offsetVertexBack + y*(seg_.x+1) + x;
		geometry->vertices[id].pos = Vec3(minPos.x + size_.x*t.x, minPos.y + size_.y*t.y, minPos.z);
		geometry->vertices[id].tex = t * Vec2(tex_.x, tex_.y);
		geometry->vertices[id].tan = Vec3(1.0f,0.0f,0.0f);
		geometry->vertices[id].bin = Vec3(0.0f,1.0f,0.0f);
		geometry->vertices[id].nor = Vec3(0.0f,0.0f,-1.0f);

		id = offsetVertexFront + y*(seg_.x+1) + x;
		geometry->vertices[id].pos = Vec3(minPos.x + size_.x*t.x,minPos.y + size_.y*t.y,maxPos.z);
		geometry->vertices[id].tex = Vec2(1.0f - t.x,t.y) * Vec2(tex_.x, tex_.y);
		geometry->vertices[id].tan = Vec3(-1.0f,0.0f,0.0f);
		geometry->vertices[id].bin = Vec3(0.0f,1.0f,0.0f);
		geometry->vertices[id].nor = Vec3(0.0f,0.0f,1.0f);
	}
	for(Size y = 0; y <= seg_.y; ++y)
	for(Size z = 0; z <= seg_.z; ++z)
	{
		Vec2 t(Float32(z)/Float32(seg_.z),Float32(y)/Float32(seg_.y));

		id = offsetVertexLeft + z*(seg_.y+1) + y;
		geometry->vertices[id].pos = Vec3(minPos.x,minPos.y + size_.y*t.y,minPos.z + size_.z*t.x);
		geometry->vertices[id].tex = Vec2(1.0f - t.x,t.y) * Vec2(tex_.z, tex_.y);
		geometry->vertices[id].tan = Vec3(0.0f,0.0f,-1.0f);
		geometry->vertices[id].bin = Vec3(0.0f,1.0f,0.0f);
		geometry->vertices[id].nor = Vec3(-1.0f,0.0f,0.0f);

		id = offsetVertexRight + z*(seg_.y+1) + y;
		geometry->vertices[id].pos = Vec3(maxPos.x,minPos.y + size_.y*t.y,minPos.z + size_.z*t.x);
		geometry->vertices[id].tex = t * Vec2(tex_.z, tex_.y);
		geometry->vertices[id].tan = Vec3(0.0f,0.0f,1.0f);
		geometry->vertices[id].bin = Vec3(0.0f,1.0f,0.0f);
		geometry->vertices[id].nor = Vec3(1.0f,0.0f,0.0f);
	}
	for(Size z = 0; z <= seg_.z; ++z)
	for(Size x = 0; x <= seg_.x; ++x)
	{
		Vec2 t(Float32(x)/Float32(seg_.x),Float32(z)/Float32(seg_.z));

		id = offsetVertexBottom + x*(seg_.z+1) + z;
		geometry->vertices[id].pos = Vec3(minPos.x + size_.x*t.x,minPos.y,minPos.z + size_.z*t.y);
		geometry->vertices[id].tex = Vec2(t.x,1.0f - t.y) * Vec2(tex_.x,tex_.z);
		geometry->vertices[id].tan = Vec3(1.0f,0.0f,0.0f);
		geometry->vertices[id].bin = Vec3(0.0f,0.0f,-1.0f);
		geometry->vertices[id].nor = Vec3(0.0f,-1.0f,0.0f);

		id = offsetVertexTop + x*(seg_.z+1) + z;
		geometry->vertices[id].pos = Vec3(minPos.x + size_.x*t.x,maxPos.y,minPos.z + size_.z*t.y);
		geometry->vertices[id].tex = t * Vec2(tex_.x, tex_.z);
		geometry->vertices[id].tan = Vec3(1.0f,0.0f,0.0f);
		geometry->vertices[id].bin = Vec3(0.0f,0.0f,1.0f);
		geometry->vertices[id].nor = Vec3(0.0f,1.0f,0.0f);
	}

	for(Size x = 0; x < seg_.x; ++x)
	for(Size y = 0; y < seg_.y; ++y)
	{
		id = offsetIndexBack + 6*(y*seg_.x + x);
		geometry->indices[id + 0] = offsetVertexBack + (y + 0)*(seg_.x + 1) + (x + 0);
		geometry->indices[id+1] = offsetVertexBack + (y+0)*(seg_.x+1) + (x+1);
		geometry->indices[id+2] = offsetVertexBack + (y+1)*(seg_.x+1) + (x+0);
		geometry->indices[id+3] = geometry->indices[id+2];
		geometry->indices[id+4] = geometry->indices[id+1];
		geometry->indices[id+5] = offsetVertexBack + (y+1)*(seg_.x+1) + (x+1);

		id = offsetIndexFront + 6*(y*seg_.x + x);
		geometry->indices[id+0] = offsetVertexFront + (y+0)*(seg_.x+1) + (x+0);
		geometry->indices[id+2] = offsetVertexFront + (y+0)*(seg_.x+1) + (x+1);
		geometry->indices[id+1] = offsetVertexFront + (y+1)*(seg_.x+1) + (x+0);
		geometry->indices[id+3] = geometry->indices[id+2];
		geometry->indices[id+4] = geometry->indices[id+1];
		geometry->indices[id+5] = offsetVertexFront + (y+1)*(seg_.x+1) + (x+1);
	}
	for(Size y = 0; y < seg_.y; ++y)
	for(Size z = 0; z < seg_.z; ++z)
	{
		id = offsetIndexLeft + 6*(z*seg_.y + y);
		geometry->indices[id+0] = offsetVertexLeft + (z+0)*(seg_.y+1) + (y+0);
		geometry->indices[id+1] = offsetVertexLeft + (z+0)*(seg_.y+1) + (y+1);
		geometry->indices[id+2] = offsetVertexLeft + (z+1)*(seg_.y+1) + (y+0);
		geometry->indices[id+3] = geometry->indices[id+2];
		geometry->indices[id+4] = geometry->indices[id+1];
		geometry->indices[id+5] = offsetVertexLeft + (z+1)*(seg_.y+1) + (y+1);

		id = offsetIndexRight + 6*(z*seg_.y + y);
		geometry->indices[id+0] = offsetVertexRight + (z+0)*(seg_.y+1) + (y+0);
		geometry->indices[id+2] = offsetVertexRight + (z+0)*(seg_.y+1) + (y+1);
		geometry->indices[id+1] = offsetVertexRight + (z+1)*(seg_.y+1) + (y+0);
		geometry->indices[id+3] = geometry->indices[id+2];
		geometry->indices[id+4] = geometry->indices[id+1];
		geometry->indices[id+5] = offsetVertexRight + (z+1)*(seg_.y+1) + (y+1);
	}
	for(Size z = 0; z < seg_.z; ++z)
	for(Size x = 0; x < seg_.x; ++x)
	{
		id = offsetIndexBottom + 6*(x*seg_.z + z);
		geometry->indices[id+0] = offsetVertexBottom + (x+0)*(seg_.z+1) + (z+0);
		geometry->indices[id+1] = offsetVertexBottom + (x+0)*(seg_.z+1) + (z+1);
		geometry->indices[id+2] = offsetVertexBottom + (x+1)*(seg_.z+1) + (z+0);
		geometry->indices[id+3] = geometry->indices[id+2];
		geometry->indices[id+4] = geometry->indices[id+1];
		geometry->indices[id+5] = offsetVertexBottom + (x+1)*(seg_.z+1) + (z+1);

		id = offsetIndexTop + 6*(x*seg_.z + z);
		geometry->indices[id+0] = offsetVertexTop + (x+0)*(seg_.z+1) + (z+0);
		geometry->indices[id+2] = offsetVertexTop + (x+0)*(seg_.z+1) + (z+1);
		geometry->indices[id+1] = offsetVertexTop + (x+1)*(seg_.z+1) + (z+0);
		geometry->indices[id+3] = geometry->indices[id+2];
		geometry->indices[id+4] = geometry->indices[id+1];
		geometry->indices[id+5] = offsetVertexTop + (x+1)*(seg_.z+1) + (z+1);
	}

	geometry->GenBoundBox();

	return WrapReference(geometry);
}
inline GreatVEngine::Reference<GreatVEngine::Geometry> GreatVEngine::Geometry::CreateSphere(const Float32& radius_, const Vec2& tex_, const UVec2& seg_)
{
	if(seg_.x < 3 || seg_.y < 3)
	{
		throw Exception("Invalid segments count");
	}

	auto geometry = new Geometry(
		(seg_.x + 1)*(seg_.y + 1),
		6 * seg_.x*seg_.y);

	for(Size x = 0; x <= seg_.x; ++x)
	for(Size y = 0; y <= seg_.y; ++y)
	{
		Float32 dx = Float32(x) / Float32(seg_.x);
		Float32 dy = Float32(y) / Float32(seg_.y);
		Size id = y*(seg_.x + 1) + x;
		Mat3 rMat = RotateZXY3(Vec3(90.0f - dy*180.0f, 180.0f - dx*360.0f, 0.0f));
		geometry->vertices[id].pos = rMat * Vec3(0.0f, 0.0f, radius_);
		geometry->vertices[id].tex = Vec2(dx, dy)*tex_;
		geometry->vertices[id].tan = rMat * Vec3(-1.0f, 0.0f, 0.0f);
		geometry->vertices[id].bin = rMat * Vec3(0.0f, 1.0f, 0.0f);
		geometry->vertices[id].nor = rMat * Vec3(0.0f, 0.0f, 1.0f);
	}

	for(Size x = 0; x < seg_.x; ++x)
	for(Size y = 0; y < seg_.y; ++y)
	{
		Size id = 6 * (y*seg_.x + x);
		geometry->indices[id + 0] = (y + 0)*(seg_.x + 1) + (x + 0);
		geometry->indices[id + 1] = (y + 0)*(seg_.x + 1) + (x + 1);
		geometry->indices[id + 2] = (y + 1)*(seg_.x + 1) + (x + 0);
		geometry->indices[id + 3] = geometry->indices[id + 1];
		geometry->indices[id + 4] = (y + 1)*(seg_.x + 1) + (x + 1);
		geometry->indices[id + 5] = geometry->indices[id + 2];
	}

	geometry->GenBoundBox();

	return WrapReference(geometry);
}
inline GreatVEngine::Reference<GreatVEngine::Geometry> GreatVEngine::Geometry::CreateCapsule(const Float32& radius_, const Float32& height_, const Vec2& tex_, const UVec2& seg_)
{
	auto geometry = new Geometry(
		(seg_.x + 1)*(seg_.y + 1) * 2,
		(6 * ((seg_.y - 1) * 2 + 1) + 3 * 2) * seg_.x);

	Float32	st = (radius_*PI*0.5f) / (2.0f*(radius_*PI*0.5f) + height_);
	Mat3	tMat;

	for(Size x = 0; x <= seg_.x; ++x)
	{
		Float32 dx = Float32(x) / Float32(seg_.x);

		for(Size y = 0; y <= seg_.y; ++y)
		{
			Float32 dy = Float32(y) / Float32(seg_.y);
			{
				Size id = (seg_.y+1)*2*x + y;
				tMat = RotateZXY3(Vec3(-90.0f + 90.0f*dy,360.0f*dx-180.0f,0.0f));
				geometry->vertices[id].pos = (tMat * Vec3(0.0f, 0.0f, radius_)) + Vec3(0.0f, height_*0.5f, 0.0f);
				geometry->vertices[id].tan = tMat*Vec3(-1.0f, 0.0f, 0.0f);
				geometry->vertices[id].bin = tMat*Vec3(0.0f, -1.0f, 0.0f);
				geometry->vertices[id].nor = tMat*Vec3(0.0f, 0.0f, 1.0f);
				geometry->vertices[id].tex = Vec2(1.0f - dx, 1.0f - dy*st) * tex_;
			}
			{
				Size id = (seg_.y+1)*2*x + (seg_.y+1) + y;
				tMat = RotateZXY3(Vec3(90.0f*dy, 360.0f*dx - 180.0f, 0.0f));
				geometry->vertices[id].pos = (tMat * Vec3(0.0f, 0.0f, radius_)) + Vec3(0.0f, -height_*0.5f, 0.0f);
				geometry->vertices[id].tan = tMat*Vec3(-1.0f, 0.0f, 0.0f);
				geometry->vertices[id].bin = tMat*Vec3(0.0f, -1.0f, 0.0f);
				geometry->vertices[id].nor = tMat*Vec3(0.0f, 0.0f, 1.0f);
				geometry->vertices[id].tex = Vec2(1.0f - dx, (1.0f - dy)*st) * tex_;
			}
		}
	}

	Size iR = 6*(1 + 2*(seg_.y-1)) + 3*2;
	for(Size x = 0; x < seg_.x; ++x)
	{
		{
			Size id = iR*x + 0;
			geometry->indices[id + 0] = (seg_.y + 1) * 2 * (x + 0) + 0;
			geometry->indices[id + 1] = (seg_.y + 1) * 2 * (x + 1) + 1;
			geometry->indices[id + 2] = (seg_.y + 1) * 2 * (x + 0) + 1;
		}
		{
			Size id = iR*x + (3 + 6*(2*(seg_.y-1) + 1)) + 0;
			geometry->indices[id + 0] = (seg_.y + 1) * 2 * (x + 0) + (2 * seg_.y);
			geometry->indices[id + 1] = (seg_.y + 1) * 2 * (x + 1) + (2 * seg_.y);
			geometry->indices[id + 2] = (seg_.y + 1) * 2 * (x + 0) + (2 * seg_.y + 1);
		}
		{
			Size id = iR*x + 3 + 6 * (seg_.y - 1);
			geometry->indices[id + 0] = (seg_.y + 1) * 2 * (x + 0) + seg_.y + 0;
			geometry->indices[id + 1] = (seg_.y + 1) * 2 * (x + 1) + seg_.y + 0;
			geometry->indices[id + 2] = (seg_.y + 1) * 2 * (x + 0) + seg_.y + 1;
			geometry->indices[id + 3] = geometry->indices[id + 2];
			geometry->indices[id + 4] = geometry->indices[id + 1];
			geometry->indices[id + 5] = (seg_.y + 1) * 2 * (x + 1) + seg_.y + 1;
		}
		for(Size y = 0; y < seg_.y - 1; ++y)
		{
			{
				Size id = iR*x + 3 + 6 * y;
				geometry->indices[id + 0] = (seg_.y + 1) * 2 * (x + 0) + 1 + (y + 0);
				geometry->indices[id + 1] = (seg_.y + 1) * 2 * (x + 1) + 1 + (y + 0);
				geometry->indices[id + 2] = (seg_.y + 1) * 2 * (x + 0) + 1 + (y + 1);
				geometry->indices[id + 3] = geometry->indices[id + 2];
				geometry->indices[id + 4] = geometry->indices[id + 1];
				geometry->indices[id + 5] = (seg_.y + 1) * 2 * (x + 1) + 1 + (y + 1);
			}
			{
				Size id = iR*x + 3 + 6*(seg_.y-1) + 6 + 6*y;
				geometry->indices[id + 0] = (seg_.y + 1) * 2 * (x + 0) + (seg_.y + 1) + (y + 0);
				geometry->indices[id + 1] = (seg_.y + 1) * 2 * (x + 1) + (seg_.y + 1) + (y + 0);
				geometry->indices[id + 2] = (seg_.y + 1) * 2 * (x + 0) + (seg_.y + 1) + (y + 1);
				geometry->indices[id + 3] = geometry->indices[id + 2];
				geometry->indices[id + 4] = geometry->indices[id + 1];
				geometry->indices[id + 5] = (seg_.y + 1) * 2 * (x + 1) + (seg_.y + 1) + (y + 1);
			}
		}
	}

	geometry->GenBoundBox();

	return WrapReference(geometry);
}
inline GreatVEngine::Reference<GreatVEngine::Geometry> GreatVEngine::Geometry::CreateTorus(const Float32 radius_, const Float32 width_, const Vec2& tex_, const UVec2& seg_)
{
	if(seg_.x < 3 || seg_.y < 3)
	{
		throw Exception("Invalid segment count");
	}

	auto geometry = new Geometry(
		(seg_.x + 1)*(seg_.y + 1),
		seg_.x*seg_.y * 6);

	SInt32 segX = seg_.x + 1;
	SInt32 segY = seg_.y + 1;
	Float32 step_tx = tex_.x / Float32(seg_.x);
	Float32 step_ty = tex_.y / Float32(seg_.y);
	Float32 step_ay = 360.0f / Float32(seg_.y);
	Float32 step_ax = 360.0f / Float32(seg_.x);

	Vec3 tpos;
	Mat3 tmat;
	SInt32 id;

	for(SInt32 x = 0; x < SInt32(seg_.x) + 1; ++x)
	{
		for(SInt32 y = 0; y < SInt32(seg_.y) + 1; ++y)
		{
			id = (seg_.y + 1)*x + y;
			tpos = VecXYZ((
				RotateZXY4(Vec3(0, step_ax*x, 0))
				*Move4(Vec3(0, 0, radius_))
				*RotateZXY4(Vec3(step_ay*y, 0, 0))
				) * Vec4(Vec3(0, 0, width_), 1.0f));
			geometry->vertices[id].pos = tpos;
			geometry->vertices[id].tex = Vec2(step_tx*x, step_ty*y);
			tmat = RotateZXY3(Vec3(step_ay*y, step_ax*x, 0));
			geometry->vertices[id].tan = tmat * Vec3(1, 0, 0);
			geometry->vertices[id].bin = tmat * Vec3(0, -1, 0);
			geometry->vertices[id].nor = tmat * Vec3(0, 0, 1);
		}
	}

	for(SInt32 x = 0; x < SInt32(seg_.x); ++x)
	{
		for(SInt32 y = 0; y < SInt32(seg_.y); ++y)
		{
			id = 6 * (seg_.y*x + y);
			geometry->indices[id + 0] = (seg_.y + 1)*(x + 0) + (y + 0);
			geometry->indices[id + 1] = (seg_.y + 1)*(x + 1) + (y + 0);
			geometry->indices[id + 2] = (seg_.y + 1)*(x + 0) + (y + 1);
			geometry->indices[id + 3] = geometry->indices[id + 1];
			geometry->indices[id + 4] = (seg_.y + 1)*(x + 1) + (y + 1);
			geometry->indices[id + 5] = geometry->indices[id + 2];
		}
	}

	geometry->GenBoundBox();

	return WrapReference(geometry);
}
#pragma endregion


#pragma region
#pragma endregion


