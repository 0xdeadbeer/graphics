#version 330 core 

layout (location = 0) in vec3 pos; 
layout (location = 1) in vec2 texture_coord; 

uniform vec3 size;
uniform ivec3 offset;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection; 

out vec2 tex_coord;

void main() {
    vec3 new_pos = pos.xyz + offset.xyz; 
    new_pos = new_pos.xyz * size.xyz;

    gl_Position = projection * view * model * vec4(new_pos.xyz, 1.0);
    tex_coord = texture_coord;
}
