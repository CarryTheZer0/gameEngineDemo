/*
 * BoxCollider.cpp
 *
 *  Created on: 12 May 2021
 *      Author: mchlp
 */

#include "BoxCollider.h"

#include "../DebugRenderer.h"

BoxCollider::BoxCollider(Entity* pOwner, b2Body* pBody, DebugRenderer* pDebug, float xExtent, float yExtent,
		b2Vec2 offset, float density, float friction) :
	Fixture(),
	m_pDebug(pDebug),
	m_xExtent(xExtent),
	m_yExtent(yExtent),
	m_offset(offset)
{
	b2PolygonShape colliderBox;
	colliderBox.SetAsBox(m_xExtent, m_yExtent);
	b2FixtureDef colliderDef;
	colliderDef.shape = &colliderBox;
	colliderDef.density = density;
	colliderDef.friction = friction;

	init(pBody, colliderDef);
}

void BoxCollider::render(float percent, glm::vec2 camera)
{
	b2Vec2 pos = m_pFixture->GetBody()->GetPosition() + m_offset;
	float x = pos.x * 80;
	float y = pos.y * 80;
	float xExtent = m_xExtent * 80;
	float yExtent = m_yExtent * 80;

	m_pDebug->drawLine(glm::vec2(x + xExtent, y + yExtent), glm::vec2(x - xExtent, y + yExtent), camera);
	m_pDebug->drawLine(glm::vec2(x - xExtent, y + yExtent), glm::vec2(x - xExtent, y - yExtent), camera);
	m_pDebug->drawLine(glm::vec2(x - xExtent, y - yExtent), glm::vec2(x + xExtent, y - yExtent), camera);
	m_pDebug->drawLine(glm::vec2(x + xExtent, y - yExtent), glm::vec2(x + xExtent, y + yExtent), camera);
}
