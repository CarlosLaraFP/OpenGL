#pragma once

#include "Geometry.hpp"

class Square : public Geometry
{
private:
	glm::mat4 m_ModelMatrix;

public:
	Square(glm::mat4 modelMatrix);

	virtual void SetLayout() override;
};
