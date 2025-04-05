#version 330 core
out vec4 FragColor;

in vec2 TexCoords; // Recibir del vertex shader

uniform sampler2D texture_diffuse1; // Nombre debe coincidir con el uniform del modelo

void main() {
    FragColor = texture(texture_diffuse1, TexCoords); // Muestrear la textura
}