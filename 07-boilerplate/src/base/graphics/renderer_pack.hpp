#pragma once 

#include <iostream>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

struct RendererData {
    glm::vec4 vertex; 
    glm::vec4 origin;
    glm::vec4 color; 
    glm::vec2 tex;
    float ctt_ratio; // color to texture ratio (i.e. how much does the color affect the texture)
};

class RendererPack {

public:
    std::vector<struct RendererData> data;
    RendererPack();
    ~RendererPack();
};
