#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform float time;

void main()
{
    float v = sin(uv.x * 10.0 + time);
    v += sin(uv.y * 10.0 + time * 0.5);
    v += sin((uv.x + uv.y) * 10.0 + time * 0.7);
    v += sin(length(uv - 0.5) * 20.0 - time);
    v /= 4.0;

    vec3 color = vec3(
        sin(v * 2 * 3.14159),
        sin(v * 2 * 3.14159 + 1.094),
        sin(v * 2 * 3.14159 + 2.188)
    );
    color = color * 0.5 + 0.5;

    FragColor = vec4(color, 1.0);
}