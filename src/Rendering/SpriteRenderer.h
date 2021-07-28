/*
 * spriteRenderer.h
 *
 *  Created on: 26 Mar 2021
 *      Author: mchlp
 */

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>

#include "Shader.h"
#include "Texture.h"

class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader &shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void drawSprite(Texture2D &texture, glm::vec4 srcCoords,
    		glm::vec2 position, glm::vec2 camera = glm::vec2(), glm::vec2 size = glm::vec2(10.0f, 10.0f),
			float rotate = 0.0f, glm::vec2 rotateOffset = glm::vec2(), glm::vec3 color = glm::vec3(1.0f));
    void draw();
    void drawLineOfSightFilter(glm::vec2 a);
    void setShadowOrigin(glm::vec2 pos);
private:
    // Render state
    Shader       m_shader;
    unsigned int m_quadVAO;
    unsigned int m_quadVBO;
    unsigned int m_quadIBO;
    unsigned int m_quadSSBO;
    int m_texCount;
    int m_batchOffset;
    int m_batchOffsetFilter;
    std::map<unsigned int, int> m_texIDs;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif
