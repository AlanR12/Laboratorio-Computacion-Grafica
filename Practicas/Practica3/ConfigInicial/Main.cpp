#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource =
{
	"#version 330 core\n"  // Cambiado a GLSL 3.30 (OpenGL 3.3)
	"layout (location = 0) in vec3 position;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
	"}\n"
};

const GLchar* fragmentShaderSource =
{
	"#version 330 core\n"  // Cambiado a GLSL 3.30 (OpenGL 3.3)
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"    color = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
	"}\n"
};

// Funciones de configuración del shader
void CrearShader();

int main() {
	// Inicializa GLFW
	glfwInit();

	// Configura el contexto de OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Usar perfil core
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Crea una ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 0 - OpenGL 3.3", nullptr, nullptr);

	// Verificación de errores de creación de ventana
	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Establece el contexto actual
	glfwMakeContextCurrent(window);

	// Inicializa GLEW
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos información de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Define las dimensiones del viewport
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	// Configura los shaders
	CrearShader();

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // Left
		0.5f, -0.5f, 0.0f,  // Right
		0.0f,  0.5f, 0.0f   // Top
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
	glBindVertexArray(0); // Unbind VAO

	// Bucle de renderizado
	while (!glfwWindowShouldClose(window)) {
		// Limpia el buffer de color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Dibuja el triángulo
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Intercambia los buffers
		glfwSwapBuffers(window);

		// Procesa eventos
		glfwPollEvents();
	}

	// Limpieza
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

void CrearShader() {
	// Creamos el vertex shader y guardamos su identificador
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Verificamos los errores en tiempo de compilación
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Creamos el fragment shader y guardamos su identificador
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Verificamos los errores en tiempo de compilación
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Creamos un nuevo programa y asignamos los shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Verificamos los errores en tiempo de enlace
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Usamos el programa
	glUseProgram(shaderProgram);

	// Eliminamos los shaders (ya están enlazados al programa)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}