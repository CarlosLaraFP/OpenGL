#version 430 core

// Vertex attributes are the inputs that our vertex shader receives. 
// Each attribute represents a different aspect of our vertex data, such as position, color, texture coordinates, etc.
// The index 0 here is specifying the first vertex attribute, which the shader obtains from the VAO (after glVertexAttribPointer + enable).
// The attribute slots defined in the Vertex Array Object (VAO) need to match the vertex attributes in the shaders.

layout(location = 0) in vec2 position;

// rotation angle in degrees
uniform float u_Rotation;
        
void main()
{
    float rad = radians(u_Rotation);
    mat2 rotationMatrix = mat2(cos(rad), -sin(rad), sin(rad), cos(rad));
    gl_Position = vec4(rotationMatrix * position, 0.0, 1.0);
};