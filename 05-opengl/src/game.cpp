#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game.hpp"
#include "texture.hpp"
#include "sprite.hpp"
#include "other/stb_image.h"

Game::Game(GLFWwindow *window) {
    this->window = window;
    this->program = Program();

    stbi_set_flip_vertically_on_load(true);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &this->texture_slots);
}

// code this later lol
int Game::setup(int initial_width, int initial_height) {
    glfwSetKeyCallback(this->window, Game::input_callback);
    glfwSetFramebufferSizeCallback(this->window, Game::resize_callback);

    Game::resize_callback(this->window, initial_width, initial_height);

    int ret = this->program.add_shader("assets/shaders/vertex.glsl", GL_VERTEX_SHADER);
    if (ret != 0) {
        std::cout << "Error: failed adding vertex shader" << std::endl;
        return -1;
    }

    ret = this->program.add_shader("assets/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    if (ret != 0) {
        std::cout << "Error: failed adding fragment shader" << std::endl;
        return -1;
    }

    if (this->program.link() != 0) {
        std::cout << "Error: failed linking program" << std::endl; 
        return -1;
    }

    glUseProgram(this->program.id);

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

    return 0;
}

void Game::run() {
    this->counter = 0;
    this->vbo_offset = 0;
    this->ebo_offset = 0; 

    Sprite sprite("assets/player/idle.png", SPLIT_TEXTURE);
    sprite.bake();
    this->sprites.push_back(&sprite);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(struct vertex) * 4 * this->texture_slots, nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6 * this->texture_slots, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *) offsetof(vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *) offsetof(vertex, texture));
    glEnableVertexAttribArray(1);

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


        for (int sprite = 0; sprite < this->sprites.size(); sprite++, counter++) {
            Game::load(this->sprites[sprite], this->counter);

            std::cout << "Counter: " << counter << " Texture slots: " << this->texture_slots << std::endl;
            if (counter > 0 && counter % this->texture_slots == 0) {
                Game::draw();
                this->counter = 0;
                this->vbo_offset = 0;
                this->ebo_offset = 0; 
            }

        }

        Game::draw();
        this->counter = 0;
        this->vbo_offset = 0;
        this->ebo_offset = 0; 
        
        glfwPollEvents(); 
        glfwSwapBuffers(this->window);
    }

    glBindVertexArray(0);
}

void Game::load(Sprite *sprite, int count) {
    unsigned int program = this->program.id;
    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "sampler"), count);

    glBufferSubData(GL_ARRAY_BUFFER, vbo_offset, sizeof(struct vertex) * sprite->vertices.size(), sprite->vertices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, ebo_offset, sizeof(int) * sprite->indices.size(), sprite->indices.data());

    vbo_offset += sizeof(struct vertex) * sprite->vertices.size(); 
    ebo_offset += sizeof(int) * sprite->indices.size();
    
    glActiveTexture(GL_TEXTURE0 + count);
    glBindTexture(GL_TEXTURE_2D, sprite->texture.texture_id);
}

void Game::draw() {
    unsigned int program = this->program.id;
    glUseProgram(program);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::mat4(1.0f); 

    int width, height;
    float half_width, half_height;
    glfwGetWindowSize(this->window, &width, &height);

    half_width = (float) width / 2; 
    half_height = (float) height / 2; 

    projection = glm::ortho(-half_width/100, half_width/100, -half_height/100, half_height/100, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(program,"view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, this->sprites[0]->indices.size(), GL_UNSIGNED_INT, 0);
    std::cout << "Error: " << glGetError() << std::endl;

}
