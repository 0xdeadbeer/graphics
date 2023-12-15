#include <iostream>
#include <GL/glew.h>
#include "program.hpp"
#include "../macros.hpp"

Program::Program() {
    this->id = glCreateProgram();
}

int Program::add_shader(const char *path, unsigned int type) {
    Shader new_shader; 
    unsigned int ret = new_shader.load(path, type);

    if (!ret) 
        return RET_ERR;

    this->shaders.push_back(new_shader);

    return 0;
}

int Program::link() {
    for (int shader = 0; shader < this->shaders.size(); shader++) 
        glAttachShader(this->id, this->shaders[shader].id);
        
    glLinkProgram(this->id);

    int success; 
    char info_log[512]; 
    glGetProgramiv(this->id, GL_LINK_STATUS, &success); 

    if (!success) {
        glGetProgramInfoLog(this->id, 512, NULL, info_log); 
        return RET_ERR;
    }

    return 0;
}
