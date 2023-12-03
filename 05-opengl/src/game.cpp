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
    return 0;
}

void Game::run() {
    Sprite sprite("assets/player/idle.png", SPLIT_TEXTURE);
    sprite.bake();
    this->sprites.push_back(&sprite);

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

        for (int i = 0; i < this->sprites.size(); i++) 
            Game::draw(this->sprites[i]);
        
        glfwPollEvents(); 
        glfwSwapBuffers(this->window);
    }

    glBindVertexArray(0);
}

void Game::draw(Sprite *sprite) {
    unsigned int program = this->program.id;
    glUseProgram(program);

    glUniform3i(glGetUniformLocation(program, "offset"), sprite->pos.x, sprite->pos.y, sprite->pos.z); 
    glUniform3f(glGetUniformLocation(program, "size"), sprite->size.x, sprite->size.y, sprite->size.z);
    glUniform1i(glGetUniformLocation(program, "texture1"), 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::mat4(1.0f); 

    int width, height;
    float half_width, half_height;
    glfwGetWindowSize(this->window, &width, &height);

    half_width = (float) width / 2; 
    half_height = (float) height / 2; 

    projection = glm::ortho(-half_width/100, half_width/100, -half_height/100, half_height/100, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(program,"view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(sprite->vao);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct vertex) * sprite->vertices.size(), sprite->vertices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(int) * sprite->indices.size(), sprite->indices.data());

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, sprite->texture.texture_id);

    glDrawElements(GL_TRIANGLES, sprite->indices.size(), GL_UNSIGNED_INT, 0);
}
