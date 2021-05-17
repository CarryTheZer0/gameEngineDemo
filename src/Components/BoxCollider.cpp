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
	m_offset(offset),
	m_vertexCount(4)
{
	b2PolygonShape colliderBox;
	colliderBox.SetAsBox(m_xExtent, m_yExtent, offset, 0.0f);
	b2FixtureDef colliderDef;
	colliderDef.shape = &colliderBox;
	colliderDef.density = density;
	colliderDef.friction = friction;

	init(pBody, colliderDef);
}

void BoxCollider::render(float percent, glm::vec2 camera)
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
			x + (poly->m_vertices[i + 1].x * 80), y + (poly->m_vertices[i + 1].y * 80)), camera);
	}

	m_pDebug->drawLine(glm::vec2(
		x + (poly->m_vertices[0].x * 80), y + (poly->m_vertices[0].y * 80)),
			glm::vec2(
		x + (poly->m_vertices[m_vertexCount - 1].x * 80), y + (poly->m_vertices[m_vertexCount - 1].y * 80)), camera);
}

void BoxCollider::flipX()
{
	b2PolygonShape* poly = dynamic_cast<b2PolygonShape*>(m_pFixture->GetShape());
	for (b2Vec2& vertex : poly->m_vertices)
	{
		vertex.x += -m_offset.x * 2;
	}
	m_offset.x *= -1;
}
