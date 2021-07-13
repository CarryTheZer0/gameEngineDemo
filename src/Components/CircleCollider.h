/*
 * CircleCollider.h
 *
 *  Created on: 13 May 2021
 *      Author: mchlp
 */

#ifndef COMPONENTS_CIRCLECOLLIDER_H_
#define COMPONENTS_CIRCLECOLLIDER_H_

#include "Fixture.h"

class Entity;
class DebugRenderer;

class CircleCollider : public Fixture
{
public:
	CircleCollider() = default;
	CircleCollider(Entity* pOwner, b2Body* pBody, DebugRenderer* pDebug, float radius,
			b2Vec2 offset, float density, float friction, Fixture* pointer);
	~CircleCollider() = default;

	void render(float percent, glm::vec2 camera, float scale = 1.0f);
	void flipX();
private:
	DebugRenderer* m_pDebug;
	float m_radius;
};

#endif /* COMPONENTS_CIRCLECOLLIDER_H_ */
