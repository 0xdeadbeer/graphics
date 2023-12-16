#version 330 core 

layout (location = 0) in vec4 position; 
layout (location = 1) in vec4 origin;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 texture_position; 
layout (location = 4) in float ctt;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec4 _color;
out vec2 _texture_position;
out float _ctt;

void main() {
    vec3 origined_position = origin.xyz + position.xyz;
    origined_position *= 0.2;
    //gl_Position = projection_matrix * view_matrix * position;
    gl_Position = projection_matrix * view_matrix * vec4(origined_position, 1.0);
    _color = color;
    _texture_position = texture_position;
    _ctt = ctt;
}
