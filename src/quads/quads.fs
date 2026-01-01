#version 330 core

in vec2 vUV;
in vec4 vColor;
out vec4 FragColor;

void main() {
    vec2 center = vUV - vec2(0.5);
    float dist = length(center);
    // float alpha = 1.0 - step(0.5, dist);
    // float alpha = dist < 0.5 ? 1.0 : 0.0;
    float alpha = 1.0 - smoothstep(0.25, 0.5, dist);
    
    FragColor = vec4(vColor.rgb, vColor.a * alpha);
}