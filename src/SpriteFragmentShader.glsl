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
	vec4 shadowColor = vec4(spriteColor, 1.0) * texture(images[index], texCoords);
	vec2 fragMagnitude = vec2(gl_FragCoord.x, gl_FragCoord.y) - castOrigin;
	float dotProd = dot(viewDirection, normalize(fragMagnitude));
	
	if (dotProd == 0.0 || (acos(dotProd) < 0.3 && pow(fragMagnitude.x, 2.0) + pow(fragMagnitude.y, 2.0) > 1600.0))
	{
		// keep in colour
		shadowColor == vec4(spriteColor, 1.0) * texture(images[index], texCoords);
    }
    else
    {
    	// make greyscale
    	float avg = (shadowColor.x + shadowColor.y + shadowColor.z) / 3;
        shadowColor = vec4(avg, avg, avg, shadowColor.w) * vec4(0.8, 0.8, 0.8, 1.0);
   	}
   	color = shadowColor;
}