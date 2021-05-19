/*
 * BoxCollider.h
 *
 *  Created on: 12 May 2021
 *      Author: mchlp
 */

#ifndef COMPONENTS_BOXCOLLIDER_H_
#define COMPONENTS_BOXCOLLIDER_H_

#include "Fixture.h"

class Entity;
class DebugRenderer;

class BoxCollider : public Fixture
{
public:
	BoxCollider() = default;
	BoxCollider(Entity* pOwner, b2Body* pBody, DebugRenderer* pDebug, float xExtent, float yExtent,
			b2Vec2 offset, float density, float friction);
	~BoxCollider() = default;

	void render(float percent, glm::vec2 camera, float scale = 1.0f);
	void flipX();
private:
	DebugRenderer* m_pDebug;
	float m_xExtent;
	float m_yExtent;
	b2Vec2 m_offset;
	int m_vertexCount;
};

#endif /* COMPONENTS_BOXCOLLIDER_H_ */
