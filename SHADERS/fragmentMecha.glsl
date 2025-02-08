#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;

void main()
{
    // Sample the textures (returns a vec4 with RGBA values)
    vec4 color1 = texture(texture_diffuse1, TexCoord);
    
    // Mix the result with the third texture
    FragColor = color1;
}

