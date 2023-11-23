#pragma once

#include "VertexBuffer.hpp"

/*
	The VAO is a GPU-side object that holds the state needed to supply vertex data to vertex shaders.
	The link between the attributes defined in the VAO and those in the shader is made through the attribute indices.
*/
class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vbo);
	void Bind() const;
	void Unbind() const;
};