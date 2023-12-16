#include <optional>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "renderer.hpp"
#include "../macros.hpp"
#include "graphics.hpp"

Renderer::Renderer(Window &window) : window(window) {
    pack = RendererPack();
    this->window = window;
    
    glGenBuffers(1, &this->vertex_buffer_id);
    glGenVertexArrays(1, &this->array_buffer_id);

    this->view_matrix = glm::mat4(1.0f);
    this->projection_matrix = glm::mat4(1.0f);
}

int Renderer::setup(std::optional<Program> program) {
    int ret;

    if (program != std::nullopt)
        this->program = program.value();
    else {
        Program new_program = Program();

        ret = new_program.add_shader("assets/shaders/default_vertex.glsl", GL_VERTEX_SHADER);
        if (ret != RET_OK) {
            LOG(LOG_ERR, "Failed adding vertex shader to program");
            return RET_ERR;
        }

        ret = new_program.add_shader("assets/shaders/default_fragment.glsl", GL_FRAGMENT_SHADER);
        if (ret != RET_OK) {
            LOG(LOG_ERR, "Failed adding fragment shader to program");
            return RET_ERR;
        }

        ret = new_program.link();
        if (ret != RET_OK) {
            LOG(LOG_ERR, "Failed linking program");
            return RET_ERR;
        }

        this->program = new_program;
    }

    glBindVertexArray(this->array_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);

    glBufferData(GL_ARRAY_BUFFER, sizeof(struct RendererData) * DEFAULT_VBO_SIZE, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        DEFAULT_POSITION_ATTRIBUTE, 
        4, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(struct RendererData),
        (void *) offsetof(RendererData, vertex)
    );
    glEnableVertexAttribArray(DEFAULT_POSITION_ATTRIBUTE);

    glVertexAttribPointer(
        DEFAULT_ORIGIN_ATTRIBUTE,
        4, 
        GL_FLOAT, 
        GL_FALSE, 
        sizeof(struct RendererData), 
        (void *) offsetof(RendererData, origin)
    );
    glEnableVertexAttribArray(DEFAULT_ORIGIN_ATTRIBUTE);

    glVertexAttribPointer(
        DEFAULT_COLOR_ATTRIBUTE, 
        4, 
        GL_FLOAT,
        GL_FALSE,
        sizeof(struct RendererData), 
        (void *) offsetof(RendererData, color)
    );
    glEnableVertexAttribArray(DEFAULT_COLOR_ATTRIBUTE);

    glVertexAttribPointer(
        DEFAULT_TEXTURE_ATTRIBUTE, 
        2, 
        GL_FLOAT,
        GL_FALSE,
        sizeof(struct RendererData), 
        (void *) offsetof(RendererData, tex)
    );
    glEnableVertexAttribArray(DEFAULT_TEXTURE_ATTRIBUTE);

    glVertexAttribPointer(
        DEFAULT_CTT_ATTRIBUTE, 
        1, 
        GL_FLOAT,
        GL_FALSE,
        sizeof(struct RendererData), 
        (void *) offsetof(RendererData, ctt_ratio)
    );
    glEnableVertexAttribArray(DEFAULT_CTT_ATTRIBUTE);

    // setup matrices 
    this->view_matrix = glm::translate(this->view_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
    
    return RET_OK;
}

void Renderer::draw(struct RendererData data) {
    if (batch_buffer.size() >= DEFAULT_VBO_SIZE) {
        LOG(LOG_WARN, "Batch buffer limit reached!");
        return;
    }

    batch_buffer.push_back(data);
}

void Renderer::batch() {
    glBindVertexArray(this->array_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);

    glBufferSubData(GL_ARRAY_BUFFER, 0, batch_buffer.size() * sizeof(struct RendererData), batch_buffer.data());

    glUseProgram(this->program.id);

    glUniformMatrix4fv(glGetUniformLocation(this->program.id, "view_matrix"), 1, GL_FALSE, &this->view_matrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(this->program.id, "projection_matrix"), 1, GL_FALSE, &this->projection_matrix[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, DEFAULT_VBO_SIZE);

    batch_buffer.clear();
}

void Renderer::logic() {
    int window_width; 
    int window_height; 
    float half_width; 
    float half_height; 

    this->window.get_size(&window_width, &window_height);

    half_width = window_width / 2.0f;
    half_height = window_height / 2.0f;

    this->projection_matrix = glm::ortho(-half_width/100, half_width/100, -half_height/100, half_height/100, 0.1f, 100.0f);

}

Renderer::~Renderer() {
    glDisableVertexAttribArray(DEFAULT_POSITION_ATTRIBUTE);
    glDisableVertexAttribArray(DEFAULT_COLOR_ATTRIBUTE);
    glDisableVertexAttribArray(DEFAULT_TEXTURE_ATTRIBUTE);
    glDisableVertexAttribArray(DEFAULT_CTT_ATTRIBUTE);
}
