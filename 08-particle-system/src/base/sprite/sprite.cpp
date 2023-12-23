#include <iostream>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include "../graphics/renderer.hpp"
#include "../macros.hpp"
#include "sprite.hpp"

Sprite::Sprite(Renderer &renderer) : renderer(renderer) {
    this->vertices = std::vector<glm::vec4>();
    this->texture_positions = std::vector<glm::vec2>();
    this->origin = glm::vec4(0.0f);
    this->size = glm::vec4(1.0f);
    this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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

    this->texture_positions.push_back(glm::vec2(1.0f, 1.0f)); 
    this->texture_positions.push_back(glm::vec2(1.0f, 0.0f));
    this->texture_positions.push_back(glm::vec2(0.0f, 0.0f));

    this->texture_positions.push_back(glm::vec2(0.0f, 1.0f));
    this->texture_positions.push_back(glm::vec2(1.0f, 1.0f));
    this->texture_positions.push_back(glm::vec2(0.0f, 0.0f));
}

void Sprite::update() {
    for (int i = 0; i < this->vertices.size(); i++) {
        struct RendererData data = {
            .vertex = this->vertices[i],
            .origin = this->origin,
            .size = this->size,
            .color = this->color, 
            .tex = this->texture_positions[i], 
            .ctt_ratio = this->ctt
        }; 

        this->renderer.draw(data);
    }
}

Sprite::~Sprite() {

}
