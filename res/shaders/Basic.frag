#version 430 core
        
out vec4 color;

in vec2 v_TextureCoordinates;
in vec4 v_Color;
in float v_TextureID; // TODO bug fix: Always comes in as zero

//uniform vec4 u_Color;
uniform sampler2D u_Textures[2];
        
void main()
{
    int index = int(v_TextureID);
    color = texture(u_Textures[index], v_TextureCoordinates);

    // Since the fragment shader runs for every pixel, we need to know the precise location in the texture
    //vec4 textureColor = texture(u_Texture, v_TextureCoordinates);
    //color = textureColor;
    //color = vec4(1.0, 1.0, 1.0, 1.0);
    //color = v_Color;
};