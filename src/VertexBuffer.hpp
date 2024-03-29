#pragma once

#include <vector>
#include <stdexcept>

#include <glm/glm.hpp> // Include the main GLM header
#include <glm/gtc/matrix_transform.hpp> // Include for transformation functions

#include "Renderer.hpp"


struct Vertex 
{
	glm::vec2 Position;
	glm::vec2 TextureCoordinates;
	glm::vec4 Color;
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

	const std::vector<Vertex>& GetVertices() const
	{
		return m_Vertices;
	}

	void SetVertexData(std::vector<Vertex> vertices)
	{
		m_Vertices = std::move(vertices);
	}

	void SetVertexAttributes(std::vector<VertexAttribute> attributes)
	{
		m_VertexAttributes = std::move(attributes);
	}
};
