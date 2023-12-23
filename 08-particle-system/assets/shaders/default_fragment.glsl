#version 330 core
in vec4 _color; 
in vec2 _texture_position; 
in float _ctt;

out vec4 color; 

uniform sampler2D sampler;

void main() {
    color = texture(sampler, _texture_position) * (_color * _ctt);
}
