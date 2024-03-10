#version 430 core
        
out vec4 color;

in vec2 v_TextureCoordinates;
in vec4 v_Color;
in float v_TextureID;

//uniform vec4 u_Color;
uniform sampler2D u_Textures[2];
        
void main()
{
    int index = int(v_TextureID);
    // Since the fragment shader runs for every pixel, we need to know the precise location in the texture
    color = texture(u_Textures[index], v_TextureCoordinates);
};