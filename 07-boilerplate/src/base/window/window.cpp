#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "window.hpp"
#include "../macros.hpp"

int Window::init(int width, int height, std::string title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (this->window == NULL) {
        LOG(LOG_ERR, "Failed creating window");
        glfwTerminate();
        return RET_ERR;
    }

    glfwMakeContextCurrent(this->window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG(LOG_ERR, "FAiled initializing Glew");
        return RET_ERR;
    }

    glfwSetFramebufferSizeCallback(this->window, this->resize_callback);

    return RET_OK;
}

void Window::resize_callback(GLFWwindow *window, int new_width, int new_height) {
    glViewport(0, 0, new_width, new_height);
}

void Window::get_size(int *width, int *height) {
    glfwGetWindowSize(this->window, width, height);
}
