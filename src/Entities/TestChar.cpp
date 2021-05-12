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
	m_jumping(false),
	m_facingRight(true),
	m_snap(false)
{}

void Player::update(float deltaTime)
{
	m_sprite.update(deltaTime);

	if(m_jumping && m_body.getBody()->GetLinearVelocity().y > 0)
		{
			m_sprite.playAnimation("fall", false);
		}

	float y = m_body.getBody()->GetLinearVelocity().y;
	float x = m_body.getBody()->GetLinearVelocity().x;

	if (m_pInput->isKeyHeld(GLFW_KEY_D) && m_contact != 1)
	{
		m_facingRight = true;
		m_body.getBody()->SetLinearVelocity(b2Vec2(3.3f, y));
		m_snap = false;
		if (!m_jumping)
			m_sprite.playAnimation("run", true);
	}
	else if (m_pInput->isKeyHeld(GLFW_KEY_A) && m_contact != 2)
	{
		m_facingRight = false;
		m_body.getBody()->SetLinearVelocity(b2Vec2(-3.3f, y));
		m_snap = false;
		if (!m_jumping)
			m_sprite.playAnimation("run", true);
	}
	else if (!m_jumping)
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(-0.0f, y));
		m_sprite.playAnimation("snap", false);
		m_snap = true;
	}
	else
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(x * 0.95, y));
	}
	if (m_pInput->wasKeyPressed(GLFW_KEY_SPACE) && !m_jumping)
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(x, 0.0f));
		m_body.getBody()->ApplyForceToCenter(b2Vec2(0.0f, -900.0f), true);
		m_sprite.playAnimation("jump", false);
		m_jumping = true;
		m_snap = false;
	}

	if (m_pInput->isMouseHeld(GLFW_MOUSE_BUTTON_1))
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

	m_body.update();
}

void Player::render(float percent, glm::vec2 camera)
{
	m_sprite.render(percent, camera);
	m_colliderMain.render(percent, camera);
	m_groundCheck.render(percent, camera);
	m_leftCheck.render(percent, camera);
	m_rightCheck.render(percent, camera);
	if (m_snap)
		m_spriteHead.render(percent, camera);
}

void Player::init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug)
{
	addComponent(&m_sprite);
	addComponent(&m_body);
	addComponent(&m_colliderMain);
	m_sprite.addAnimation(glm::vec4(0.0f, 0.0f, 0.2f, 0.33f), 4, "run");
	m_sprite.addAnimation(glm::vec4(0.8f, 0.0f, 1.0f, 0.33f), 1, "snap");
	m_sprite.addAnimation(glm::vec4(0.0f, 0.33f, 0.2f, 0.66f), 4, "jump");
	m_sprite.addAnimation(glm::vec4(0.0f, 0.66f, 0.2f, 1.0f), 1, "fall");
	m_sprite.playAnimation("run");

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	m_body = Body(this, pWorld, bodyDef, pos);

	m_colliderMain = BoxCollider(this, m_body.getBody(), pDebug,
			m_sprite.getDimensions().x / 80.0f / 3.0f, m_sprite.getDimensions().y / 80.0f / 2.0f,
			b2Vec2(), 1.33f, 0.3f);

	m_groundCheck = Sensor(this, m_body.getBody(), pDebug,
			m_sprite.getDimensions().x / 80.0f / 3.1f, m_sprite.getDimensions().y / 80.0f / 10.0f,
			b2Vec2(0.0f, m_sprite.getDimensions().y / 80.0f / 2.0f), &m_groundCheck);
	m_groundCheck.initBegin(std::bind(&Player::contactFloor, this));

	m_rightCheck = Sensor(this, m_body.getBody(), pDebug,
			m_sprite.getDimensions().x / 80.0f / 10.0f, m_sprite.getDimensions().y / 80.0f / 2.1f,
			b2Vec2(m_sprite.getDimensions().x / 80.0f / 3.0f, 0.0f), &m_rightCheck);
	m_rightCheck.initBegin(std::bind(&Player::contactEdge, this, true));
	m_rightCheck.initEnd(std::bind(&Player::endContactEdge, this));

	m_leftCheck = Sensor(this, m_body.getBody(), pDebug,
			m_sprite.getDimensions().x / 80.0f / 10.0f, m_sprite.getDimensions().y / 80.0f / 2.1f,
			b2Vec2(-m_sprite.getDimensions().x / 80.0f / 3.0f, 0.0f), &m_leftCheck);
	m_leftCheck.initBegin(std::bind(&Player::contactEdge, this, false));
	m_leftCheck.initEnd(std::bind(&Player::endContactEdge, this));
}

void Player::contactFloor()
{
	m_sprite.playAnimation("run", true);
	m_jumping = false;
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
