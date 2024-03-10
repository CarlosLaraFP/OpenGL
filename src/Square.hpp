#pragma once

#include "Geometry.hpp"

class Square : public Geometry
{
public:
	Square();

	virtual void SetLayout() override;

	float m_Position[2] = { -0.5f, 0.0f };
};
