#version 430 core

// Vertex attributes are the inputs that our vertex shader receives. 
// Each attribute represents a different aspect of our vertex data, such as position, color, texture coordinates, etc.
// The index 0 here is specifying the first vertex attribute, which the shader obtains from the VAO (after glVertexAttribPointer + enable).
// The attribute slots defined in the Vertex Array Object (VAO) need to match the vertex attributes in the shaders.

layout(location = 0) in vec2 position; // vec4 for position, vec2 for rotation
layout(location = 1) in vec2 textureCoordinates;
layout(location = 2) in vec4 color;
layout(location = 3) in float textureID;

// output data into the fragment shader
out vec2 v_TextureCoordinates;
out vec4 v_Color;
out float v_TextureID;

// initial translation
uniform mat4 u_Model;
// rotation angle in degrees
//uniform float u_Rotation;
// camera transformation
uniform mat4 u_View;
// the vertex positions are multiplied by the projection matrix, transforming them into the correct space considering the aspect ratio
uniform mat4 u_ProjectionMatrix;
        
void main()
{
    // TODO fix: Entire vertex buffer is getting rotated rather than individual objects
    //float rad = radians(u_Rotation);
    //mat2 rotationMatrix = mat2(cos(rad), -sin(rad), sin(rad), cos(rad));
    // Model-View-Projection matrix multiplication on our vertex positions
    //gl_Position = u_ProjectionMatrix * u_View * u_Model * vec4(rotationMatrix * position, 0.0, 1.0);
    gl_Position = u_ProjectionMatrix * u_View * u_Model * vec4(position, 0.0, 1.0);
    v_TextureCoordinates = textureCoordinates;
    v_Color = color;
    v_TextureID = textureID;
};