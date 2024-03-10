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
	unsigned int componentCount;
	unsigned int componentType;
	unsigned char normalized;
	unsigned int offset;

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
	void EnableVertexAttributes() const;
	std::vector<Vertex> m_Vertices;

public:
	VertexBuffer(const void* data, unsigned int size, bool dynamic);
	~VertexBuffer();

	void Bind();
	void Unbind() const;

	template<size_t N>
	void SetVertexData(const Vertex(&vertices)[N])
	{
		// TODO: Move a vector into this method instead
		m_Vertices.clear();
		m_Vertices.reserve(N);

		// TODO: Implement more efficiently
		for (size_t i = 0; i < N; ++i)
		{
			m_Vertices.push_back(vertices[i]); // Copy each element
		}
	}

	void SetVertexAttributes(std::vector<VertexAttribute> attributes)
	{
		m_VertexAttributes = std::move(attributes);
	}

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

	//inline const std::vector<VertexAttribute>& GetVertexAttributes() const { return m_VertexAttributes; }
	inline unsigned int GetStride() const { return m_Stride; }
};
