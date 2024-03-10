#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
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

	template<size_t N> // non-type template parameter
	void SetVertexData(const Vertex(&vertices)[N])
	{
		vbo->SetVertexData(vertices);
	}

	void Bind()
	{
		/*
			When we configure a VAO and subsequently use it in a draw call, OpenGL uses the state information stored in the VAO
			to set up the necessary inputs for the vertex shader. This setup includes information about where in the GPU memory
			the vertex data is, how it's formatted, and how it should be fed into the vertex shader's inputs.
		*/
		// Tells OpenGL that the VAO with the given ID is now in use, prompting the GPU to allocate memory and initialize the state for this VAO.
		vao->Bind();
		vbo->Bind();
		ibo->Bind();
		material->Bind();
	};

	void SetTranslationVector(glm::vec3 vector) { material->SetTranslationVector(vector); }

	inline unsigned int GetIndexCount() const { return ibo->GetCount(); }

protected:
	VertexArray* vao = nullptr;
	VertexBuffer* vbo = nullptr;
	IndexBuffer* ibo = nullptr;
	Material* material = nullptr;
};
