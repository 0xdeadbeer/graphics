#pragma once 

#include <iostream>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include "../graphics/renderer.hpp"

class Sprite {
public:
    Sprite(Renderer &renderer);
    ~Sprite();

    void bake();
    void update();

    glm::vec4 origin;
    glm::vec4 size;
    glm::vec4 color; 
    float ctt;

private: 
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec2> texture_positions;
    Renderer &renderer;
};
