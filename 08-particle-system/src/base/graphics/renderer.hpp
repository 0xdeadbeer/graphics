#pragma once 

#include <GL/glew.h>
#include <optional>
#include <glm/mat4x4.hpp>
#include "program.hpp"
#include "../window/window.hpp"

struct RendererData {
    glm::vec4 vertex; 
    glm::vec4 origin; 
    glm::vec4 size; 
    glm::vec4 color; 
    glm::vec2 tex; 
    float ctt_ratio; // color to texture ratio (i.e. how much does the collor affect the texture)
};

class Renderer {
public: 
    Renderer(Window &window);
    ~Renderer();

    int setup_attributes();
    int setup(std::optional<Program> program = std::nullopt);
    void logic();

    int load_texture(std::string location, GLenum format);
    void draw(struct RendererData data);
    void batch();

    inline static void blend() {
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    inline static void clear(glm::vec4 color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    std::vector<struct RendererData> batch_buffer;

private: 
    GLuint array_buffer_id;
    GLuint vertex_buffer_id;
    GLuint texture_id;

    glm::mat4 view_matrix; 
    glm::mat4 projection_matrix;

    Window &window;
    Program program;
};
