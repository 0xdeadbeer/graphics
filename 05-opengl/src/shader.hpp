#pragma once 

struct Shader {
     unsigned int id; 
     unsigned int type; 

     unsigned int load(const char *path, unsigned int type);
};
