#pragma once

#include <vector>

#include "Renderer.hpp"

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
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	template<typename T>
	void Push(unsigned int count);

	inline const std::vector<VertexAttribute>& GetVertexAttributes() const { return m_VertexAttributes; }
	inline unsigned int GetStride() const { return m_Stride; }
	void Bind() const;
	void Unbind() const;
};
