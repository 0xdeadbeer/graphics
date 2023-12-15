#pragma once 
#include <iostream>
#include <GLFW/glfw3.h>

class Window {
public: 
    int init(int width, int height, std::string title);

    static void resize_callback(GLFWwindow *window, int new_width, int new_height);
    void get_size(int *width, int *height);

    GLFWwindow *window;
};
