#pragma once

#include <vector>
#include <stdexcept>

#include "Renderer.hpp"

struct Vec2
{
	float a, b;
};

struct Vec3
{
	float a, b, c;
};

struct Vec4
{
	float a, b, c, d;
};

struct Vertex 
{
	Vec2 Position;
	Vec2 TextureCoordinates;
	Vec4 Color;
	float TextureID;
};

struct VertexAttribute
{
	unsigned int componentType;
	unsigned int componentCount;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:         return 4;
		case GL_UNSIGNED_INT:  return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}

		ASSERT(false);
		return 0;
	}
};

// Vertex Buffer Object (VBO)
// Buffer objects are necessary to efficiently manage and store vertex data in GPU memory.
// We assume a 1:1 correspondence between a VBO and a layout.
class VertexBuffer
{
private:
	unsigned int m_RendererID;
	std::vector<VertexAttribute> m_VertexAttributes;
	unsigned int m_Stride { 0 };

public:
	VertexBuffer(const void* data, unsigned int size, bool dynamic);
	~VertexBuffer();

	template<typename T>
	void Push(unsigned int count)
	{
		// Compile-time assertion unsupported in VS2022
		throw std::runtime_error("Unsupported type for VertexBuffer::Push");
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_VertexAttributes.emplace_back(VertexAttribute{ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexAttribute::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_VertexAttributes.emplace_back(VertexAttribute{ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexAttribute::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_VertexAttributes.emplace_back(VertexAttribute{ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexAttribute::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	void AddVertices();
	std::array<Vertex, 4> CreateQuad(float x, float y, float t);

	//inline const std::vector<VertexAttribute>& GetVertexAttributes() const { return m_VertexAttributes; }
	inline unsigned int GetStride() const { return m_Stride; }
	void Bind();
	void Unbind() const;
};
