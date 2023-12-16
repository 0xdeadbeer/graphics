#include <iostream>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include "../base/graphics/renderer.hpp"
#include "sprite.hpp"

Sprite::Sprite(Renderer &renderer) : renderer(renderer) {
    this->vertices = std::vector<glm::vec4>();
    this->origin = glm::vec4(1.0f);
    this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->texture_position = glm::vec2(0.0f);
    this->ctt = 1.0f;
}

// prepare data
void Sprite::bake() { 
    this->vertices.push_back(glm::vec4(0.5f, 0.5f, 0.0f, 0.0f));
    this->vertices.push_back(glm::vec4(0.5f, -0.5f, 0.0f, 0.0f));
    this->vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.0f, 0.0f));

    this->vertices.push_back(glm::vec4(-0.5f, 0.5f, 0.0f, 0.0f));
    this->vertices.push_back(glm::vec4(0.5f, 0.5f, 0.0f, 0.0f));
    this->vertices.push_back(glm::vec4(-0.5f, -0.5f, 0.0f, 0.0f));
}

void Sprite::update() {
    for (int i = 0; i < this->vertices.size(); i++) {
        struct RendererData data = {
            .vertex = this->vertices[i],
            .origin = this->origin,
            .color = this->color, 
            .tex = this->texture_position, 
            .ctt_ratio = this->ctt
        }; 

        this->renderer.draw(data);
    }
}

Sprite::~Sprite() {

}
