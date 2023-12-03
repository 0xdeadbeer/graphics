#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "game.hpp"

int initial_width = 1000; 
int initial_height = 600; 
std::string vertex_shader_location = "assets/shaders/shader.vert";
std::string fragment_shader_location = "assets/shaders/shader.frag";

int main() {
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    GLFWwindow *window = glfwCreateWindow(initial_width, initial_height, "OpenGL", NULL, NULL); 
    if (window == NULL) {
        std::cout << "Error: failed creating window" << std::endl; 
        glfwTerminate();
        return -1; 
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Error: failed initializing glew " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    Game main_game(window);
    main_game.setup(initial_width, initial_height);
    main_game.run();

    glfwTerminate();
    return 0;
}
