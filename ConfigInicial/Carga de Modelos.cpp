//Practica 6 Carga de Modelos 3D y Cámara Sintética
//Robles Reyes Alan
//14 de marzo del 2025
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
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 9.0f, 13.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Carga de modelos y camara sintetica - Alan Robles", nullptr, nullptr);

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

    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model moon((char*)"Models/Moon.obj");
    Model star((char*)"Models/star.obj");
    Model nave((char*)"Models/HN 48 Flying Car.obj");
    /*Model casco((char*)"Models/Motorcycle_Helmet_v01_L3.obj");
    Model helmet((char*)"Models/10503_Football_helmet_v1_L3.obj");
    Model inge((char*)"Models/10134_ConHelmet_v2_L3.obj");*/
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
  

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
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded models
        //Luna
        glm::mat4 moonModel(1);
        moonModel = glm::rotate(moonModel, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        moonModel = glm::rotate(moonModel, glm::radians(75.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        //moonModel = glm::translate(moonModel, glm::vec3(0.0f, 0.0f, 0.0f));
        moonModel = glm::scale(moonModel, glm::vec3(5.0f, 5.0f,5.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(moonModel));
        moon.Draw(shader);
        //Perro
        glm::mat4 dogModel(1);
        dogModel = glm::translate(dogModel, glm::vec3(0.0f, 9.23f, 0.0f));
        dogModel = glm::scale(dogModel, glm::vec3(1.5f, 1.5f, 1.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(dogModel));
        dog.Draw(shader);
        //Estrellas
        //1
        glm::mat4 starModel(1);
        starModel = glm::translate(starModel, glm::vec3(8.0f, 10.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel));
        star.Draw(shader);
        //2
        glm::mat4 starModel2(1);
        starModel2 = glm::translate(starModel2, glm::vec3(-8.0f, 10.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel2));
        star.Draw(shader);
        //3
        glm::mat4 starModel3(1);
        starModel3 = glm::translate(starModel3, glm::vec3(-13.0f, 18.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel3));
        star.Draw(shader);
        //4
        glm::mat4 starModel4(1);
        starModel4 = glm::translate(starModel4, glm::vec3(13.0f, 18.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel4));
        star.Draw(shader);
        //5
        glm::mat4 starModel5(1);
        starModel5 = glm::translate(starModel5, glm::vec3(0.0f, 18.0f, -10.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(starModel5));
        star.Draw(shader);
        //Nave
        glm::mat4 naveModel(1);
        naveModel = glm::rotate(naveModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        naveModel = glm::rotate(naveModel, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        naveModel = glm::translate(naveModel, glm::vec3(0.0f, 8.0f, -9.5f));
        naveModel = glm::scale(naveModel, glm::vec3(0.025f, 0.025f, 0.025f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(naveModel));
        nave.Draw(shader);
        ////Casco
        //glm::mat4 cascoModel(1);
        //cascoModel = glm::translate(cascoModel, glm::vec3(0.0f, 0.0f, 1.0f));
        //cascoModel = glm::scale(cascoModel, glm::vec3(10.0f, 10.0f, 10.0f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(cascoModel));
        //casco.Draw(shader);
        ////Helmet
        //glm::mat4 helmetModel(1);
        //helmetModel = glm::translate(helmetModel, glm::vec3(0.0f, 0.0f, -2.0f));
        //helmetModel = glm::scale(helmetModel, glm::vec3(0.01f, 0.01f, 0.01f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(helmetModel));
        //helmet.Draw(shader);
        ////Inge
        //glm::mat4 ingeModel(1);
        //ingeModel = glm::translate(ingeModel, glm::vec3(0.0f, 0.0f, 0.0f));
        //ingeModel = glm::scale(ingeModel, glm::vec3(3.0f, 3.0f, 3.0f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ingeModel));
        //inge.Draw(shader);


        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}