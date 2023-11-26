#pragma once

#include "Geometry.hpp"

class Square : public Geometry
{
public:
	Square();

	virtual void SetLayout() override;
};
