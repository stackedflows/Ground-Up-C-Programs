#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/*
    opengl: everything is in 3d space
        0: change 3d co-ords to 2d coords
        1: transforms 3d co-ords to pixels
*/

// install glad/glfw with https://www.youtube.com/watch?v=XpBGwZNyUh0&t=43s&ab_channel=VictorGordan
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// copy paste https://github.com/datenwolf/linmath.h/blob/master/linmath.h
#include "linmath.h"

using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// handles window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// handles "esc" key press
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// define triangle verticies
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void verticiesToGPU(const int VBO) {
    // bind array buffer to object, as this is the type needed for vbo
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // now we can assign the verticies to the buffer, static draw: set once, used many times vs dynamic draw, where the buffer data will change often
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

// inline shader program in glsl, gl shader language
const char* vertexShaderSource =
"// specify version\n"
"#version 330 core\n"
"// declare vertex attributes with in keyword, set the item as a 3 vector as each vertex has 3 values\n"
"layout (location = 0) in vec3 aPos;\n"
"// translate input data into gl readable, do stuff with it\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"};\0"
;

// inline fragment shader prog. in glsl, colours/alpha must be between 0,1. Output must be a single vector4
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0"
;

// checks for errors in the vertex shader source code
void checkShaderCompilerErrors(unsigned int shader) {
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

// compile vertex shader and check for errors
void vertexShaderCompile(const int vertexShader) {
    // attach shader source code to shader, compile
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for errors in shader compilation
    checkShaderCompilerErrors(vertexShader);
}

// compile fragment shader to gpu
void fragmentShaderCompile(const int fragmentShader) {
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompilerErrors(fragmentShader);
}

// checks for errors in linker
void checkLinker(unsigned int linkProgram) {
    int  success;
    char infoLog[512];
    glGetProgramiv(linkProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(linkProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void shaderProgramLink(unsigned int shaderProgram, unsigned int vertexShader, unsigned int fragmentShader) {
    // attach programs to linker
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check to see if linker has worked
    checkLinker(shaderProgram);

    // now we can use the linked shaders
    glUseProgram(shaderProgram);

    // and remove the unused shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


int main() {
    // initialise glfw, check it is using the correct version (3)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // open window, check for errors, set as current context
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // load glad, manages function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // assign gl viewport to glfw window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //// - - - now to storing the vertex data on gpu . . .
    
    // create vertex buffer object and assign to it a single unit of memory locally
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // bind verticies to correct api data type, send to gpu
    verticiesToGPU(VBO);

    //// - - - now we can set up a vertex shader and process the array on gpu, and compile it as followed . . .

    // create shader object, with local reference
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // compile the vertex shader, and check for errors
    vertexShaderCompile(vertexShader);

    //// - - - now for the fragment shader, which colours the shape we will create

    // we create and compile the fragment shader in a similar way to the vertex shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShaderCompile(fragmentShader);

    //// - - - combined shader program: we need to link the above two programs into one: following which, opengl now knows how to process vertex and fragment data

    // create link program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // links the vertex shader and fragment shader into single program
    shaderProgramLink(shaderProgram, vertexShader, fragmentShader);

    // render window context while not exit pressed
    while (!glfwWindowShouldClose(window)){
        // check for user input
        processInput(window);

        // rendering commands here ... 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // swap color buffers, check if events are triggered
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // end program
    glfwTerminate();
    return 0;
}
