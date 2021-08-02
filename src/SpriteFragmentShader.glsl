#version 430 core

layout(std430, binding = 3) buffer FilterSSBO {
    vec2 filterVertices[];
};

in vec2 texCoords;
in float texIndex;
out vec4 color;

uniform sampler2D images[32]; // 32 = maxTexUnits in SpriteRenderer.cpp
uniform vec2 castOrigin;
uniform int filterPolySize;

// return true if a point is on one side of the line AB, false otherwise
bool isWithin(in vec2 point, in vec2 a, in vec2 b)
{
	float sign = (b.x - a.x) * (point.y - a.y) - (b.y - a.y) * (point.x - a.x);

	if (sign < 0)
	{
		return(false);
	}
	else
	{
		return(true);
	}
	return(false);
}

bool checkPixel()
{
	// dont filter if castOrigin is not set
	if (castOrigin == vec2(0.0, 0.0))
	{
		return true;
	}
	
	if (isWithin(vec2(gl_FragCoord.x, gl_FragCoord.y), filterVertices[0], castOrigin)
		|| isWithin(vec2(gl_FragCoord.x, gl_FragCoord.y), castOrigin, filterVertices[filterPolySize]))
	{
		return false;
	}
	
	// if pixel is within any given triangle defined in filterVertices return true
	for (int i=0; i < filterPolySize; i++)
	{
		// triangle from player to two poly vertices
		if (isWithin(vec2(gl_FragCoord.x, gl_FragCoord.y), castOrigin, filterVertices[i]) &&
			isWithin(vec2(gl_FragCoord.x, gl_FragCoord.y), filterVertices[i], filterVertices[i + 1]) &&
			isWithin(vec2(gl_FragCoord.x, gl_FragCoord.y), filterVertices[i + 1], castOrigin))
		{
			return true;
		}
	}
	return false;
}

void main()
{    
	int index = int(texIndex);
	vec4 shadowColor = texture(images[index], texCoords);
	
	if (!checkPixel()) 
	{
		// make greyscale
		float avg = (shadowColor.x + shadowColor.y + shadowColor.z) / 3;
		shadowColor = vec4(avg, avg, avg, shadowColor.w);
	}
	
   	color = shadowColor;
}
