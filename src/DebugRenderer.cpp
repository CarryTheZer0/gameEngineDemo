/*
 * DebugRenderer.cpp
 *
 *  Created on: 10 May 2021
 *      Author: mchlp
 */

#include "DebugRenderer.h"

struct line
{
	float x1; float y1; float r1; float g1; float b1;
	float x2; float y2; float r2; float g2; float b2;
};

DebugRenderer::DebugRenderer(Shader &shader) :
	m_batchOffset(0),
	m_shader(shader)
{
    initRenderData();
}

DebugRenderer::~DebugRenderer()
{
    glDeleteVertexArrays(1, &m_lineVAO);
}

void DebugRenderer::drawBox(glm::vec4 srcCoords, glm::vec2 position, glm::vec2 camera,
		glm::vec2 size, float rotate, glm::vec2 rotateOffset, glm::vec3 color)
{
    m_shader.use();

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
    	pos1.x, pos1.y, 1.0f, 1.0f, 1.0f, // first vertex
    	pos2.x, pos2.y, 1.0f, 1.0f, 1.0f, // second vertex

		pos2.x, pos2.y, 1.0f, 1.0f, 1.0f, // first vertex
		pos3.x, pos3.y, 1.0f, 1.0f, 1.0f, // second vertex

		pos3.x, pos3.y, 1.0f, 1.0f, 1.0f, // first vertex
		pos4.x, pos4.y, 1.0f, 1.0f, 1.0f, // second vertex

		pos4.x, pos4.y, 1.0f, 1.0f, 1.0f, // first vertex
		pos1.x, pos1.y, 1.0f, 1.0f, 1.0f // second vertex
    };

    // write to vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferSubData(GL_ARRAY_BUFFER, m_batchOffset * sizeof(line), sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_batchOffset += 4;

	// translate camera
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(camera.x, camera.y, 0.0f));
	// set uniforms
	m_shader.setMatrix4("view", view);
}

void DebugRenderer::drawLine(glm::vec2 a, glm::vec2 b, glm::vec2 camera, glm::vec3 color)
{
    m_shader.use();

    // generate vertex data
    float vertices[] = {
    	a.x, a.y, color.x, color.y, color.z, // first vertex
    	b.x, b.y, color.x, color.y, color.z  // second vertex
    };

    // write to vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferSubData(GL_ARRAY_BUFFER, m_batchOffset * sizeof(line), sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_batchOffset++;

	// translate camera
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(camera.x, camera.y, 0.0f));
	// set uniforms
	m_shader.setMatrix4("view", view);
}

void DebugRenderer::draw()
{
	// draw lines
	m_shader.use();
	glBindVertexArray(m_lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glDrawArrays(GL_LINES, 0, m_batchOffset * 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_batchOffset = 0;
}

void DebugRenderer::initRenderData()
{
	glGenVertexArrays(1, &m_lineVAO);
	glGenBuffers(1, &m_lineVBO);

	GLfloat lineSeg[] =
	{
	    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // first vertex
	    2.0f, 2.0f, 1.0f, 1.0f, 1.0f // second vertex
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineSeg) * 1000, nullptr, GL_DYNAMIC_DRAW);

	glBindVertexArray(m_lineVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)8);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

