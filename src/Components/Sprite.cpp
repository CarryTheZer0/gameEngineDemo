/*
 * Sprite.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#include "../Rendering/SpriteRenderer.h"
#include "../Entity.h"
#include "../Rendering/ResourceManager.h"
#include "Sprite.h"

Sprite::Sprite(Entity* pOwner, SpriteRenderer* pRenderer, const char* name,
		glm::vec4 srcRect, glm::vec2 offset, float scale, glm::vec2 rotateOffset) :
	Component(pOwner),
	m_pRenderer(pRenderer),
	m_srcRect(srcRect),
	m_offset(offset),
	m_scale(scale),
	m_rotateOffset(rotateOffset),
	m_rotate(0),
	m_flipX(false)
{
	m_texture = ResourceManager::getTexture(name);
	m_offset.x -= m_texture.getWidth() * (srcRect.z - srcRect.x) / 2 * m_scale;
	m_offset.y -= m_texture.getHeight() * (srcRect.w - srcRect.y) / 2 * m_scale;
}

void Sprite::render(float percent, glm::vec2 camera, float scale)
{
	glm::vec2 pos = m_pOwner->getPos() * scale + m_offset * scale;
	glm::vec2 size = glm::vec2((m_srcRect.z - m_srcRect.x) * m_texture.getWidth() * m_scale,
			(m_srcRect.w - m_srcRect.y) * m_texture.getHeight() * m_scale);
	size *= scale;
	glm::vec4 srcRect = m_srcRect;
	if (m_flipX)
	{
		float temp;
		temp = srcRect.x;
		srcRect.x = srcRect.z;
		srcRect.z = temp;
	}

	m_pRenderer->drawSprite(m_texture, srcRect, pos, camera, size, m_rotate, m_rotateOffset);
}

glm::vec2 Sprite::getDimensions()
{
	return glm::vec2(m_texture.getWidth() * (m_srcRect.z - m_srcRect.x) * m_scale,
			m_texture.getHeight() * (m_srcRect.w - m_srcRect.y) * m_scale);
}
