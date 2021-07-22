/*
 * Environment.cpp
 *
 *  Created on: 9 Jul 2021
 *      Author: mchlp
 */

#include "Environment.h"

Environment::Environment(b2World* pWorld, DebugRenderer* pDebug) :
	m_pWorld(pWorld),
	m_pDebug(pDebug)
{}

void Environment::init(tinyxml2::XMLElement* pLevel)
{
	tinyxml2::XMLElement* pNode = pLevel->FirstChildElement("node");

	while (pNode)
	{
		float x, y;
		pNode->QueryFloatAttribute("x", &x);
		pNode->QueryFloatAttribute("y", &y);

		m_nodes.emplace_back(b2Vec2(x, y));

		pNode = pNode->NextSiblingElement("node");
	}

	b2Vec2 vs[m_nodes.size()];
	std::copy(m_nodes.begin(), m_nodes.end(), vs);

	b2ChainShape chain;
	chain.CreateLoop(vs, m_nodes.size());

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(0.0f, 0.0f);
	b2Body* m_pBody = m_pWorld->CreateBody(&bodyDef);

	b2FixtureDef colliderDef;
	colliderDef.shape = &chain;
	colliderDef.density = 0.0f;
	colliderDef.friction = 0.3f;
	colliderDef.userData.pointer = reinterpret_cast<uintptr_t>(nullptr);
	b2Fixture* m_pFixture = m_pBody->CreateFixture(&colliderDef);
}

void Environment::render(glm::vec2 camera, float scale)
{
	for (auto i = 0; i < m_nodes.size() - 1; i++)
	{
		m_pDebug->drawLine(glm::vec2(m_nodes[i].x, m_nodes[i].y) * 80.0f * scale,
				glm::vec2(m_nodes[i + 1].x, m_nodes[i + 1].y) * 80.0f * scale, camera);
	}
	m_pDebug->drawLine(glm::vec2(m_nodes[0].x, m_nodes[0].y) * 80.0f * scale,
			glm::vec2(m_nodes[m_nodes.size() - 1].x, m_nodes[m_nodes.size() - 1].y) * 80.0f * scale, camera);
}
