#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aVel;
layout(location = 2) in float aAge;

out float vSpeed;
out float vAge;
out vec2 vVel;

void main() {
    vSpeed = length(aVel);
    vAge = aAge;
    vVel = aVel;
    gl_Position = vec4(aPos, 0.0, 1.0);
}