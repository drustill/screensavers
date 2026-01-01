#version 330 core

layout (location = 0) in vec2 aQuadVertex;
layout (location = 1) in vec2 aParticlePos;
layout (location = 2) in vec4 aColor;

out vec2 vUV;
out vec4 vColor;

uniform float uSize;
uniform float uAspect;

void main() {
    vec2 scaled = aQuadVertex * uSize;
    scaled.x /= uAspect;
    vec2 worldPos = scaled + aParticlePos;
    gl_Position = vec4(worldPos, 0.0, 1.0);
    vUV = aQuadVertex + 0.5;
    vColor = aColor;
}