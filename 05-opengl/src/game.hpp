#pragma once

#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include "object.hpp"

class Game {
public: 
    Game(GLFWwindow *window);
    static std::string read_file(const char *path);
    unsigned int load_shader(const char *path, unsigned int type);
    int link_program();
    void setup();
    void run(int initial_width, int initial_height);
    void logic();

    // callbacks 
    static void input_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void resize_callback(GLFWwindow *window, int new_width, int new_height);

    GLFWwindow *window;

    std::vector<Object*> objects;

    unsigned int vao; 
    unsigned int vbo; 
    unsigned int ebo;
    
    unsigned int vshader; 
    unsigned int fshader; 
    unsigned int program;
};
