#version 330 core

layout (location = 0) in vec2 aQuadVertex;
layout (location = 1) in vec2 aCloverPos;
layout (location = 2) in float aSize;
layout (location = 3) in float aRotation;

out vec2 uv;
out float vSize;
out float vRotation;

uniform float uAspectRatio;

void main() {
    vec2 p = aQuadVertex * aSize;
    p.x /= uAspectRatio;
    p += aCloverPos;
    gl_Position = vec4(p, 0.0, 1.0);
    uv = aQuadVertex;
    vSize = aSize;
    vRotation = aRotation;
}