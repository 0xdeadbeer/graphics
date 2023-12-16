#pragma once 

#include <GL/glew.h>
#include <optional>
#include <glm/mat4x4.hpp>
#include "renderer_pack.hpp"
#include "program.hpp"
#include "../window/window.hpp"

class Renderer {
public: 
    GLuint array_buffer_id;
    GLuint vertex_buffer_id;
    Program program;
    RendererPack pack;

    glm::mat4 view_matrix; 
    glm::mat4 projection_matrix;

    Window &window;

    Renderer(Window &window);
    ~Renderer();

    int setup(std::optional<Program> program = std::nullopt);
    void logic();

    std::vector<struct RendererData> batch_buffer;

    void draw(struct RendererData data);
    void batch();
};
