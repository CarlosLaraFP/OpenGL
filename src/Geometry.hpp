#pragma once

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Material.hpp"

// abstract class
class Geometry
{
public:
	virtual ~Geometry()
	{
		delete material;
		delete ibo;
		delete vbo;
		delete vao;
	};

	virtual void SetLayout() = 0;

	void Bind()
	{
		/*
			Binding VAO is enough because it already encapsulates the VBO state.
			When we configure a VAO and subsequently use it in a draw call, OpenGL uses the state information stored in the VAO
			to set up the necessary inputs for the vertex shader. This setup includes information about where in the GPU memory
			the vertex data is, how it's formatted, and how it should be fed into the vertex shader's inputs.
		*/
		vao->Bind();
		ibo->Bind();
		material->Bind();
	};

	void SetModelMatrix(glm::mat4 modelMatrix) { material->SetModelMatrix(modelMatrix); }

	inline unsigned int GetIndexCount() const { return ibo->GetCount(); }

protected:
	VertexArray* vao = nullptr;
	VertexBuffer* vbo = nullptr;
	IndexBuffer* ibo = nullptr;
	Material* material = nullptr;
};
