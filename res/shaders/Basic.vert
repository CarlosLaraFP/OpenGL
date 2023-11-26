#version 430 core

// Vertex attributes are the inputs that our vertex shader receives. 
// Each attribute represents a different aspect of our vertex data, such as position, color, texture coordinates, etc.
// The index 0 here is specifying the first vertex attribute, which the shader obtains from the VAO (after glVertexAttribPointer + enable).
// The attribute slots defined in the Vertex Array Object (VAO) need to match the vertex attributes in the shaders.

layout(location = 0) in vec2 position; // vec4 for position, vec2 for rotation
layout(location = 1) in vec2 textureCoordinates;

// output data into the fragment shader
out vec2 v_TextureCoordinates;

uniform mat4 u_View;
// the vertex positions are multiplied by the projection matrix, transforming them into the correct space considering the aspect ratio
uniform mat4 u_ProjectionMatrix; // TODO: Model-View-Projection matrix
// rotation angle in degrees
uniform float u_Rotation;
        
void main()
{
    float rad = radians(u_Rotation);
    mat2 rotationMatrix = mat2(cos(rad), -sin(rad), sin(rad), cos(rad));
    // Matrix operations are non-commutative
    // The projection is meant to be the final transformation applied to bring the geometry into clip space.
    // This applies the rotation first to the model space vertices and then applies the projection transformation.
    gl_Position = u_ProjectionMatrix * u_View * vec4(rotationMatrix * position, 0.0, 1.0);
    // assign the vec2 we took in from our vertex buffer
    v_TextureCoordinates = textureCoordinates;
};