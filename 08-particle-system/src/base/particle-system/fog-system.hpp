#pragma once 

#include "particle-system.hpp"

class FogSystem : public ParticleSystem {
public:
    FogSystem(Renderer &renderer, struct ParticleSystemData data) : ParticleSystem(renderer, data) {}
    ~FogSystem();

    void logic();
    void spawn_particle();
    inline void set_speed(float new_speed) {
        this->speed = new_speed;
    }

private: 
    float speed = 1.0f;
};
