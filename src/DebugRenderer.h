/*
 * DebugRenderer.h
 *
 *  Created on: 10 May 2021
 *      Author: mchlp
 */

#ifndef DEBUGRENDERER_H_
#define DEBUGRENDERER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class DebugRenderer
{
public:
    // Constructor (inits shaders/shapes)
	DebugRenderer(Shader &shader);
    // Destructor
    ~DebugRenderer();
    // Renders a defined quad textured with given sprite
    void drawBox(glm::vec4 srcCoords,
    		glm::vec2 position, glm::vec2 camera = glm::vec2(), glm::vec2 size = glm::vec2(10.0f, 10.0f),
			float rotate = 0.0f, glm::vec2 rotateOffset = glm::vec2(), glm::vec3 color = glm::vec3(1.0f));
    void drawLine(glm::vec2 a, glm::vec2 b, glm::vec2 camera);
    void draw();
private:
    // Render state
    Shader       m_shader;
    unsigned int m_lineVAO;
    unsigned int m_lineVBO;
    int m_batchOffset;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif /* DEBUGRENDERER_H_ */
