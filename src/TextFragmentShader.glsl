#version 430 core
in vec2 texCoords;
in float texIndex;
out vec4 color;

uniform sampler2D text[32];
uniform vec3 textColor;

void main()
{    
	int index = int(texIndex);
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text[index], texCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}  