#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include "game.hpp"
#include "sprite.hpp"

Sprite::Sprite(const char *path, enum TextureType type) : texture(type) {
    int image_width; 
    int image_height; 
    int image_resolution; 
    int image_channels; 

    unsigned char *image_bytes = stbi_load(path, &image_width, &image_height, &image_channels, 0);
    if (image_bytes == NULL) {
        std::cout << "Error: failed loading image data" << std::endl; 
        return;
    }

    this->texture.bind_texture();
    this->texture.load_data(image_bytes, image_width, image_height, image_height);

    stbi_image_free(image_bytes);

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    this->pos = glm::vec3(0.0f);
    this->size = glm::vec3(1.0f);
    this->rot = glm::vec3(0.0f);
}

void Sprite::bake() {
    struct vertex v1 = { {0.5f, 0.5f, 0.0f}, {1.0f, 1.0f} };
    struct vertex v2 = { {0.5f, -0.5f, 0.0f}, {1.0f, 0.0f} };
    struct vertex v3 = { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f} };
    struct vertex v4 = { {-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f} };

    this->vertices.push_back(v1);
    this->vertices.push_back(v2);
    this->vertices.push_back(v3);
    this->vertices.push_back(v4);

    this->indices = {
        0, 1, 3,
        1, 2, 3 
    };

    Texture &tex = this->texture;
    if (tex.type == SPLIT_TEXTURE) {
        for (int i = 0; i < this->vertices.size(); i++) {
            struct vertex *v = &this->vertices[i];

            float tex_x = ((tex.cell_x + v->texture[0]) * tex.cell_res) / tex.texture_width; 
            float tex_y = ((tex.cell_y + v->texture[1]) * tex.cell_res) / tex.texture_height; 

            v->texture[0] = tex_x; 
            v->texture[1] = tex_y;
        }
    }

    glBindVertexArray(this->vao); 

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(struct vertex), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *) offsetof(vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void *) offsetof(vertex, texture));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
