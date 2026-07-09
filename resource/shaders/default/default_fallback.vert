#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

layout(std140) uniform Matrices {
    mat4 worldToView;
    mat4 viewToClip;
};

uniform mat4 objectToWorld;

out vec2 uv;

void main() {
    gl_Position = viewToClip * worldToView * objectToWorld * vec4(position, 1.0);
    uv = texcoord;
}