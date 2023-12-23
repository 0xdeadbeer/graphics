#version 330 core 

layout (location = 0) in vec3 pos; 
layout (location = 1) in vec2 texture_coord; 

uniform mat4 view;
uniform mat4 projection; 

out vec2 tex_coord;

void main() {
    gl_Position = projection * view * vec4(pos.xyz, 1.0);
    tex_coord = texture_coord;
}
