/*
 * Shape.h
 *
 *  Created on: 7 Apr 2021
 *      Author: mchlp
 */

#ifndef COMPONENTS_FIXTURE_H_
#define COMPONENTS_FIXTURE_H_

#include <glm/glm.hpp>
#include <Box2d/Box2d.h>
#include "../Component.h"

class Entity;

class Fixture : public Component
{
public:
	Fixture() = default;
	Fixture(Entity* pOwner);
	virtual ~Fixture() = default;

	void update();
	void init(b2Body* pBody, b2FixtureDef def, Fixture* fix);

	b2Fixture* getFixture();
protected:
	b2Fixture* m_pFixture;
};

#endif /* COMPONENTS_FIXTURE_H_ */
