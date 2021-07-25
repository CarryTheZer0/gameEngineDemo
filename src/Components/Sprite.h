/*
 * Sprite.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#ifndef COMPONENTS_SPRITE_H_
#define COMPONENTS_SPRITE_H_

#include <glm/glm.hpp>
#include "../Component.h"
#include "../Rendering/Texture.h"

class Entity;
class SpriteRenderer;
class DebugRenderer;

class Sprite : public Component
{
public:
	Sprite() = default;
	Sprite(Entity* pOwner, SpriteRenderer* pRenderer, const char* name,
			glm::vec4 srcRect, glm::vec2 offset, float scale, glm::vec2 rotateOffset = glm::vec2());
	virtual ~Sprite() = default;

	virtual void render(float percent, glm::vec2 camera, float scale = 1.0f);

	virtual glm::vec2 getDimensions();

	float getRotate() { return m_rotate; }
	void setRotate(float rotate) { m_rotate = rotate; }

	void flipX(bool flip) { m_flipX = flip; }
protected:
	SpriteRenderer* m_pRenderer;
	DebugRenderer* m_pDebug;
	Texture2D m_texture;
	glm::vec4 m_srcRect;
	glm::vec2 m_offset;
	glm::vec2 m_prevPos;
	float m_scale;
	float m_rotate;
	glm::vec2 m_rotateOffset;
	bool m_flipX;
};

#endif /* COMPONENTS_SPRITE_H_ */
