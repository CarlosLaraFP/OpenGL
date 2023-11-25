#version 430 core
        
out vec4 color;

in vec2 v_TextureCoordinates;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
        
void main()
{
    // Since the fragment shader runs for every pixel, we need to know the precise location in the texture
    vec4 textureColor = texture(u_Texture, v_TextureCoordinates);
    color = textureColor;
};