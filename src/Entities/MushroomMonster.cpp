/*
 * MushroomMonster.cpp
 *
 *  Created on: 7 Jun 2021
 *      Author: mchlp
 */


#include "MushroomMonster.h"

#include "../Rendering/SpriteRenderer.h"
#include "../Rendering/DebugRenderer.h"
#include "testChar.h"

MushroomMonster::MushroomMonster(GameplayScene* pParentScene, SpriteRenderer* pRenderer, DebugRenderer* pDebug) :
	Entity(pParentScene, pRenderer, pDebug),
	m_sprite(this, pRenderer, "mush", glm::vec4(0.0f, 0.0f, 0.125f, 1.0f), 0.2f, 0.45f),
	m_photographable(this, "mushPhoto"),
	m_facingRight(true),
	m_contact(false),
	m_jumpTarget(nullptr),
	m_timeElapsed(0.0f),
	m_state(IDLE)
{}

void MushroomMonster::update(float deltaTime)
{
	m_body.update();
	m_sprite.update(deltaTime);

	m_timeElapsed += deltaTime;

	switch (m_state)
	{
		case IDLE:
		{
			idle();
			break;
		}
		case WALK:
		{
			walk();
			break;
		}
		case GRAZE:
		{
			graze();
			break;
		}
	}
}

void MushroomMonster::render(float percent, glm::vec2 camera, float scale)
{
	m_sprite.render(percent, camera, scale);
	m_colliderMain.render(percent, camera, scale);
	m_colliderJump.render(percent, camera, scale);
	m_colliderCircle.render(percent, camera, scale);
	m_frontCheck.render(percent, camera, scale);
}

void MushroomMonster::flipX()
{
	bool facingRight = m_facingRight ? false : true;
	m_sprite.flipX(facingRight);
	m_colliderCircle.flipX();
	m_colliderMain.flipX();
	m_colliderJump.flipX();
	m_frontCheck.flipX();
	m_facingRight = facingRight;
}

void MushroomMonster::idle()
{
	m_sprite.playAnimation("idle");
	m_body.getBody()->SetLinearVelocity(b2Vec2(0.0f, m_body.getBody()->GetLinearVelocity().y));

	if (m_timeElapsed > 2.0f)
	{
		m_timeElapsed = 0.0f;
		int chance = rand() % 5;
		if (chance <= 1)
		{
			m_state = WALK;
			flipX();
		}
		else if (chance >= 3)
			m_state = GRAZE;
		else
			m_state = IDLE;
	}
}

void MushroomMonster::walk()
{
	float y = m_body.getBody()->GetLinearVelocity().y;
	m_sprite.playAnimation("run");
	if (m_facingRight)
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(1.5f, y));
	}
	else
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(-1.5f, y));
	}

	if (m_contact)
	{
		flipX();
	}

	if (m_timeElapsed > 2.0f)
	{
			m_state = IDLE;
			m_timeElapsed = 0.0f;
	}
}

void MushroomMonster::graze()
{
	m_sprite.playAnimation("graze", false);
	m_body.getBody()->SetLinearVelocity(b2Vec2(0.0f, m_body.getBody()->GetLinearVelocity().y));

	if (m_timeElapsed > 2.0f)
	{
			m_state = IDLE;
			m_timeElapsed = 0.0f;
	}
}

void MushroomMonster::init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug, bool facingRight)
{
	addComponent(&m_sprite);
	addComponent(&m_body);
	addComponent(&m_sprite);
	addComponent(&m_frontCheck);
	addComponent(&m_body);
	addComponent(&m_colliderMain);
	addComponent(&m_colliderJump);
	addComponent(&m_colliderCircle);
	addComponent(&m_photographable);

	m_sprite.addAnimation(glm::vec4(0.875f, 0.0f, 1.0f, 1.0f), 1, "run");
	m_sprite.addAnimation(glm::vec4(0.875f, 0.0f, 1.0f, 1.0f), 1, "idle");
	m_sprite.addAnimation(glm::vec4(0.0f, 0.0f, 0.125f, 1.0f), 8, "graze");
	m_sprite.playAnimation("idle");

	m_transform = glm::vec2(pos.x * 80, pos.y * 80);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	m_body = Body(this, pWorld, bodyDef, pos);

	b2Filter animalFilter;
	animalFilter.groupIndex = -2; // animal
	animalFilter.categoryBits = 0x0003;  // animal
	animalFilter.maskBits = 0x0001;  // terrain

	float xExtent = m_sprite.getDimensions().x / 80.0f / 3.0f;
	float yExtent = m_sprite.getDimensions().y / 80.0f / 3.3f;

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
	m_frontCheck.initBegin(std::bind(&MushroomMonster::contactEdge, this, std::placeholders::_1));
	m_frontCheck.initEnd(std::bind(&MushroomMonster::endContactEdge, this, std::placeholders::_1));
	m_frontCheck.getFixture()->SetFilterData(sensorFilter);

	b2Filter sensorFilterJump;
	sensorFilterJump.groupIndex = -2; // animal
	sensorFilterJump.categoryBits = 0x0004; // sensor
	sensorFilterJump.maskBits = 0x0002; // player

	m_colliderJump = Sensor(this, m_body.getBody(), pDebug,
			xExtent * 0.6, 0.1f, b2Vec2(0.0f, -yExtent), &m_colliderJump);
	m_colliderJump.initBegin(std::bind(&MushroomMonster::contactJump, this, std::placeholders::_1));
	m_colliderJump.initEnd(std::bind(&MushroomMonster::endContactJump, this, std::placeholders::_1));
	m_colliderJump.getFixture()->SetFilterData(sensorFilterJump);

	if (!facingRight)
		flipX();
	m_sprite.flipX(m_facingRight);
}

void MushroomMonster::contactEdge(Fixture* contact)
{
	m_contact = true;
}

void MushroomMonster::endContactEdge(Fixture* contact)
{
	m_contact = false;
}

void MushroomMonster::contactJump(Fixture* contact)
{
	if (!m_jumpTarget)
	{
		m_jumpTarget = dynamic_cast<Player*>(contact->getOwner());
		Body* pB = m_jumpTarget->getComponent<Body>();
		if (pB->getBody()->GetLinearVelocity().y > 1.0f)
			m_jumpTarget->jump(500.0f);
	}
}

void MushroomMonster::endContactJump(Fixture* contact)
{
	m_jumpTarget = nullptr;
}
