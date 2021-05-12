/*
 * AnimatedSprite.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#include "AnimatedSprite.h"
#include "../Entity.h"
#include "../SpriteRenderer.h"
#include "../DebugRenderer.h"
#include "Body.h"

AnimatedSprite::AnimatedSprite(Entity* pOwner, SpriteRenderer* pRenderer, const char* name,
		glm::vec4 srcRect, float timeToUpdate, float scale) :
	Sprite(pOwner, pRenderer, name, srcRect, glm::vec2(), scale),
	m_frame(0),
	m_elapsedTime(0),
	m_timeToUpdate(timeToUpdate),
	m_loop(true),
	m_currentAnimation()
{}

void AnimatedSprite::update(float deltaTime)
{
	m_elapsedTime += deltaTime;
	if (m_elapsedTime > m_timeToUpdate)
	{
		m_elapsedTime -= m_timeToUpdate;
		if (m_frame < m_animations[m_currentAnimation].frameCount - 1)
		{
			m_frame++;
		}
		else
		{
			if (m_loop)	m_frame = 0;
		}
	}
}

void AnimatedSprite::render(float percent, glm::vec2 camera)
{
	glm::vec2 pos = m_pOwner->getPos() + m_animations[m_currentAnimation].offset;

	glm::vec4 srcRect = m_animations[m_currentAnimation].origin;
	float width = (srcRect.z - srcRect.x);
	srcRect.x += width * m_frame;
	srcRect.z += width * m_frame;
	glm::vec2 size = glm::vec2((srcRect.z - srcRect.x) * m_texture.getWidth() * m_scale, (srcRect.w - srcRect.y) * m_texture.getHeight() * m_scale);
	float rotate = glm::degrees(m_pOwner->getComponent<Body>()->getBody()->GetAngle());
	if (m_flipX)
	{
		float temp;
		temp = srcRect.x;
		srcRect.x = srcRect.z;
		srcRect.z = temp;
	}
	m_pRenderer->drawSprite(m_texture, srcRect, pos, camera, size, rotate);
}

void AnimatedSprite::setCurrentAnimation(const char* name)
{
	m_currentAnimation = name;
}

void AnimatedSprite::addAnimation(glm::vec4 origin, int frameCount, std::string name)
{
	glm::vec2 offset;
	offset.x -= m_texture.getWidth() * (origin.z - origin.x) / 2 * m_scale;
	offset.y -= m_texture.getHeight() * (origin.w - origin.y) / 2 * m_scale;
	m_animations.insert({ name, { frameCount, origin, offset } });
}

void AnimatedSprite::playAnimation(const char* name, bool loop)
{
	if (m_currentAnimation != name)
	{
		m_frame = 0;
		m_elapsedTime = 0.0f;
		m_currentAnimation = name;
	}

	m_loop = loop;
}

glm::vec2 AnimatedSprite::getDimensions()
{
	glm::vec4 origin = m_animations[m_currentAnimation].origin;
	return glm::vec2(m_texture.getWidth() * (origin.z - origin.x) * m_scale,
			m_texture.getHeight() * (origin.w - origin.y) * m_scale);
}
