#include <iostream>
#include <fstream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "base/window/window.hpp"
#include "base/graphics/renderer.hpp"
#include "base/particle-system/particle-system.hpp"
#include "base/particle-system/fog-system.hpp"
#include "base/macros.hpp"

const int initial_height = 600; 
const int initial_width = 1000; 
Window main_window; 

void _keyboard(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (action != GLFW_PRESS) 
        return; 

    switch (key) {
        case GLFW_KEY_E: 
            LOG(LOG_DEBUG, "PRESSING E!");
            main_window.pressed_e = !main_window.pressed_e;
            break;
        default: 
            break;
    }
}

int main() {
    int ret;

    ret = main_window.init(initial_width, initial_height, "OpenGL");
    if (ret != RET_OK) {
        LOG(LOG_ERR, "Failed initializing GLFW window");
        return RET_ERR;
    }

    main_window.set_keyboard_callback(_keyboard);

    Renderer main_renderer(main_window);
    ret = main_renderer.setup();
    if (ret != RET_OK) {
        LOG(LOG_ERR, "Failed setting up a new renderer");
        return RET_ERR;
    }

    ParticleSystemData ps_data = {
        .spawn_rate = 20,
        .default_force = glm::vec4(0.0f, 0.1f, 0.0f, 0.0f),
        .default_color = glm::vec4(0.0f, 0.8f, 0.4f, 0.4f),
        .default_size = glm::vec4(0.3f, 0.3f, 0.1f, 0.1f),
        .spawn_area = {
            .x = -120.0f,
            .y = -240.0f, 
            .width = 220.0f, 
            .height = 220.0f
        },
        .default_texture = "assets/particle.png"
    }; 

    FogSystem ps(main_renderer, ps_data);
    ps.setup();

    while (!main_window.should_close()) {
        Renderer::clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        if (main_window.pressed_e) 
            ps.set_speed(10.0f);
        else 
            ps.set_speed(1.0f);
    
        ps.logic();

        main_renderer.logic();
        main_renderer.batch();

        main_window.end_frame();
    }


    glfwTerminate();
    return RET_OK;
}
