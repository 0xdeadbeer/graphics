#pragma once 

#include <iostream>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include "../base/graphics/renderer.hpp"

class Sprite {
public:
    std::vector<glm::vec4> vertices;
    glm::vec4 origin;
    glm::vec4 color; 
    glm::vec2 texture_position;
    float ctt;

    // GLuint vertex_buffer_id;
    Renderer &renderer;

    Sprite(Renderer &renderer);
    ~Sprite();

    void bake();
    void update();
};
