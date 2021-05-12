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
		b2Vec2 offset, Sensor* pointer) :
	Fixture(pOwner),
	m_pDebug(pDebug),
	m_xExtent(xExtent),
	m_yExtent(yExtent),
	m_offset(offset),
	m_debugColor(1.0f, 1.0f, 0.0f)
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
	b2Vec2 pos = m_pFixture->GetBody()->GetPosition() + m_offset;
	float x = pos.x * 80;
	float y = pos.y * 80;
	float xExtent = m_xExtent * 80;
	float yExtent = m_yExtent * 80;

	m_pDebug->drawLine(glm::vec2(x + xExtent, y + yExtent), glm::vec2(x - xExtent, y + yExtent), camera, m_debugColor);
	m_pDebug->drawLine(glm::vec2(x - xExtent, y + yExtent), glm::vec2(x - xExtent, y - yExtent), camera, m_debugColor);
	m_pDebug->drawLine(glm::vec2(x - xExtent, y - yExtent), glm::vec2(x + xExtent, y - yExtent), camera, m_debugColor);
	m_pDebug->drawLine(glm::vec2(x + xExtent, y - yExtent), glm::vec2(x + xExtent, y + yExtent), camera, m_debugColor);
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
	m_f_invokeBegin();
	m_debugColor = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Sensor::invokeEnd()
{
	m_f_invokeEnd();
	m_debugColor = glm::vec3(1.0f, 1.0f, 0.0f);
}
