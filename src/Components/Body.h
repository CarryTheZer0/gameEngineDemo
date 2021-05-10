/*
 * body.h
 *
 *  Created on: 7 Apr 2021
 *      Author: mchlp
 */

#ifndef COMPONENTS_BODY_H_
#define COMPONENTS_BODY_H_

#include <glm/glm.hpp>
#include <Box2d/Box2d.h>
#include "../Component.h"

class Entity;

class Body : public Component
{
public:
	Body() = default;
	Body(Entity* pOwner, b2World* pWorld, b2BodyDef def, glm::vec2 pos);
	~Body() = default;

	void update();

	b2Body* getBody() { return m_pBody; }
private:
	b2Body* m_pBody;
	void init(b2World* pWorld, b2BodyDef def, glm::vec2 pos);
};

#endif /* COMPONENTS_BODY_H_ */
