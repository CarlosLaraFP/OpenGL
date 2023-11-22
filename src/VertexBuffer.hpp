#pragma once

// Vertex Buffer Object (VBO)
// Buffer objects are necessary to efficiently manage and store vertex data in GPU memory.
class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
