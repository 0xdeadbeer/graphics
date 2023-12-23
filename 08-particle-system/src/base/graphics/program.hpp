#pragma once 

#include <GL/glew.h>
#include <vector>
#include "shader.hpp"

struct Program {
    GLuint id;
    std::vector<Shader> shaders;

    Program();
    int add_shader(const char *path, unsigned int type);
    int link();
};
