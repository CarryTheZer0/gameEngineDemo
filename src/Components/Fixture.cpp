/*
 * Shape.cpp
 *
 *  Created on: 7 Apr 2021
 *      Author: mchlp
 */

#include "../Entity.h"
#include "Fixture.h"

Fixture::Fixture(Entity* pOwner, b2Body* pBody, b2FixtureDef def) :
	Component(pOwner)
{
	init(pBody, def);
}

void Fixture::update()
{}

void Fixture::init(b2Body* pBody, b2FixtureDef def)
{
	m_pFixture = pBody->CreateFixture(&def);
}

b2Fixture* Fixture::getFixture()
{
	return m_pFixture;
}
