#pragma once

#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include "program.hpp"
#include "sprite.hpp"

class Game {
public: 
    Game(GLFWwindow *window);
    int setup(int initial_width, int initial_height);
    void run();
    void logic();
    void load(Sprite *sprite, int counter);
    void draw();

    // callbacks 
    static void input_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void resize_callback(GLFWwindow *window, int new_width, int new_height);

    GLFWwindow *window;
    Program program;

    std::vector<Sprite *> sprites;

    int texture_slots;
    unsigned int vao; 
    unsigned int vbo; 
    unsigned int ebo;
    unsigned int counter;
    unsigned int vbo_offset;
    unsigned int ebo_offset;
};
