/*
 * spriteRenderer.cpp
 *
 *  Created on: 26 Mar 2021
 *      Author: mchlp
 */

#include "SpriteRenderer.h"

struct Vertex
{
   	glm::vec2 position;
   	glm::vec2 texCoords;
   	float texID;
};

SpriteRenderer::SpriteRenderer(Shader &shader) :
	m_batchOffset(0),
	m_texCount(0),
	m_shader(shader)
{
    initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

void SpriteRenderer::drawSprite(Texture2D &texture, glm::vec4 srcCoords,
		glm::vec2 position, glm::vec2 camera, glm::vec2 size, float rotate, glm::vec2 rotateOffset, glm::vec3 color)
{
	// draw current batch if texture units are full
	int textureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
	int maxTexUnits = 32; // temp line
	if (m_texCount >= textureUnits || m_texCount >= maxTexUnits)
	{
		draw();
		m_texCount = 0;
		m_texIDs.clear();
	}

	// identify required texture unit
    int textureSlot;
    if(!(m_texIDs.count(texture.getID())))
    {
    	// Add texture to map if not present
    	m_texIDs.insert(std::pair<unsigned int, int>(texture.getID(), m_texCount));
    	textureSlot = m_texCount;
    	m_texCount++;
    }
    else
    {
    	// Get texture unit if already present
        textureSlot = m_texIDs[texture.getID()];
    }

    // bind texture
    m_shader.use();
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    texture.bind();

    // prepare transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::translate(model, glm::vec3(rotateOffset.x * size.x, rotateOffset.y * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-rotateOffset.x * size.x, -rotateOffset.y * size.y, 0.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3( size, 1.0f )); // last scale

    glm::vec4 pos1 = model * glm::vec4(0.0, 1.0, 0.0, 1.0);
    glm::vec4 pos2 = model * glm::vec4(0.0, 0.0, 0.0, 1.0);
    glm::vec4 pos3 = model * glm::vec4(1.0, 0.0, 0.0, 1.0);
    glm::vec4 pos4 = model * glm::vec4(1.0, 1.0, 0.0, 1.0);
	
    // generate vertex data
    float vertices[] = {
        // pos          // texPos                 // texID
        pos1.x, pos1.y, srcCoords.x, srcCoords.w, textureSlot,
		pos2.x, pos2.y, srcCoords.x, srcCoords.y, textureSlot,
		pos3.x, pos3.y, srcCoords.z, srcCoords.y, textureSlot,
		pos4.x, pos4.y, srcCoords.z, srcCoords.w, textureSlot
    };

    // generate index data
    uint32_t indices[] = {
    		(m_batchOffset * 4) + 0, (m_batchOffset * 4) + 1, (m_batchOffset * 4) + 2,
			(m_batchOffset * 4) + 2, (m_batchOffset * 4) + 3, (m_batchOffset * 4) + 0
    };

    // write to vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferSubData(GL_ARRAY_BUFFER, m_batchOffset * sizeof(vertices), sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    // write to index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_batchOffset * sizeof(indices), sizeof(indices), indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	m_batchOffset++;

	// translate camera
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(camera.x, camera.y, 0.0f));
	// set uniforms
	m_shader.setMatrix4("view", view);
    m_shader.setVector3f("spriteColor", color);

	int samplers[m_texCount];
	for (int i = 0; i < m_texCount; i++)
    	samplers[i] = i;
    m_shader.setIntegerList("images", m_texCount, samplers);
}

void SpriteRenderer::setShadows(glm::vec2 pos, glm::vec2 dir)
{
	//pos = glm::vec2(512.0f, 440.0f); // TODO set pos correctly
    m_shader.setVector2f("viewDirection", dir);
    m_shader.setVector2f("castOrigin", pos);
}

void SpriteRenderer::draw()
{
	// draw 2 triangles per sprite
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);
	glDrawElements(GL_TRIANGLES, m_batchOffset * 6, GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_batchOffset = 0;

//	glColor3f(1.0f, 0.0f, 0.0f);
//	glLineWidth(2.5);
//	glBegin(GL_LINES);
//	glVertex3f(0.0, -10, 0);
//	glVertex3f(0.0, 10, 0);
//	glEnd();
//	GLenum err1 = glGetError();

//	GLfloat lineVertices[] =
//	{
//			-100.0f, -100.0f,
//			100.0f, 100.0f
//	};
//
//	glColor3f(1.0f, 0.0f, 0.0f);
//	glLineWidth(10);
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glVertexPointer(2, GL_FLOAT, 0, lineVertices);
//	glDrawArrays(GL_LINES, 0, 2);
//	glDisableClientState(GL_VERTEX_ARRAY);
}

void SpriteRenderer::initRenderData()
{
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glGenBuffers(1, &m_quadIBO);

    float vertices[] = {
            // pos      // texPos   // texID
            0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    		1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    		1.0f, 1.0f, 1.0f, 1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 1000, nullptr, GL_DYNAMIC_DRAW);

    uint32_t indices[] = {
    		0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * 1000, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_quadVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)8);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)16);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
