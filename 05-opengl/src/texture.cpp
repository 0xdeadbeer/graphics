#include <GL/glew.h>
#include "texture.hpp"

Texture::Texture(enum TextureType type) {
    glGenTextures(1, &this->texture_id);
    this->type = type;
    this->cell_x = 0; 
    this->cell_y = 0;
    this->cell_res = 0; 
}

void Texture::bind_texture() {
    glBindTexture(GL_TEXTURE_2D, this->texture_id); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
}

void Texture::load_data(unsigned char *bytes, int width, int height, int cell_res) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    this->texture_width = width; 
    this->texture_height = height; 
    this->cell_res = cell_res;
}
