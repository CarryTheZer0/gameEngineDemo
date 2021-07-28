/*
 * Environment.h
 *
 *  Created on: 9 Jul 2021
 *      Author: mchlp
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <vector>

#include <Box2d/Box2d.h>
#include "tinyxml2.h"

#include "Rendering/DebugRenderer.h"

class Environment
{
public:
	Environment() = default;
	Environment(b2World* pWorld, DebugRenderer* pDebug);
	~Environment() = default;

	void init(tinyxml2::XMLElement* pLevel);

	void render(glm::vec2 camera, float scale);
	std::vector<glm::vec2> getNodes() { return m_nodes; }
private:
	b2World* m_pWorld;
	DebugRenderer* m_pDebug;

	std::vector<glm::vec2> m_nodes;
	b2Body* m_pBody;
	b2Fixture* m_pFixture;

	glm::vec2 findIntersection(glm::vec2 a, glm::vec2 b, glm::vec2 aMin, glm::vec2 bMin);
};

#endif /* ENVIRONMENT_H_ */
