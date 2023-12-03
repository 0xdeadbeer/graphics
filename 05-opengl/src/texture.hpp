#pragma once 

enum TextureType {
    FULL_TEXTURE, 
    SPLIT_TEXTURE, 
    DYNAMIC_TEXTURE
};

class Texture {
public: 
    unsigned int texture_id; 

    Texture(enum TextureType type);
    void bind_texture(); 
    void load_data(unsigned char *bytes, int width, int height, int cell_res);

    enum TextureType type;
    int texture_width; 
    int texture_height; 
    int cell_res;

    int cell_x; 
    int cell_y;
};
