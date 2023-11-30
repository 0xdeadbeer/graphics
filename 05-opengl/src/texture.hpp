#pragma once 

class Texture {
public: 
    unsigned int texture_id; 

    Texture();
    void bind_texture(); 
    void load_data(unsigned char *bytes, int width, int height);
};
