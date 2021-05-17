/*
 * Sensor.cpp
 *
 *  Created on: 15 Apr 2021
 *      Author: mchlp
 */

#include "Sensor.h"
#include "../Entity.h"
#include "../DebugRenderer.h"

void dummy() {}

Sensor::Sensor(Entity* pOwner, b2Body* pBody, DebugRenderer* pDebug, float xExtent, float yExtent,
		b2Vec2 offset, Sensor* pointer, glm::vec3 debugColor) :
	Fixture(pOwner),
	m_pDebug(pDebug),
	m_xExtent(xExtent),
	m_yExtent(yExtent),
	m_offset(offset),
	m_collidingCount(0),
	m_vertexCount(4),
	m_debugColor(debugColor),
	m_debugColorCurrent(debugColor)
{
	m_f_invokeBegin = dummy;
	m_f_invokeEnd = dummy;

	b2PolygonShape box;
	box.SetAsBox(xExtent, yExtent, offset, 0.0f);
	b2FixtureDef def;
	def.shape = &box;
	def.isSensor = true;
	def.userData.pointer = reinterpret_cast<uintptr_t>(pointer);

	init(pBody, def);
}

void Sensor::render(float percent, glm::vec2 camera)
{
	b2PolygonShape* poly = dynamic_cast<b2PolygonShape*>(m_pFixture->GetShape());

	b2Vec2 pos = m_pFixture->GetBody()->GetPosition();
	float x = pos.x * 80;
	float y = pos.y * 80;

	for (int i = 0; i < m_vertexCount - 1; i++)
	{
		m_pDebug->drawLine(glm::vec2(
			x + (poly->m_vertices[i].x * 80), y + (poly->m_vertices[i].y * 80)),
				glm::vec2(
			x + (poly->m_vertices[i + 1].x * 80), y + (poly->m_vertices[i + 1].y * 80)),
			camera, m_debugColorCurrent);
	}

	m_pDebug->drawLine(glm::vec2(
		x + (poly->m_vertices[0].x * 80), y + (poly->m_vertices[0].y * 80)),
			glm::vec2(
		x + (poly->m_vertices[m_vertexCount - 1].x * 80), y + (poly->m_vertices[m_vertexCount - 1].y * 80)),
		camera, m_debugColorCurrent);
}

void Sensor::flipX()
{
	b2PolygonShape* poly = dynamic_cast<b2PolygonShape*>(m_pFixture->GetShape());
	for (b2Vec2& vertex : poly->m_vertices)
	{
		vertex.x += -m_offset.x * 2;
	}
	m_offset.x *= -1;
}

void Sensor::initBegin(std::function<void()> func)
{
	m_f_invokeBegin = func;
}

void Sensor::initEnd(std::function<void()> func)
{
	m_f_invokeEnd = func;
}

void Sensor::invokeBegin()
{
	m_collidingCount++;
	m_debugColorCurrent = glm::vec3(0.0f, 1.0f, 0.0f);
	m_f_invokeBegin();
}

void Sensor::invokeEnd()
{
	m_collidingCount--;
	if (!m_collidingCount)
		m_debugColorCurrent = m_debugColor;
	m_f_invokeEnd();
}
