#version 330 core
in vec4 _color; 
in vec2 _texture_position; 
in float _ctt;

out vec4 color; 

void main() {
//    color = texture(sampler, _texture_position);
    //color = vec4(1.0, 1.0, 1.0, 1.0);
    color = _color;
}
