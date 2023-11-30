#pragma once 

#include <vector>
#include "texture.hpp"

class Game;
class Texture; 

class Object {
public: 
    Object(Game *game, Texture texture); 

    void bake();
    void draw();

    Texture texture;
    int x; 
    int y; 

    std::vector<float> vertices; 
    std::vector<int> indices;

    unsigned int vao; 
    unsigned int vbo; 
    unsigned int ebo;

    Game *game;
};
