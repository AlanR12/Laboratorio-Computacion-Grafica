//Practica 4 Modelado Geometrico
//Robles Reyes Alan
//28 de febrero del 2025
//115002762

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=0.0f;
float movZ=-5.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado geometrico - Alan Robles", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificaci�n de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	float vertices[] = {
		//------------Gris Claro------------
		// Cara frontal
		-0.5f, -0.5f, 0.5f, 0.85f, 0.85f,0.85f,
		0.5f, -0.5f, 0.5f,  0.85f, 0.85f,0.85f,
		0.5f,  0.5f, 0.5f,  0.85f, 0.85f,0.85f,
		0.5f,  0.5f, 0.5f,  0.85f, 0.85f,0.85f,
		-0.5f,  0.5f, 0.5f, 0.85f, 0.85f,0.85f,
		-0.5f, -0.5f, 0.5f, 0.85f, 0.85f,0.85f,
		// Cara trasera
		-0.5f, -0.5f,-0.5f, 0.85f, 0.85f,0.85f,
		 0.5f, -0.5f,-0.5f, 0.85f, 0.85f,0.85f,
		 0.5f,  0.5f,-0.5f, 0.85f, 0.85f,0.85f,
		 0.5f,  0.5f,-0.5f, 0.85f, 0.85f,0.85f,
		-0.5f,  0.5f,-0.5f, 0.85f, 0.85f,0.85f,
		-0.5f, -0.5f,-0.5f, 0.85f, 0.85f,0.85f,
		// Cara derecha
		 0.5f, -0.5f,  0.5f,  0.85f, 0.85f,0.85f,
		 0.5f, -0.5f, -0.5f,  0.85f, 0.85f,0.85f,
		 0.5f,  0.5f, -0.5f,  0.85f, 0.85f,0.85f,
		 0.5f,  0.5f, -0.5f,  0.85f, 0.85f,0.85f,
		 0.5f,  0.5f,  0.5f,  0.85f, 0.85f,0.85f,
		 0.5f,  -0.5f, 0.5f, 0.85f, 0.85f,0.85f,
		 // Cara izquierda
		 -0.5f,  0.5f,  0.5f,  0.85f, 0.85f,0.85f,
		 -0.5f,  0.5f, -0.5f,  0.85f, 0.85f,0.85f,
		 -0.5f, -0.5f, -0.5f,  0.85f, 0.85f,0.85f,
		 -0.5f, -0.5f, -0.5f,  0.85f, 0.85f,0.85f,
		 -0.5f, -0.5f,  0.5f,  0.85f, 0.85f,0.85f,
		 -0.5f,  0.5f,  0.5f,  0.85f, 0.85f,0.85f,
		 // Cara superior
		 -0.5f, -0.5f, -0.5f, 0.85f, 0.85f,0.85f,
		 0.5f, -0.5f, -0.5f,  0.85f, 0.85f,0.85f,
		 0.5f, -0.5f,  0.5f,  0.85f, 0.85f,0.85f,
		 0.5f, -0.5f,  0.5f,  0.85f, 0.85f,0.85f,
		 -0.5f, -0.5f,  0.5f, 0.85f, 0.85f,0.85f,
		 -0.5f, -0.5f, -0.5f, 0.85f, 0.85f,0.85f,
		 // Cara inferior
		 -0.5f,  0.5f, -0.5f, 0.85f, 0.85f,0.85f,
		 0.5f,  0.5f, -0.5f,  0.85f, 0.85f,0.85f,
		 0.5f,  0.5f,  0.5f,  0.85f, 0.85f,0.85f,
		 0.5f,  0.5f,  0.5f,  0.85f, 0.85f,0.85f,
		 -0.5f,  0.5f,  0.5f, 0.85f, 0.85f,0.85f,
		 -0.5f,  0.5f, -0.5f, 0.85f, 0.85f,0.85f,
		 //---------------Negro----------------
		// Cara frontal
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,0.0f,
		0.5f, -0.5f, 0.5f,  0.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  0.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  0.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.5f, 0.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,0.0f,
		// Cara trasera
		-0.5f, -0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		 0.5f, -0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		-0.5f,  0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		-0.5f, -0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		// Cara derecha
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,0.0f,
		 // Cara izquierda
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 // Cara superior
		 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,
		 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,
		 // Cara inferior
		 -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,
		 -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,
	};


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	

		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO);
		
		//Cuerpo
	    model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.1f, 1.3f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//De frente
		//Pata Derecha
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(1.332f, -2.748f, -4.51f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Pata Izquierda
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(-1.332f, -2.748f, -4.51f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Fondo Ojo Derecho
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.1));
		model = glm::translate(model, glm::vec3(1.0f, 1.1f, -10.48f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Fondo Ojo Izquierdo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.1));
		model = glm::translate(model, glm::vec3(-1.0f, 1.1f, -10.48f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Ojo Derecho
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1));
		model = glm::translate(model, glm::vec3(2.8f, 3.0f, -10.478f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Ojo Izquierdo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1));
		model = glm::translate(model, glm::vec3(-2.8f, 3.0f, -10.478f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Fondo Nariz
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, -9.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Nariz
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.21f, 0.12f, 0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.17f, -9.791f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Brazo Derecho
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.09f, 0.6f, 0.6f));
		model = glm::translate(model, glm::vec3(6.62f, -0.584f, -2.834f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Brazo Izquierdo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.09f, 0.6f, 0.6f));
		model = glm::translate(model, glm::vec3(-6.62f, -0.584f, -2.834f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Oreja Derecha
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(1.332f, 3.2f, -5.509f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Oreja Derecha 2.0
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.3f));
		model = glm::translate(model, glm::vec3(1.75f, 3.2f, -3.84f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Oreja Izquierda
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(-1.332f, 3.2f, -5.509f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Oreja Izquierda 2.0
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.3f));
		model = glm::translate(model, glm::vec3(-1.75f, 3.2f, -3.84f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		//Cola
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.22f, 0.22f, 0.22f));
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, -9.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);


		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX += 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX -= 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		 movY += 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		 movY -= 0.002f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ -= 0.004f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ += 0.004f;
	 if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		 rot += 0.05f;
	 if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		 rot -= 0.05f; //0.008 rotacion lenta
 }