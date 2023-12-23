#version 330 core
out vec4 color; 
in vec2 tex_coord;

uniform sampler2D sampler;

void main() {
    color = texture(sampler, tex_coord);
//    color = vec4(1.0, 0.0, 0.0, 1.0);

    if (color.a < 0.1) 
        discard;
}
