#version 330 core
in vec2 uv;
in float vSize;
in float vRotation;
out vec4 FragColor;

uniform float time;

vec4 computeColor(vec2 p, float rotation) {
    float r = sqrt(dot(p,p));
    float a = atan( p.y, p.x ) + time*rotation;
    float s = 0.5 + 0.5*sin(3.0*a);
    float t = 0.085 + 0.35*pow(s,0.3);
    t += 0.1*pow(0.5 + 0.5*cos(6.0*a), 0.5);
    float h = r/t;
    float f = 0.0;
    if (h<1.0) f = 1.0;

    vec3 color = mix( vec3(0.0), vec3(0.5*h,0.5 + 0.5*h,0.0), f );
    return vec4(color, f);
}

void main() {
    vec2 p = uv * 2.0;
    FragColor = computeColor(p, vRotation);
}