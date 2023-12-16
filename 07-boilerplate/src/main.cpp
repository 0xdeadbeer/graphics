#include <iostream>
#include <fstream>
#include <cstdlib>
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

    std::vector<Sprite> sprites;

    int limit_x = 10; 
    int limit_y = 10;
    float scale = 0.2;

    for (int i = -limit_x/2; i < limit_x/2; i++) {
        for (int j = -limit_y/2; j < limit_y/2; j++) {
            Sprite new_sprite(main_renderer);
            new_sprite.bake();

            float r = (float) rand() / (float) RAND_MAX; 
            float g = (float) rand() / (float) RAND_MAX; 
            float b = (float) rand() / (float) RAND_MAX; 

            new_sprite.color = glm::vec4(r, g, b, 1.0f);
            new_sprite.origin = glm::vec4(i*scale, j*scale, 0.0f, 0.0f);

            sprites.push_back(new_sprite);
        }
    } 
    
    while (!glfwWindowShouldClose(window.window)) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < sprites.size(); i++) {
            // randomly update their position by little 
            float x = ((float) rand() / (float) RAND_MAX) * 0.5;
            float y = ((float) rand() / (float) RAND_MAX) * 0.5;

            int x_side = rand() % 2;
            int y_side = rand() % 2;

            if (x_side == 0)
                sprites[i].origin.x += x; 
            else 
                sprites[i].origin.x -= x; 

            if (y_side == 0) 
                sprites[i].origin.y += y; 
            else 
                sprites[i].origin.y -= y;


            sprites[i].update();
        }

        main_renderer.logic();
        main_renderer.batch();

        glfwPollEvents();
        glfwSwapBuffers(window.window);
    }


    glfwTerminate();
    return RET_OK;
}
