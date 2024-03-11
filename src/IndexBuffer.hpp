#pragma once

#include <vector>

// Index Buffer Object (IBO)
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_IndexCount;
	std::vector<unsigned int> m_Indices;

public:
	IndexBuffer(const unsigned int* data, unsigned int indexCount, bool dynamic);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetIndexBuffer(std::vector<unsigned int> indices)
	{
		m_Indices = std::move(indices);
	}

	inline unsigned int GetCount() const { return m_IndexCount; }
};
