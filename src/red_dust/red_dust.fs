#version 330 core
in float vSpeed;
in float vAge;
in vec2 vVel;
out vec4 FragColor;

void main() {
    vec3 slow = vec3(0.4, 0.0, 0.0);
    vec3 fast = vec3(1.0, 0.1, 0.1);
    vec3 color = mix(slow, fast, clamp(vSpeed * 2.0, 0.0, 1.0));
    
    // specular? what is this
    vec3 lightDir = normalize(vec3(1.0, 1.0, 0.5));
    vec3 velDir = normalize(vec3(vVel, 0.3));
    float spec = pow(max(dot(velDir, lightDir), 0.0), 8.0);  // lower exponent = broader shine
    
    vec3 metallic = vec3(1.0, 0.8, 0.8) * spec * 1.2;
    color += metallic;
    
    float alpha = 1.0 - (vAge / 15.0);
    FragColor = vec4(color, alpha * 0.9);
}