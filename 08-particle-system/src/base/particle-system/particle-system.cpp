
#include "particle-system.hpp"

float randomize() {
    return (float) rand() / (float) RAND_MAX;
}

ParticleSystem::ParticleSystem(Renderer& renderer, struct ParticleSystemData data) : renderer(renderer) {
    this->data = data;
}

int ParticleSystem::setup() {
    int ret = this->renderer.load_texture(this->data.default_texture, GL_RGBA);
    if (ret != RET_OK)
        return RET_ERR;

    return RET_OK;
}

void ParticleSystem::logic() {
    for (int i = 0; i < this->particles.size(); i++) {
        Particle &particle = this->particles[i];
        Sprite &sprite = particle.sprite;

        sprite.origin += particle.force;
        particle.update();
    }

    for (int i = 0; i < (int) this->data.spawn_rate; i++) 
        this->spawn_particle();
}

glm::vec4 generate_origin(Rectangle rect) {
    glm::vec4 new_origin = glm::vec4(0.0f);

    new_origin.x = rect.x + (randomize() * rect.width);
    new_origin.y = rect.y + (randomize() * rect.height);

    return new_origin;
}

void ParticleSystem::spawn_particle() {
    Particle new_particle(this->renderer, this->data.default_force);

    glm::vec4 new_origin = generate_origin(this->data.spawn_area);

    new_particle.sprite.origin = new_origin;
    new_particle.sprite.size = this->data.default_size;
    new_particle.sprite.color = this->data.default_color;
    
    this->particles.push_back(new_particle);
}

ParticleSystem::~ParticleSystem() {

}
