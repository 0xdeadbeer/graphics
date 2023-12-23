#include "particle-system.hpp"

Particle::Particle(Renderer &renderer, glm::vec4 force = glm::vec4(0.0f)) : sprite(renderer) {
    this->force = force;
    this->sprite.bake();
    this->time = 0.0f;
}

void Particle::update() {
    this->time += 0.0001f;
    this->sprite.update();
}

Particle::~Particle() {

}
