#version 330 core
in vec2 texCoords;
in float texIndex;
out vec4 color;

uniform sampler2D images[32]; // 32 = maxTexUnits in SpriteRenderer.cpp
uniform vec3 spriteColor;
uniform vec2 viewDirection;
uniform vec2 castOrigin;

void main()
{    
	int index = int(texIndex);
	color = vec4(spriteColor, 1.0) * texture(images[index], texCoords);
}