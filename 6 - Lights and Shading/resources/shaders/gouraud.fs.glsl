#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 LightColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 objectColor;

void main() {
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
    // float ambientStrength = 0.1;
    FragColor = vec4(LightColor * objectColor, 1.0);
}