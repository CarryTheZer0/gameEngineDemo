/*
 * Charger.cpp
 *
 *  Created on: 23 Apr 2021
 *      Author: mchlp
 */

#include "Charger.h"

#include "../Rendering/SpriteRenderer.h"
#include "../Rendering/DebugRenderer.h"
#include "testChar.h"
#include "../GameplayScene.h"

Charger::Charger(GameplayScene* pParentScene, SpriteRenderer* pRenderer, DebugRenderer* pDebug) :
	Entity(pParentScene, pRenderer, pDebug),
	m_sprite(this, pRenderer, "animal", glm::vec4(0.0f, 0.0f, 0.25f, 1.0f), 0.2f, 0.3f),
	m_photographable(this, "animalPhoto"),
	m_facingRight(true),
	m_charging(false),
	m_contact(false)
{}

void Charger::update(float deltaTime)
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

	std::vector<Fixture*> pColliders = m_pParentScene->getPlayer()->getComponents<Fixture>();
	for(Fixture* pColl : pColliders)
		{
			b2Fixture* pFix = pColl->getFixture();
			if (!pFix->IsSensor())
			{
				bool hit = pFix->RayCast(&o, i, childIndex);
				if (hit) m_charging = true;
			}
		}

	if (m_charging) {
		m_sprite.playAnimation("run");
		charge();
	}
}

void Charger::render(float percent, glm::vec2 camera, float scale)
{
	m_sprite.render(percent, camera, scale);
	m_colliderMain.render(percent, camera, scale);
	m_colliderCircle.render(percent, camera, scale);
	m_frontCheck.render(percent, camera, scale);
	m_hurtBox.render(percent, camera, scale);
}

void Charger::proc()
{
	if (m_facingRight)
	{
		setFacingRight(false);
	}
	else
	{
		setFacingRight(true);
	}
}

void Charger::charge()
{
	float y = m_body.getBody()->GetLinearVelocity().y;
	if (m_facingRight && m_contact)
	{
		setFacingRight(false);
		m_charging = false;
		m_sprite.playAnimation("idle");
	}
	else if (!m_facingRight && m_contact)
	{
		setFacingRight(true);
		m_charging = false;
		m_sprite.playAnimation("idle");
	}

	if (m_facingRight)
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(4.0f, y));
	}
	else
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(-4.0f, y));
	}
}

void Charger::setFacingRight(bool facingRight)
{
	m_sprite.flipX(facingRight);
	if (facingRight != m_facingRight)
	{
		m_colliderCircle.flipX();
		m_colliderMain.flipX();
		m_hurtBox.flipX();
		m_frontCheck.flipX();
	}
	m_facingRight = facingRight;
}

void Charger::init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug, bool facingRight)
{
	addComponent(&m_sprite);
	addComponent(&m_body);
	addComponent(&m_sprite);
	addComponent(&m_frontCheck);
	addComponent(&m_hurtBox);
	addComponent(&m_body);
	addComponent(&m_colliderMain);
	addComponent(&m_colliderCircle);
	addComponent(&m_photographable);

	m_sprite.addAnimation(glm::vec4(0.0f, 0.0f, 0.25f, 1.0f), 4, "run");
	m_sprite.addAnimation(glm::vec4(0.5f, 0.0f, 0.75f, 1.0f), 1, "idle");
	m_sprite.playAnimation("idle");

	m_transform = glm::vec2(pos.x * 80, pos.y * 80);
	m_depth = 1;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	m_body = Body(this, pWorld, bodyDef, pos);

	b2Filter animalFilter;
	animalFilter.groupIndex = -2; // animal
	animalFilter.categoryBits = 0x0003; // animal
	animalFilter.maskBits = 0x0001; // terrain

	float xExtent = m_sprite.getDimensions().x / 80.0f / 3.0f;
	float yExtent = m_sprite.getDimensions().y / 80.0f / 3.9f;

	m_colliderMain = BoxCollider(this, m_body.getBody(), pDebug,
			xExtent - (yExtent / 2), yExtent * 0.95, b2Vec2(-yExtent / 2, 0.0f), 1.33f, 0.3f,
			&m_colliderMain);
	m_colliderMain.getFixture()->SetFilterData(animalFilter);

	m_colliderCircle = CircleCollider(this, m_body.getBody(), pDebug, yExtent, b2Vec2(xExtent - yExtent, 0.0f),
			1.33f / 2.0f, 0.3f, &m_colliderCircle);
	m_colliderCircle.getFixture()->SetFilterData(animalFilter);

	b2Filter sensorFilter;
	sensorFilter.groupIndex = -2; // animal
	sensorFilter.categoryBits = 0x0004; // sensor
	sensorFilter.maskBits = 0x0001; // terrain

	m_frontCheck = Sensor(this, m_body.getBody(), pDebug,
			0.1f, yExtent * 0.9, b2Vec2(xExtent, 0.0f), &m_frontCheck);
	m_frontCheck.initBegin(std::bind(&Charger::contactEdge, this, std::placeholders::_1));
	m_frontCheck.initEnd(std::bind(&Charger::endContactEdge, this, std::placeholders::_1));
	m_frontCheck.getFixture()->SetFilterData(sensorFilter);

	b2Filter hurtFilter;
	hurtFilter.groupIndex = -2; // animal
	hurtFilter.categoryBits = 0x0004; // sensor
	hurtFilter.maskBits = 0x0002; // player

	m_hurtBox = Sensor(this, m_body.getBody(), pDebug,
			yExtent * 1.6, yExtent, b2Vec2(xExtent - (yExtent * 1.6), 0.0f), &m_hurtBox, glm::vec3(1.0f, 0.0f, 0.0f));
	m_hurtBox.initBegin(std::bind(&Player::reset, m_pParentScene->getPlayer()));
	m_hurtBox.getFixture()->SetFilterData(hurtFilter);

	setFacingRight(facingRight);
}

void Charger::contactEdge(Fixture* contact)
{
	m_contact = true;
}

void Charger::endContactEdge(Fixture* contact)
{
	m_contact = false;
}
