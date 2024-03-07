#pragma once

#include "Geometry.hpp"

class Square : public Geometry
{
public:
	Square();

	virtual void SetLayout() override;

	void CreateDoubleSquares(const float originalVertices[], size_t originalSize, float spacing, std::vector<float>& newVertices);
};
