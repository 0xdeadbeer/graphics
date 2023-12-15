#include <iostream>
#include <GL/glew.h>
#include "shader.hpp"
#include "../common.hpp"
#include "../macros.hpp"

unsigned int Shader::load(const char *path, unsigned int type) {
    std::string source_s = read_file(path);
    const char *source = source_s.c_str();

    unsigned int new_shader = glCreateShader(type); 
    if (!new_shader) 
        return 0; 

    glShaderSource(new_shader, 1, &source, NULL); 
    glCompileShader(new_shader); 

    int success; 
    char info_log[512]; 
    glGetShaderiv(new_shader, GL_COMPILE_STATUS, &success); 

    if (!success) {
        glGetShaderInfoLog(new_shader, 512, NULL, info_log); 
        std::cout << "Error: failed compiling new shader " << info_log << std::endl; 
        LOG(LOG_ERR, "Failed compiling new shader");
        return RET_OK;
    }

    this->id = new_shader; 
    this->type = type;

    return new_shader;
}
