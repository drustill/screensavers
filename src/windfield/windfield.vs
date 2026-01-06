#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aGridPos;
layout(location = 2) in vec2 aDirection;

uniform float uArrowLength;
uniform float uArrowThickness;

out vec2 vLocalPos;

void main() {
    float angle = atan(aDirection.y, aDirection.x);
    float c = cos(angle);
    float s = sin(angle);
    mat2 rot = mat2(c, s, -s, c);
    
    vec2 scaled = vec2(aPos.x * uArrowLength, aPos.y * uArrowThickness);
    vec2 pos = rot * scaled;
    pos += aGridPos;
    
    vLocalPos = aPos;  
    gl_Position = vec4(pos, 0.0, 1.0);
}