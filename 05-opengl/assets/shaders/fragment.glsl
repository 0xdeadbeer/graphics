#version 330 core
out vec4 color; 
in vec2 tex_coord;

uniform sampler2D texture1;

void main() {
    color = texture(texture1, tex_coord);
//    color = vec4(1.0, 0.0, 0.0, 1.0);

    if (color.a < 0.1) 
        discard;
}
