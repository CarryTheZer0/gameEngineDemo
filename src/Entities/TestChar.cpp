/*
 * TestChar.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#include <functional>
#include <cmath>

#include "TestChar.h"

#include "../SpriteRenderer.h"
#include "../DebugRenderer.h"
#include "../InputHandler.h"
#include "../PhotographSystem.h"
#include "../Game.h"
#include "../GameplayScene.h"

Player::Player(Game* pGame, GameplayScene* pScene, SpriteRenderer* pRenderer, DebugRenderer* pDebug, InputHandler* pInput, PhotographSystem* pPhoto) :
	Entity(pScene, pRenderer, pDebug),
	m_sprite(this, pRenderer, "run", glm::vec4(0.0f, 0.0f, 0.2f, 1.0f), 0.2f, 0.2f),
	m_spriteHead(this, pRenderer, "run", glm::vec4(0.8f, 0.33f, 1.0f, 0.66f), glm::vec2(), 0.2f, glm::vec2(0.0f, -0.3f)),
	m_pGame(pGame),
	m_pInput(pInput),
	m_pPhoto(pPhoto),
	m_grounded(true),
	m_facingRight(true),
	m_shouldReset(false),
	m_snap(false),
	m_contactLeft(0),
	m_contactRight(0)
{}

void Player::update(float deltaTime)
{
	m_sprite.update(deltaTime);

	// check freefall
	if(!m_grounded && m_body.getBody()->GetLinearVelocity().y > 0)
	{
		m_sprite.playAnimation("fall", false);
	}

	float y = m_body.getBody()->GetLinearVelocity().y;
	float x = m_body.getBody()->GetLinearVelocity().x;

	// check lateral movement inputs
	if (((m_pInput->isKeyHeld(GLFW_KEY_D) && (!m_snap || !m_grounded)) || m_pInput->wasKeyPressed(GLFW_KEY_D))
			&& m_contactRight == 0)
	{
		flipX(true);
		m_body.getBody()->SetLinearVelocity(b2Vec2(3.3f, y));
		if (m_grounded)
		{
			m_snap = false;
			m_sprite.playAnimation("run", true);
		}
	}
	else if (((m_pInput->isKeyHeld(GLFW_KEY_A) && (!m_snap || !m_grounded)) || m_pInput->wasKeyPressed(GLFW_KEY_A))
			&& m_contactLeft == 0)
	{
		flipX(false);
		m_body.getBody()->SetLinearVelocity(b2Vec2(-3.3f, y));
		if (m_grounded)
		{
			m_snap = false;
			m_sprite.playAnimation("run", true);
		}
	}
	else if (m_grounded)
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(-0.0f, y));
		m_sprite.playAnimation("idle", false);
	}
	else
	{
		m_body.getBody()->SetLinearVelocity(b2Vec2(x * 0.95, y));
	}

	// check space - jump
	if (m_pInput->wasKeyPressed(GLFW_KEY_SPACE) && m_grounded)
	{
		m_snap = false;
		jump();
	}

	// check mouse 1 - photo
	if (m_pInput->wasMousePressed(GLFW_MOUSE_BUTTON_1))
	{
		m_snap = true;
		if (m_grounded)
		{
			m_sprite.playAnimation("snap", true);
			m_body.getBody()->SetLinearVelocity(b2Vec2(0.0f, y));
		}
		else
		{
			m_body.getBody()->SetLinearVelocity(b2Vec2(x * 0.95, y));
		}
	}
	else if (m_pInput->wasMouseReleased(GLFW_MOUSE_BUTTON_1))
	{
		m_snap = false;
	}

	// check snap flag for photo
	if (m_snap)
	{
		takePhoto();
	}

	if (m_shouldReset)
	{
		m_body.getBody()->SetTransform(b2Vec2(0.0f, 1.0f), 0.0f);
		m_body.getBody()->SetLinearVelocity(b2Vec2());
		m_shouldReset = false;
	}

	m_body.update();
}

void Player::camUpdate()
{
	if (m_snap)
	{
		glm::vec2 shadowOrigin;

		// TODO dont know why y component is offset and reversed - maybe box2d and openGL use opposite directions for y
		shadowOrigin.x = -m_pParentScene->getCamera().getPos().x + m_transform.x * m_pParentScene->getCamera().getScale();
		shadowOrigin.y = m_pParentScene->getCamera().getPos().y - m_transform.y * m_pParentScene->getCamera().getScale() + 768.0f;

		m_pRenderer->setShadows(shadowOrigin + glm::vec2(0.0f, 60.0f * m_pParentScene->getCamera().getScale()),
				glm::vec2(m_lookDirection.x, -m_lookDirection.y));

		m_pParentScene->getCamera().setTargetScale(0.7f);
		m_pParentScene->getCamera().setTargetPos(m_transform + m_mouseVector * 0.8f);
	}
	else
	{
		m_pParentScene->getCamera().setScale(1.0f);
		m_pParentScene->getCamera().setPos(m_transform);
	}
}

void Player::render(float percent, glm::vec2 camera, float scale)
{
	m_sprite.render(percent, camera, scale);
	m_colliderMain.render(percent, camera, scale);
	m_colliderCircle.render(percent, camera, scale);
	m_groundCheck.render(percent, camera, scale);
	m_leftCheck.render(percent, camera, scale);
	m_rightCheck.render(percent, camera, scale);
	if (m_snap)
		m_spriteHead.render(percent, camera, scale);
}

void Player::jump(float yForce)
{
	float x = m_body.getBody()->GetLinearVelocity().x;
	m_body.getBody()->SetLinearVelocity(b2Vec2(x, 0.0f));
	m_body.getBody()->ApplyForceToCenter(b2Vec2(0.0f, -yForce), true);
	m_sprite.playAnimation("jump", false);
}

void Player::reset()
{
	m_shouldReset = true;
}

void Player::flipX(bool facingRight)
{
	m_sprite.flipX(!facingRight);
	m_spriteHead.flipX(!facingRight);
}

void Player::takePhoto()
{
	float camScale = m_pParentScene->getCamera().getScale();
	glm::vec2 mousePos = m_pInput->getMousePos();

	if (mousePos.x > m_pGame->getWidth()) mousePos.x = m_pGame->getWidth();
	if (mousePos.x < 0) mousePos.x = 0;
	if (mousePos.y > m_pGame->getHeight()) mousePos.y = m_pGame->getHeight();
	if (mousePos.y < 0) mousePos.y = 0;

	m_mouseVector = mousePos + m_pParentScene->getCamera().getPos() -
			m_transform * camScale + glm::vec2(0.0f, 60.0f * camScale);

	m_lookDirection = glm::normalize(m_mouseVector);

	float temp = glm::dot(glm::vec2(1.0f, 0.0f), m_lookDirection);
	temp = glm::acos(temp);
	temp = glm::degrees(temp);
	if (m_lookDirection.y < 0) temp *= -1;

	if (m_lookDirection.x < 0)
	{
		flipX(false);
		temp += 180;
	}
	else
	{
		flipX(true);
	}

	m_spriteHead.setRotate(temp);

	if(m_pInput->wasMousePressed(GLFW_MOUSE_BUTTON_2))
	{
		m_pPhoto->takePhoto(m_transform - glm::vec2(0.0f, 60.0f), m_lookDirection);
		m_snap = false;
	}
}

void Player::init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug)
{
	addComponent(&m_sprite);
	addComponent(&m_body);
	addComponent(&m_colliderMain);
	addComponent(&m_colliderCircle);
	m_sprite.addAnimation(glm::vec4(0.0f, 0.0f, 0.2f, 0.33f), 4, "run");
	m_sprite.addAnimation(glm::vec4(0.8f, 0.0f, 1.0f, 0.33f), 1, "idle");
	m_sprite.addAnimation(glm::vec4(0.8f, 0.0f, 1.0f, 0.33f), 1, "snap");
	m_sprite.addAnimation(glm::vec4(0.0f, 0.33f, 0.2f, 0.66f), 4, "jump");
	m_sprite.addAnimation(glm::vec4(0.0f, 0.66f, 0.2f, 1.0f), 1, "fall");
	m_sprite.playAnimation("run");

	m_transform = glm::vec2(pos.x * 80, pos.y * 80);
	m_pParentScene->getCamera().setScale(1.0f);
	m_pParentScene->getCamera().setPos(m_transform);

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
			xExtent * 0.9f, yExtent - (xExtent / 2), b2Vec2(0.0f, -xExtent / 2), 1.33f, 0.3f,
			&m_colliderMain);
	m_colliderMain.getFixture()->SetFilterData(playerFilter); // TODO pass filter into constructor (maybe)

	m_colliderCircle = CircleCollider(this, m_body.getBody(), pDebug, xExtent, b2Vec2(0.0f, yExtent - xExtent),
			1.33f / 2.0f, 0.3f, &m_colliderCircle);
	m_colliderCircle.getFixture()->SetFilterData(playerFilter);

	b2Filter sensorFilter;
	sensorFilter.categoryBits = 0x0004; // sensor
	sensorFilter.maskBits = 0x0001; // terrain
	sensorFilter.groupIndex = -1; // player

	m_groundCheck = Sensor(this, m_body.getBody(), pDebug,
			xExtent * 0.8, 0.1f,
			b2Vec2(0.0f, yExtent),
			&m_groundCheck);
	m_groundCheck.initBegin(std::bind(&Player::contactFloor, this, std::placeholders::_1));
	m_groundCheck.initEnd(std::bind(&Player::endContactFloor, this, std::placeholders::_1));
	m_groundCheck.getFixture()->SetFilterData(sensorFilter);

	m_rightCheck = Sensor(this, m_body.getBody(), pDebug,
			0.1f, yExtent * 0.95f, b2Vec2(xExtent, -yExtent * 0.05), &m_rightCheck);
	m_rightCheck.initBegin(std::bind(&Player::contactEdge, this, std::placeholders::_1, true));
	m_rightCheck.initEnd(std::bind(&Player::endContactEdge, this, std::placeholders::_1, true));
	m_rightCheck.getFixture()->SetFilterData(sensorFilter);

	m_leftCheck = Sensor(this, m_body.getBody(), pDebug,
			0.1f, yExtent * 0.95f, b2Vec2(-xExtent, -yExtent * 0.05), &m_leftCheck);
	m_leftCheck.initBegin(std::bind(&Player::contactEdge, this, std::placeholders::_1, false));
	m_leftCheck.initEnd(std::bind(&Player::endContactEdge, this, std::placeholders::_1, false));
	m_leftCheck.getFixture()->SetFilterData(sensorFilter);
}

void Player::contactFloor(Fixture* contact)
{
	m_sprite.playAnimation("run", true);
	m_grounded = true;
}

void Player::endContactFloor(Fixture* contact)
{
	if (!m_groundCheck.getCollidingCount())
		m_grounded = false;
}

void Player::contactEdge(Fixture* contact, bool right)
{
	if (right) m_contactRight++;
	else m_contactLeft++;
}

void Player::endContactEdge(Fixture* contact, bool right)
{
	if (right) m_contactRight--;
	else m_contactLeft--;
}
