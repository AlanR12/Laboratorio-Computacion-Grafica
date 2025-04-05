#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords; // Añadir esto (asumiendo que el VAO tiene las UVs en location 2)

out vec2 TexCoords; // Pasar al fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = texCoords; // Pasar las coordenadas de textura
    gl_Position = projection * view * model * vec4(position, 1.0f);
}