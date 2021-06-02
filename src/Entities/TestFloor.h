/*
 * TestFoor.h
 *
 *  Created on: 7 Apr 2021
 *      Author: mchlp
 */

#ifndef ENTITIES_TESTFLOOR_H_
#define ENTITIES_TESTFLOOR_H_

#include <glm/glm.hpp>
#include "../Entity.h"
#include "../Components/Sprite.h"
#include "../Components/Body.h"
#include "../Components/BoxCollider.h"

class SpriteRenderer;
class DebugRenderer;

class Floor : public Entity
{
public:
	Floor(SpriteRenderer* pRenderer,  DebugRenderer* pDebug);
	~Floor() = default;

	void init(b2World* pWorld, glm::vec2 pos, DebugRenderer* pDebug);
	void update(float deltaTime) override;
	void camUpdate() override {}
	void render(float percent, glm::vec2 camera, float scale) override;
private:
	Sprite m_sprite;
	Body m_body;
	BoxCollider m_colliderMain;
};

#endif /* ENTITIES_TESTFLOOR_H_ */
