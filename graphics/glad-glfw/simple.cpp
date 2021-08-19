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
