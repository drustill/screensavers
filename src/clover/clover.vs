#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aGridPos;
layout(location = 2) in vec2 aDirection;

uniform float uAspectRatio;
uniform float uArrowLength;

void main() {
    float angle = atan(aDirection.y, aDirection.x);
    float c = cos(angle);
    float s = sin(angle);
    mat2 rot = mat2(c, s, -s, c);
    
    vec2 arrow = rot * (aPos * uArrowLength * length(aDirection));
    arrow.x /= uAspectRatio;
    
    vec2 pos = aGridPos + arrow;
    pos.x /= uAspectRatio;
    
    gl_Position = vec4(pos, 0.0, 1.0);
}