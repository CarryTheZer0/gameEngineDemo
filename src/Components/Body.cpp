/*
 * Body.cpp
 *
 *  Created on: 7 Apr 2021
 *      Author: mchlp
 */

#include "../Entity.h"
#include "Body.h"

Body::Body(Entity* pOwner, b2World* pWorld, b2BodyDef def, glm::vec2 pos) :
	Component(pOwner)
{
	init(pWorld, def, pos);
}

void Body::update()
{
	b2Vec2 pos = m_pBody->GetPosition();
	m_pOwner->setPos({pos.x * 80, pos.y * 80});
}

void Body::init(b2World* pWorld, b2BodyDef def, glm::vec2 pos)
{
	def.position.Set(pos.x, pos.y);
	m_pBody = pWorld->CreateBody(&def);
}

