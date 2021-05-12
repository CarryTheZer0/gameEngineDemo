/*
 * TestFloor.cpp
 *
 *  Created on: 7 Apr 2021
 *      Author: mchlp
 */

#include "TestFloor.h"

#include "../SpriteRenderer.h"
#include "../DebugRenderer.h"
#include "../InputHandler.h"

Floor::Floor(SpriteRenderer* pRenderer,  DebugRenderer* pDebug) :
	m_sprite(this, pRenderer, "dplan", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(), 0.8f)
{}

void Floor::update(float deltaTime)
{
	m_body.update();
}

void Floor::render(float percent, glm::vec2 camera)
{
	m_sprite.render(percent, camera);
	m_colliderMain.render(percent, camera);
}

void Floor::init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug)
{
	addComponent(&m_sprite);

	b2BodyDef groundBodyDef;
	m_body = Body(this, pWorld, groundBodyDef, pos);

	m_colliderMain = BoxCollider(this, m_body.getBody(), pDebug,
			m_sprite.getDimensions().x / 80.0f /  2.3f, m_sprite.getDimensions().y / 80.0f /  5.0f,
			b2Vec2(), 0.0f, 0.3f);
}
