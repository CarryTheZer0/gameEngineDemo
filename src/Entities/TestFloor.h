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
#include "../Components/Fixture.h"

class SpriteRenderer;

class Floor : public Entity
{
public:
	Floor(SpriteRenderer* pRenderer);
	~Floor() = default;

	void init(b2World* pWorld, glm::vec2 pos);
	void update(float deltaTime) override;
	void render(float percent, glm::vec2 camera) override;
private:
	Sprite m_sprite;
	Body m_body;
	Fixture m_fixture;
};

#endif /* ENTITIES_TESTFLOOR_H_ */
