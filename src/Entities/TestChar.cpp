/*
 * TestChar.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#include <functional>

#include "TestChar.h"

#include "../SpriteRenderer.h"
#include "../DebugRenderer.h"
#include "../InputHandler.h"
#include "../PhotographSystem.h"
#include "../Game.h"

Player::Player(Game* pGame, SpriteRenderer* pRenderer, DebugRenderer* pDebug, InputHandler* pInput, PhotographSystem* pPhoto) :
	m_sprite(this, pRenderer, "run", glm::vec4(0.0f, 0.0f, 0.2f, 1.0f), 0.2f, 0.2f),
	m_spriteHead(this, pRenderer, "run", glm::vec4(0.8f, 0.33f, 1.0f, 0.66f), glm::vec2(), 0.2f, glm::vec2(0.0f, -0.3f)),
	m_pRenderer(pRenderer),
	m_pGame(pGame),
	m_pInput(pInput),
	m_pPhoto(pPhoto),
	m_grounded(true),
	m_facingRight(true),
	m_shouldReset(false),
	m_snap(false)
{}

void Player::update(float deltaTime)
{
	m_sprite.update(deltaTime);

	if(!m_grounded && m_body.getBody()->GetLinearVelocity().y > 0)
		{
			m_sprite.playAnimation("fall", false);
		}

	float y = m_body.getBody()->GetLinearVelocity().y;
	float x = m_body.getBody()->GetLinearVelocity().x;

	if (m_pInput->isKeyHeld(GLFW_KEY_D) && m_contact != 1 && !m_pInput->isMouseHeld(GLFW_MOUSE_BUTTON_1))
	{
		m_facingRight = true;
		m_body.getBody()->SetLinearVelocity(b2Vec2(3.3f, y));
		m_snap = false;
		if (m_grounded)
			m_sprite.playAnimation("run", true);
	}
	else if (m_pInput->isKeyHeld(GLFW_KEY_A) && m_contact != 2 && !m_pInput->isMouseHeld(GLFW_MOUSE_BUTTON_1))
	{
		m_facingRight = false;
		m_body.getBody()->SetLinearVelocity(b2Vec2(-3.3f, y));
		m_snap = false;
		if (m_grounded)
			m_sprite.playAnimation("run", true);
	}
	else if (m_grounded)
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(-0.0f, y));
		m_sprite.playAnimation("snap", false);
		m_snap = true;
	}
	else if (!m_pInput->isKeyHeld(GLFW_KEY_D) && !m_pInput->isKeyHeld(GLFW_KEY_A))
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(x * 0.95, y));
	}

	if (m_pInput->wasKeyPressed(GLFW_KEY_SPACE) && m_grounded)
	{
		jump(x);
	}

	if (m_pInput->isMouseHeld(GLFW_MOUSE_BUTTON_1) && m_grounded)
	{
		glm::vec2 direction = m_pInput->getMousePos() - m_pGame->getCamera() - m_transform + glm::vec2(0.0f, 70.0f);
		glm::vec2 mouseVec = glm::normalize(direction);
		float temp = glm::dot(glm::vec2(1.0f, 0.0f), mouseVec);
		temp = glm::acos(temp);
		temp = glm::degrees(temp);
		if (mouseVec.y < 0) temp *= -1;

		if (mouseVec.x < 0)
		{
			m_facingRight = false;
			temp += 180;
		}
		else
		{
			m_facingRight = true;
		}

		m_spriteHead.setRotate(temp);
		m_pRenderer->setShadows(m_pGame->getCamera() + m_transform + glm::vec2(0.0f, 60.0f),
				glm::vec2(mouseVec.x, -mouseVec.y));
		if(m_pInput->wasMousePressed(GLFW_MOUSE_BUTTON_2))
		{
			m_pPhoto->takePhoto(m_transform - glm::vec2(0.0f, 60.0f), mouseVec);
		}
	}

	if(m_facingRight)
	{
		m_sprite.flipX(false);
		m_spriteHead.flipX(false);
	}
	else
	{
		m_sprite.flipX(true);
		m_spriteHead.flipX(true);
	}

	if (m_shouldReset)
	{
		m_body.getBody()->SetTransform(b2Vec2(0.0f, 1.0f), 0.0f);
		m_body.getBody()->SetLinearVelocity(b2Vec2());
		m_shouldReset = false;
	}
	m_body.update();
}

void Player::render(float percent, glm::vec2 camera)
{
	m_sprite.render(percent, camera);
	m_colliderMain.render(percent, camera);
	m_colliderCircle.render(percent, camera);
	m_groundCheck.render(percent, camera);
	m_leftCheck.render(percent, camera);
	m_rightCheck.render(percent, camera);
	if (m_snap && m_grounded)
		m_spriteHead.render(percent, camera);
}

void Player::jump(float xVel)
{
	m_body.getBody()->SetLinearVelocity(b2Vec2(xVel, 0.0f));
	m_body.getBody()->ApplyForceToCenter(b2Vec2(0.0f, -300.0f), true);
	m_sprite.playAnimation("jump", false);
	m_grounded = false;
}

void Player::reset()
{
	m_shouldReset = true;
}

void Player::init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug)
{
	addComponent(&m_sprite);
	addComponent(&m_body);
	addComponent(&m_colliderMain);
	addComponent(&m_colliderCircle);
	m_sprite.addAnimation(glm::vec4(0.0f, 0.0f, 0.2f, 0.33f), 4, "run");
	m_sprite.addAnimation(glm::vec4(0.8f, 0.0f, 1.0f, 0.33f), 1, "snap");
	m_sprite.addAnimation(glm::vec4(0.0f, 0.33f, 0.2f, 0.66f), 4, "jump");
	m_sprite.addAnimation(glm::vec4(0.0f, 0.66f, 0.2f, 1.0f), 1, "fall");
	m_sprite.playAnimation("run");

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	m_body = Body(this, pWorld, bodyDef, pos);

	b2Filter playerFilter;
	playerFilter.categoryBits = 0x0002; // player
	playerFilter.maskBits = 0x0004  // sensor
						  | 0x0001; // terrain
	playerFilter.groupIndex = -1; // player

	float xExtent = m_sprite.getDimensions().x / 80.0f / 8.0f;
	float yExtent = m_sprite.getDimensions().y / 80.0f / 2.0f;

	m_colliderMain = BoxCollider(this, m_body.getBody(), pDebug,
			xExtent * 0.95f, yExtent - (xExtent / 2), b2Vec2(0.0f, -xExtent / 2), 1.33f, 0.3f);
	m_colliderMain.getFixture()->SetFilterData(playerFilter); // TODO pass filter into constructor (maybe)

	m_colliderCircle = CircleCollider(this, m_body.getBody(), pDebug, xExtent, b2Vec2(0.0f, yExtent - xExtent),
			1.33f / 2.0f, 0.3f);
	m_colliderCircle.getFixture()->SetFilterData(playerFilter);

	b2Filter sensorFilter;
	sensorFilter.categoryBits = 0x0004; // sensor
	sensorFilter.maskBits = 0x0001; // terrain
	sensorFilter.groupIndex = -1; // player

	m_groundCheck = Sensor(this, m_body.getBody(), pDebug,
			xExtent * 0.8, 0.1f,
			b2Vec2(0.0f, yExtent),
			&m_groundCheck);
	m_groundCheck.initBegin(std::bind(&Player::contactFloor, this));
	m_groundCheck.initEnd(std::bind(&Player::endContactFloor, this));
	m_groundCheck.getFixture()->SetFilterData(sensorFilter);

	m_rightCheck = Sensor(this, m_body.getBody(), pDebug,
			0.1f, yExtent * 0.9f, b2Vec2(xExtent, 0.0f), &m_rightCheck);
	m_rightCheck.initBegin(std::bind(&Player::contactEdge, this, true));
	m_rightCheck.initEnd(std::bind(&Player::endContactEdge, this));
	m_rightCheck.getFixture()->SetFilterData(sensorFilter);

	m_leftCheck = Sensor(this, m_body.getBody(), pDebug,
			0.1f, yExtent * 0.9f, b2Vec2(-xExtent, 0.0f), &m_leftCheck);
	m_leftCheck.initBegin(std::bind(&Player::contactEdge, this, false));
	m_leftCheck.initEnd(std::bind(&Player::endContactEdge, this));
	m_leftCheck.getFixture()->SetFilterData(sensorFilter);
}

void Player::contactFloor()
{
	m_sprite.playAnimation("run", true);
	m_grounded = true;
}

void Player::endContactFloor()
{
	if (!m_groundCheck.getCollidingCount())
		m_grounded = false;
}

void Player::contactEdge(bool right)
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

void Player::endContactEdge()
{
	m_contact = 0;
}
