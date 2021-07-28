#version 430 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 iTexCoords;
layout (location = 2) in float iTexIndex;

out vec2 texCoords;
out float texIndex;

uniform mat4 view;
uniform mat4 projection;
uniform int iFilterPolySize;

void main()
{
    texCoords = iTexCoords;
    texIndex = iTexIndex;
    gl_Position = projection * view * vec4(position, 0.0, 1.0);
}