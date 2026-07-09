#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

// 实例化
layout (location = 12) in mat4 instanceMatrix;

out vec2 uv;

// pre-frame uniform block 传递
layout(std140) uniform Matrices {
    mat4 worldToView;
    mat4 viewToClip;
};

// pre-object uniform 传递
// uniform mat4 objectToWorld;
// uniform mat4 worldToView;
// uniform mat4 viewToClip;

void main() {
    gl_Position = viewToClip * worldToView * instanceMatrix * vec4(position, 1.0);
    uv = texcoord;
}