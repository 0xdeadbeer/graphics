#pragma once 
#include <vector>
#include <glm/vec3.hpp>
#include "texture.hpp"

class Game; 
class Texture; 

struct vertex {
    float position[3]; 
    float texture[2];
};

class Sprite {
public: 
    Texture texture;

    glm::vec3 pos; 
    glm::vec3 size;
    glm::vec3 rot;
    
    std::vector<struct vertex> vertices; 
    std::vector<int> indices;

    unsigned int vao; 
    unsigned int vbo; 
    unsigned int ebo;

    Game *game;

    Sprite(const char *path, enum TextureType type);
    void bake();
};
