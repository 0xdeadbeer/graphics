#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "game.hpp"
#include "texture.hpp"
#include "object.hpp"
#include "other/stb_image.h"

Game::Game(GLFWwindow *window) {
    this->window = window;
    this->program = glCreateProgram();
}

void Game::setup() {
}

void Game::run(int initial_width, int initial_height) {
    glfwSetKeyCallback(this->window, Game::input_callback);
    glfwSetFramebufferSizeCallback(this->window, Game::resize_callback);

    Game::resize_callback(this->window, initial_width, initial_height);

    this->vshader = Game::load_shader("assets/shaders/shader.vert", GL_VERTEX_SHADER);
    if (!this->vshader) 
        return; 

    this->fshader = Game::load_shader("assets/shaders/shader.frag", GL_FRAGMENT_SHADER);
    if (!this->fshader)
        return;

    if (Game::link_program())
        return;

    stbi_set_flip_vertically_on_load(true);

    int width; 
    int height; 
    int channels; 
    unsigned char *image_bytes = stbi_load("assets/wall.jpg", &width, &height, &channels, 0);
    if (image_bytes == NULL) {
        std::cout << "Error: failed loading image data" << std::endl; 
        return;
    }

    Texture new_texture; 
    new_texture.bind_texture();
    new_texture.load_data(image_bytes, width, height);

    stbi_image_free(image_bytes);

    Object new_object(this, new_texture);  

    new_object.vertices = {
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };

    new_object.indices = {
        0, 1, 3,
        1, 2, 3 
    };

    new_object.bake();
    this->objects.push_back(&new_object);

    Game::logic();
}

void Game::input_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) 
        return; 

    switch (key) {
        case GLFW_KEY_ESCAPE: 
            glfwSetWindowShouldClose(window, true);
            break;
        default: 
            break;
    }
}

void Game::resize_callback(GLFWwindow *window, int new_width, int new_height) {
    glViewport(0, 0, new_width, new_height);
}

void Game::logic() {
    while (!glfwWindowShouldClose(this->window)) {
        glClearColor(0, 0, 0, 1); 
        glClear(GL_COLOR_BUFFER_BIT); 

        for (int i = 0; i < this->objects.size(); i++) 
            (*this->objects[i]).draw();
        
        glfwPollEvents(); 
        glfwSwapBuffers(this->window);
    }

    glBindVertexArray(0);
}

unsigned int Game::load_shader(const char *path, unsigned int type) {
    std::string source = Game::read_file(path);
    const char *bytes = source.c_str(); 

    unsigned int new_shader = glCreateShader(type); 
    if (!new_shader) 
        return 0; 

    glShaderSource(new_shader, 1, &bytes, NULL); 
    glCompileShader(new_shader); 

    int success; 
    char info_log[512]; 
    glGetShaderiv(new_shader, GL_COMPILE_STATUS, &success); 

    if (!success) {
        glGetShaderInfoLog(new_shader, 512, NULL, info_log); 
        std::cout << "Error: failed compiling new shader " << info_log << std::endl; 
        return 0;
    }

    return new_shader;
}

int Game::link_program() {
    glAttachShader(this->program, this->vshader); 
    glAttachShader(this->program, this->fshader); 
    glLinkProgram(this->program);

    int success; 
    char info_log[512]; 
    glGetProgramiv(this->program, GL_LINK_STATUS, &success); 

    if (!success) {
        glGetProgramInfoLog(this->program, 512, NULL, info_log); 
        std::cout << "Error: failed linking program " << info_log << std::endl; 
        return -1; 
    }

    return 0;
}

std::string Game::read_file(const char *path) {
    std::string content;
    std::ifstream fs(path, std::ios::in);

    if(!fs.is_open()) {
        std::cerr << "Error: Could not read file " << path << ". File does not exist." << std::endl;
        return NULL;
    }

    std::string line = "";
    while(!fs.eof()) {
        std::getline(fs, line);
        content.append(line + "\n");
    }

    fs.close();
    return content;
}

