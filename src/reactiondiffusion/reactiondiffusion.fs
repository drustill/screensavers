#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D textureA;
uniform sampler2D textureB;

void main()
{
    float a = texture(textureA, TexCoord).r;
    float b = texture(textureB, TexCoord).r;
    FragColor = vec4(b, b, b, 1.0);
}