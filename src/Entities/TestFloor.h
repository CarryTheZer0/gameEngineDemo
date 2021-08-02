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

class SpriteRenderer;

class Floor : public Entity
{
public:
	Floor(SpriteRenderer* pRenderer, glm::vec2 pos, float parallax);
	~Floor() = default;

	void update(float deltaTime) override;
	void camUpdate() override {}
	void render(float percent, glm::vec2 camera, float scale) override;
private:
	Sprite m_sprite;
	float m_parallax;
};

#endif /* ENTITIES_TESTFLOOR_H_ */
