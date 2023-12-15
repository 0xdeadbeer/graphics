#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "base/window/window.hpp"
#include "base/graphics/renderer.hpp"
#include "base/macros.hpp"
#include "sprites/sprite.hpp"

const int initial_width = 1000; 
const int initial_height = 600; 

int main() {
    Window window; 
    window.init(initial_width, initial_height, "OpenGL");

    Renderer main_renderer(window);
    int ret = main_renderer.setup();
    if (ret != RET_OK) {
        LOG(LOG_ERR, "Failed setting up a new renderer");
        return RET_ERR;
    }

    Sprite new_sprite(main_renderer.vertex_buffer_id);
    new_sprite.bake();

    new_sprite.origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    new_sprite.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window.window)) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        new_sprite.update();
        main_renderer.logic();
        main_renderer.render();

        glfwPollEvents();
        glfwSwapBuffers(window.window);
    }


    glfwTerminate();
    return RET_OK;
}
