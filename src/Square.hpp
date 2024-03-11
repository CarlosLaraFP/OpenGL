#pragma once

#include "Geometry.hpp"

class Square : public Geometry
{
public:
	Square();

	virtual void SetLayout() override;

	float m_Position[2] = { -0.5f, 0.0f };
	float m_RotationAngle { 0.0f };
    float m_RotationIncrement { 2.0f };

    void IncrementRotationAngle()
    {
        // Increase by 5 degrees per frame
        m_RotationAngle += m_RotationIncrement;
        // Wrap around if it exceeds 360 degrees
        if (m_RotationAngle >= 360.0f)
        {
            m_RotationAngle -= 360.0f;
        }
    }
};
