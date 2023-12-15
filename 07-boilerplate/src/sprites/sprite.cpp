#include <iostream>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include "../base/graphics/renderer_pack.hpp"
#include "sprite.hpp"

Sprite::Sprite(GLuint vertex_buffer_id) {
    this->vertices = std::vector<glm::vec4>();
    this->origin = glm::vec4(1.0f);
    this->color = glm::vec4(1.0f);
    this->texture_position = glm::vec2(0.0f);
    this->ctt = 1.0f;
    this->vertex_buffer_id = vertex_buffer_id;
}

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

        glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
        glBufferSubData(GL_ARRAY_BUFFER, i*sizeof(struct RendererData), sizeof(struct RendererData), &data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

Sprite::~Sprite() {

}
