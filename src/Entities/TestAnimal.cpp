/*
 * TestAnimal.cpp
 *
 *  Created on: 23 Apr 2021
 *      Author: mchlp
 */

/*
 * TestChar.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#include "TestAnimal.h"

#include "../SpriteRenderer.h"
#include "../DebugRenderer.h"
#include "testChar.h"

Animal::Animal(SpriteRenderer* pRenderer, DebugRenderer* pDebug, Player* pPlayer) :
	m_sprite(this, pRenderer, "animal", glm::vec4(0.0f, 0.0f, 0.25f, 1.0f), 0.2f, 0.2f),
	m_pPlayer(pPlayer),
	m_pRenderer(pRenderer),
	m_spawnPos(),
	m_facingRight(true)
{}

void Animal::update(float deltaTime)
{
	m_body.update();
	m_sprite.update(deltaTime);


	// temp
	b2RayCastInput i;
	i.p1.Set((m_transform.x / 80), (m_transform.y / 80));
	if (m_facingRight)
		i.p2.Set((m_transform.x / 80) + 10, (m_transform.y / 80));
	else
		i.p2.Set((m_transform.x / 80) - 10, (m_transform.y / 80));
	i.maxFraction = 1.0f;
	b2RayCastOutput o;
	int32 childIndex = 0;

	b2Fixture* pFix = m_pPlayer->getComponent<Fixture>()->getFixture();
	bool hit = pFix->RayCast(&o, i, childIndex);
	if (hit) m_charging = true;

	if (m_charging) {
		m_sprite.playAnimation("run");
		charge();
	}

	if(m_facingRight)
	{
		m_sprite.flipX(true);
	}
	else
	{
		m_sprite.flipX(false);
	}
}

void Animal::render(float percent, glm::vec2 camera)
{
	m_sprite.render(percent, camera);
	m_colliderMain.render(percent, camera);
	m_leftCheck.render(percent, camera);
	m_rightCheck.render(percent, camera);
}

void Animal::proc()
{
	if (m_facingRight)
	{
		m_body.getBody()->ApplyForceToCenter(b2Vec2(-200.0f, 0.0f), true);
		m_facingRight = false;
	}
	else
	{
		m_body.getBody()->ApplyForceToCenter(b2Vec2(200.0f, 0.0f), true);
		m_facingRight = true;
	}
}

void Animal::charge()
{
	float y = m_body.getBody()->GetLinearVelocity().y;
	if (m_facingRight && m_contact == 1)
	{
		m_facingRight = false;
		m_charging = false;
		m_sprite.playAnimation("idle");
	}
	if (!m_facingRight && m_contact == 2)
	{
		m_facingRight = true;
		m_charging = false;
		m_sprite.playAnimation("idle");
	}

	if (m_facingRight)
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(3.3f, y));
	}
	else
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(-3.3f, y));
	}
}

void Animal::init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug)
{
	addComponent(&m_sprite);
	addComponent(&m_body);

	m_sprite.addAnimation(glm::vec4(0.0f, 0.0f, 0.25f, 1.0f), 4, "run");
	m_sprite.addAnimation(glm::vec4(0.5f, 0.0f, 0.75f, 1.0f), 1, "idle");
	m_sprite.playAnimation("idle");

	m_spawnPos = glm::vec2(pos.x * 80, pos.y * 80);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	m_body = Body(this, pWorld, bodyDef, pos);

	m_colliderMain = BoxCollider(this, m_body.getBody(), pDebug,
			m_sprite.getDimensions().x / 80.0f /  3.0f, m_sprite.getDimensions().y / 80.0f / 4.0f,
			b2Vec2(), 1.33f, 0.3f);

	m_rightCheck = Sensor(this, m_body.getBody(), pDebug,
			m_sprite.getDimensions().x / 80.0f / 10.0f, m_sprite.getDimensions().y / 80.0f / 8.0f,
			b2Vec2(m_sprite.getDimensions().x / 80.0f / 3.0f, 0.0f), &m_rightCheck);
	m_rightCheck.initBegin(std::bind(&Animal::contactEdge, this, true));
	m_rightCheck.initEnd(std::bind(&Animal::endContactEdge, this));

	m_leftCheck = Sensor(this, m_body.getBody(), pDebug,
			m_sprite.getDimensions().x / 80.0f / 10.0f, m_sprite.getDimensions().y / 80.0f / 8.0f,
			b2Vec2(-m_sprite.getDimensions().x / 80.0f / 3.0f, 0.0f), &m_leftCheck);
	m_leftCheck.initBegin(std::bind(&Animal::contactEdge, this, false));
	m_leftCheck.initEnd(std::bind(&Animal::endContactEdge, this));
}

void Animal::contactEdge(bool right)
{
	if (right)
	{
		m_contact = 1;
	}
	else
	{
		m_contact = 2;
	}
}

void Animal::endContactEdge()
{
	m_contact = 0;
}
