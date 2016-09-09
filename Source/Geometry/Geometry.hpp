#pragma region Include
#pragma once


#include "Header.hpp"


#include <Logic/Mathematics.hpp>
#pragma endregion


namespace GreatVEngine
{
	class Geometry
	{
	public:
		enum class VertexPackMode
		{
			Pos32F_TBN32F_Tex32F,	//4*3 + 4*3*3 + 4*2 = 56 bytes
			Pos32F_TN16F_Tex32F,	//4*3 + 2*3*2 + 2*2 = 32 bytes
			Default = Pos32F_TBN32F_Tex32F
		};
		enum class IndexPackMode
		{
			UInt32,
			UInt16,
			UInt8,
			Default = UInt32
		};
	public:
		using Index = UInt32;
		using Bytes = Vector<UInt8>;
	public:
		struct Vertex
		{
			Vec3 pos;
			Vec3 tan;
			Vec3 bin;
			Vec3 nor;
			Vec2 tex;
		};
	public:
		inline static Reference<Geometry> CreateBox(const Vec3& size_, const Vec3& tex_, const UVec3& seg_);
	public:
		Vector<Vertex> vertices;
		Vector<Index> indices;
	public:
		inline Geometry() = default;
		inline Geometry(const Size& verticesCount, const Size& indicesCount):
			vertices(verticesCount),
			indices(indicesCount)
		{
		}
		inline Geometry(const Geometry&) = default;
		inline Geometry(Geometry&& source):
			vertices(std::move(source.vertices)),
			indices(std::move(source.indices))
		{
		}
		inline ~Geometry() = default;
	public:
		inline Geometry& operator = (const Geometry&) = default;
		inline Geometry& operator = (Geometry&& source)
		{
			vertices = std::move(source.vertices);
			indices = std::move(source.indices);

			return *this;
		}
	public:
		inline Reference<Bytes> PackVertices(const VertexPackMode& vertexPackMode_ = VertexPackMode::Default) const
		{
			switch(vertexPackMode_)
			{
				case VertexPackMode::Pos32F_TBN32F_Tex32F:
				{
					Size vertexSize = sizeof(Float32)*3 + sizeof(Float32)*3*3 + sizeof(Float32)*2;
					auto bytes = MakeReference(new Bytes(vertices.size() * vertexSize));

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
				default:
				{
					throw Exception("Not implemented");
				} break;
			}
		}
		inline Reference<Bytes> PackIndices(const IndexPackMode& indexPackMode_ = IndexPackMode::Default) const
		{
			switch(indexPackMode_)
			{
				case IndexPackMode::UInt32:
				{
					Size indexSize = sizeof(UInt32);
					auto bytes = MakeReference(new Bytes(indices.size() * indexSize));

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
	};
}


#pragma region Geometry
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
		Vec2 t(float32(x)/float32(seg_.x),float32(y)/float32(seg_.y));

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
	for(uint32 y = 0; y <= seg_.y; ++y)
	for(uint32 z = 0; z <= seg_.z; ++z)
	{
		Vec2 t(float32(z)/float32(seg_.z),float32(y)/float32(seg_.y));

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
	for(uint32 z = 0; z <= seg_.z; ++z)
	for(uint32 x = 0; x <= seg_.x; ++x)
	{
		Vec2 t(float32(x)/float32(seg_.x),float32(z)/float32(seg_.z));

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

	for(uint32 x = 0; x < seg_.x; ++x)
	for(uint32 y = 0; y < seg_.y; ++y)
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
	for(uint32 y = 0; y < seg_.y; ++y)
	for(uint32 z = 0; z < seg_.z; ++z)
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
	for(uint32 z = 0; z < seg_.z; ++z)
	for(uint32 x = 0; x < seg_.x; ++x)
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

	return MakeReference(geometry);
}
#pragma endregion


#pragma region
#pragma endregion


