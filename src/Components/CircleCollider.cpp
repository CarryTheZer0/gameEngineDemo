/*
 * CircleCollider.cpp
 *
 *  Created on: 13 May 2021
 *      Author: mchlp
 */

#include "CircleCollider.h"

#include "../DebugRenderer.h"

CircleCollider::CircleCollider(Entity* pOwner, b2Body* pBody, DebugRenderer* pDebug, float radius,
		b2Vec2 offset, float density, float friction, Fixture* pointer) :
	Fixture(pOwner),
	m_pDebug(pDebug),
	m_radius(radius)
{
	b2CircleShape shape;
	shape.m_radius = radius;
	shape.m_p = offset;
	b2FixtureDef colliderDef;
	colliderDef.shape = &shape;
	colliderDef.density = density;
	colliderDef.friction = friction;

	init(pBody, colliderDef, pointer);
}

void CircleCollider::render(float percent, glm::vec2 camera, float scale)
{
	b2CircleShape* circ = dynamic_cast<b2CircleShape*>(m_pFixture->GetShape());
	b2Vec2 pos = b2Vec2(m_pFixture->GetBody()->GetPosition().x * scale,
						m_pFixture->GetBody()->GetPosition().y * scale)
				+ b2Vec2(circ->m_p.x * scale, circ->m_p.y * scale);
	float x = pos.x * 80;
	float y = pos.y * 80;
	float radius = m_radius * 80 * scale;

	m_pDebug->drawLine(glm::vec2(x, y + radius), glm::vec2(x - radius * 0.75, y + radius * 0.75), camera);
	m_pDebug->drawLine(glm::vec2(x - radius * 0.75, y + radius* 0.75), glm::vec2(x - radius, y), camera);

	m_pDebug->drawLine(glm::vec2(x - radius, y), glm::vec2(x - radius * 0.7, y - radius * 0.75), camera);
	m_pDebug->drawLine(glm::vec2(x - radius * 0.75, y - radius * 0.75), glm::vec2(x, y - radius), camera);

	m_pDebug->drawLine(glm::vec2(x, y - radius), glm::vec2(x + radius * 0.75, y - radius * 0.75), camera);
	m_pDebug->drawLine(glm::vec2(x + radius * 0.75, y - radius * 0.75), glm::vec2(x + radius, y), camera);

	m_pDebug->drawLine(glm::vec2(x + radius, y), glm::vec2(x + radius * 0.75, y + radius * 0.75), camera);
	m_pDebug->drawLine(glm::vec2(x + radius * 0.75, y + radius * 0.75), glm::vec2(x, y + radius), camera);
}

void CircleCollider::flipX()
{
	b2CircleShape* circ = dynamic_cast<b2CircleShape*>(m_pFixture->GetShape());
	circ->m_p *= -1;
}
