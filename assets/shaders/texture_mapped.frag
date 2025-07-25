#version 330
in vec2 TexCoord;
uniform sampler2D ourTexture;
layout (location=0) out vec4 FragColor;

void main() {
    FragColor = texture(ourTexture, TexCoord);
}