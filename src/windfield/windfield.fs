#version 330 core
in vec2 vLocalPos;
out vec4 FragColor;

void main() {
    float alpha = 1.0 - smoothstep(0.3, 0.5, abs(vLocalPos.y));
    FragColor = vec4(0.7, 0.75, 0.9, alpha);
}