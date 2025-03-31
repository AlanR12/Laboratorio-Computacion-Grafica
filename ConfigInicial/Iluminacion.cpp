//Practica 8 Ilustración
//Robles Reyes Alan
//28 de marzo del 2025
//115002762

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 1366, HEIGHT = 768;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 9.0f, 30.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos1(15.0f, 8.0f, 0.0f);
glm::vec3 lightPos2(15.0f, 8.0f, 0.0f); // Posición del segundo cubo
//float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;
// Light movement
float lightAngle = 0.0f;  // Ángulo inicial
float lightRadius = 15.0f;      // Radio del círculo (ajusta según necesites)
const glm::vec3 dogCenter(0.0f, 9.23f, 0.0f); // Centro del perro (posición fija según su modelo)
bool isMovingForward = false;           // Dirección del movimiento
glm::vec3 activeLightPos;               // Luz activa (pasa al shader)
// Light colors
glm::vec3 light1Color = glm::vec3(1.0f, 0.6f, 0.0f);  // Anaranjado
glm::vec3 light2Color = glm::vec3(0.2f, 0.2f, 1.0f);  // Azul
glm::vec3 activeLightColor; // Color activo

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Materiales e Iluminacion - Alan Robles", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model moon((char*)"Models/Moon.obj");
    Model star((char*)"Models/star.obj");
    Model nave((char*)"Models/HN 48 Flying Car.obj");
    Model sun((char*)"Models/sun.obj");
    Model tierra((char*)"Models/earth.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture, textureSun, textureEarth;
    //Texturas
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);

    // Textura del Sol
    glGenTextures(1, &textureSun);
    glBindTexture(GL_TEXTURE_2D, textureSun);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* sunImage = stbi_load("Models/sun_texture_512x512.png", &textureWidth, &textureHeight, &nrChannels, 0);
    if (sunImage) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sunImage);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load sun texture" << std::endl;
    }
    stbi_image_free(sunImage);

    // Textura de la Tierra
    glGenTextures(1, &textureEarth);
    glBindTexture(GL_TEXTURE_2D, textureEarth);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* earthImage = stbi_load("Models/Earth_Diffuse.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    if (earthImage) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, earthImage);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load earth texture" << std::endl;
    }
    stbi_image_free(earthImage);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        // Calcular nueva posición de la luz (movimiento circular en X/Y)
        /*lightPos.x = dogCenter.x + lightRadius * cos(lightAngle);
        lightPos.y = dogCenter.y + lightRadius * sin(lightAngle);*/
        
        float angleRadians = glm::radians(lightAngle);

        // Primer cubo (se mueve de 0° a 180°, luego se detiene)
        float cube1Angle = glm::min(lightAngle, 180.0f);
        lightPos1 = dogCenter + lightRadius * glm::vec3(cos(glm::radians(cube1Angle)), sin(glm::radians(cube1Angle)), 0.0f);

        // Segundo cubo (comienza en 0° cuando el primero llega a 180°)
        float cube2Angle = glm::max(lightAngle - 180.0f, 0.0f);
        lightPos2 = dogCenter + lightRadius * glm::vec3(cos(glm::radians(cube2Angle)), sin(glm::radians(cube2Angle)), 0.0f);

        // Determinar luz activa y su color
        if (isMovingForward) {
            activeLightPos = (lightAngle <= 180.0f) ? lightPos1 : lightPos2;
            activeLightColor = (lightAngle <= 180.0f) ? light1Color : light2Color;
        }
        else {
            activeLightPos = (lightAngle >= 180.0f) ? lightPos2 : lightPos1;
            activeLightColor = (lightAngle >= 180.0f) ? light2Color : light1Color;
        }

        lightingShader.Use();
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), activeLightPos.x, activeLightPos.y, activeLightPos.z);
        /*GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);*/
        //GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        //glUniform3f(lightPosLoc, lightPos.x + movelightPos, lightPos.y + movelightPos, lightPos.z + movelightPos);*/
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Set lights properties
        /*glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.6f, 0.6f, 0.6f);*/
        // Pasar propiedades de la luz con el color activo
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), activeLightColor.x * 0.3f, activeLightColor.y * 0.3f, activeLightColor.z * 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), activeLightColor.x * 0.7f, activeLightColor.y * 0.7f, activeLightColor.z * 0.7f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), activeLightColor.x * 0.6f, activeLightColor.y * 0.6f, activeLightColor.z * 0.6f);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
 

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.1f, 0.1f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 5.0f, 5.0f, 5.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);


        // Draw the loaded model
        //Luna
        glm::mat4 moonModel(1);
        moonModel = glm::rotate(moonModel, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        moonModel = glm::rotate(moonModel, glm::radians(75.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        moonModel = glm::scale(moonModel, glm::vec3(5.0f, 5.0f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(moonModel));
        moon.Draw(lightingShader);
        //Perro
        glm::mat4 dogModel(1);
        dogModel = glm::translate(dogModel, glm::vec3(0.0f, 9.23f, 0.0f));
        dogModel = glm::scale(dogModel, glm::vec3(1.5f, 1.5f, 1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(dogModel));
        dog.Draw(lightingShader);
        //Estrellas
        //1
        glm::mat4 starModel(1);
        starModel = glm::translate(starModel, glm::vec3(8.0f, 10.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel));
        star.Draw(lightingShader);
        //2
        glm::mat4 starModel2(1);
        starModel2 = glm::translate(starModel2, glm::vec3(-8.0f, 10.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel2));
        star.Draw(lightingShader);
        //3
        glm::mat4 starModel3(1);
        starModel3 = glm::translate(starModel3, glm::vec3(-13.0f, 18.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel3));
        star.Draw(lightingShader);
        //4
        glm::mat4 starModel4(1);
        starModel4 = glm::translate(starModel4, glm::vec3(13.0f, 18.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel4));
        star.Draw(lightingShader);
        //5
        glm::mat4 starModel5(1);
        starModel5 = glm::translate(starModel5, glm::vec3(0.0f, 18.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel5));
        star.Draw(lightingShader);
        //Nave
        glm::mat4 naveModel(1);
        naveModel = glm::rotate(naveModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        naveModel = glm::rotate(naveModel, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        naveModel = glm::translate(naveModel, glm::vec3(0.0f, 8.0f, -9.5f));
        naveModel = glm::scale(naveModel, glm::vec3(0.025f, 0.025f, 0.025f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(naveModel));
        nave.Draw(lightingShader);


        //glm::mat4 model(1);
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        //glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glBindVertexArray(VAO);
       //glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        glBindVertexArray(0);




        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //Cubo de la luz
        //Modelo Sol
        //moonModel = glm::mat4(1.0f);
        //moonModel = glm::translate(moonModel, lightPos1); // Posición de la luz
        //moonModel = glm::scale(moonModel, glm::vec3(0.8f)); // Escala pequeña
        glActiveTexture(GL_TEXTURE0); // Activar la unidad de textura 0
        glBindTexture(GL_TEXTURE_2D, textureSun); // Vincular textura del Sol
        glm::mat4 sunModel = glm::mat4(1.0f);
        sunModel = glm::translate(sunModel, lightPos1);
        sunModel = glm::scale(sunModel, glm::vec3(1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(sunModel));
        sun.Draw(lampshader);
        //Cubo
        /*glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);*/
        //Segundo Cubo
        //Modelo Tierra
        //moonModel = glm::mat4(1.0f);
        //moonModel = glm::translate(moonModel, lightPos2); // Posición de la luz
        //moonModel = glm::scale(moonModel, glm::vec3(0.3f)); // Escala pequeña
        glActiveTexture(GL_TEXTURE0); // Misma unidad de textura (o usa GL_TEXTURE1 si prefieres)
        glBindTexture(GL_TEXTURE_2D, textureEarth); // Vincular textura de la Tierra
        glm::mat4 earthModel = glm::mat4(1.0f);
        earthModel = glm::translate(earthModel, lightPos2);
        earthModel = glm::scale(earthModel, glm::vec3(1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(earthModel));
        tierra.Draw(lampshader);
        //Segundo Cubo
        /*glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);*/


        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }
    
    float speed = 50.0f; // Ajusta la velocidad

    if (keys[GLFW_KEY_O]) {
        lightAngle += speed * deltaTime; // Avance (0° -> 360°)
    }
    if (keys[GLFW_KEY_L]) {
        lightAngle -= speed * deltaTime; // Retroceso (360° -> 0°)
    }

    // Limitar el ángulo entre 0° y 360°
    lightAngle = glm::clamp(lightAngle, 0.0f, 360.0f);


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    //if (keys[GLFW_KEY_O]) {
    //lightAngle += 0.02f;  // Aumenta el ángulo (sentido antihorario)
    //}

    //if (keys[GLFW_KEY_L]) {
    //lightAngle -= 0.02f;  // Disminuye el ángulo (sentido horario)
    //}
    ////Restriccion
    //lightAngle = glm::clamp(lightAngle, glm::radians(0.0f), glm::radians(180.0f));
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}