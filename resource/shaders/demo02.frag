#version 330 core

out vec4 target_Color;

in vec2 uv;

uniform sampler2D texture1;

void main() 
{
    target_Color = texture(texture1, uv);
}