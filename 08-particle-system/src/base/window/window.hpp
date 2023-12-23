#pragma once 
#include <iostream>
#include <GLFW/glfw3.h>

class Window {
public: 
    int init(int width, int height, std::string title);

    inline static void resize_callback(GLFWwindow *window, int new_width, int new_height) {
        glViewport(0, 0, new_width, new_height);
    }
    inline void set_keyboard_callback(void (*callback)(GLFWwindow *window, int key, int scancode, int action, int mode)) {
        glfwSetKeyCallback(this->window, callback);
    }
    inline void get_size(int *width, int *height) {
        glfwGetWindowSize(this->window, width, height);
    }
    inline bool should_close() {
        return glfwWindowShouldClose(this->window);
    }
    inline void end_frame() {
        glfwPollEvents();
        glfwSwapBuffers(this->window);
    }

    bool pressed_e = false;

private: 
    GLFWwindow *window; 
};
