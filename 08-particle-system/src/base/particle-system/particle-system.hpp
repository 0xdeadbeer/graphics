#pragma once 

#include <iostream>
#include <vector>
#include "../graphics/renderer.hpp"
#include "../sprite/sprite.hpp"
#include "../macros.hpp"
#include <glm/vec4.hpp>

class Particle {
public: 
    Particle(Renderer &renderer, glm::vec4 force);
    ~Particle();

    void update();

    Sprite sprite;
    glm::vec4 force;
    float time;
};

struct Rectangle {
    float x; 
    float y; 
    float width; 
    float height;
};

struct ParticleSystemData {
    float spawn_rate; // spawn per call 
    glm::vec4 default_force;
    glm::vec4 default_color;
    glm::vec4 default_size;
    struct Rectangle spawn_area;
    std::string default_texture;
};

class ParticleSystem {
public:
    ParticleSystem(Renderer &renderer, struct ParticleSystemData data);
    ~ParticleSystem();

    int setup();
    virtual void logic();
    virtual void spawn_particle();

protected: 
    Renderer &renderer; 
    struct ParticleSystemData data;
    std::vector<Particle> particles;
};
