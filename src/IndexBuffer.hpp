#pragma once

// Index Buffer Object (IBO)
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_IndexCount;

public:
	IndexBuffer(const unsigned int* data, unsigned int indexCount);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_IndexCount; }
};
