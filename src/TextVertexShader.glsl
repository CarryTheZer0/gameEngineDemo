#version 430 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 iTexCoords;
layout (location = 2) in float iTexIndex;
out vec2 texCoords;
out float texIndex;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex, 0.0, 1.0);
    texCoords = iTexCoords;
    texIndex = iTexIndex;
}  