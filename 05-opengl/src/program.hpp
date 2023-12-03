#pragma once 

#include <vector>
#include "shader.hpp"

struct Program {
    unsigned int id;
    std::vector<Shader> shaders;

    Program();
    int add_shader(const char *path, unsigned int type);
    int link();
};
