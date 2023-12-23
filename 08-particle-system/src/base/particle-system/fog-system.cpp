#include <glm/vec4.hpp>
#include "particle-system.hpp"
#include "fog-system.hpp"

extern glm::vec4 generate_origin(Rectangle rect);
extern float randomize();

void FogSystem::logic() {
    for (int i = 0; i < this->particles.size(); i++) {
        Particle &particle = this->particles[i];
        Sprite &sprite = particle.sprite;

        sprite.origin += particle.force * this->speed;
        particle.update();
    }

    for (int i = 0; i < (int) this->data.spawn_rate; i++) 
        this->spawn_particle();
}

void FogSystem::spawn_particle() {
    glm::vec4 new_force = this->data.default_force; 
    new_force.x = (randomize() - 0.5f) * 0.02f;
    new_force.y = randomize() * new_force.y;

    Particle new_particle(this->renderer, new_force);

    glm::vec4 new_origin = generate_origin(this->data.spawn_area);

    new_particle.sprite.origin = new_origin;
    new_particle.sprite.size = this->data.default_size;
    new_particle.sprite.color = this->data.default_color;
    
    this->particles.push_back(new_particle);
}

FogSystem::~FogSystem() {
}
