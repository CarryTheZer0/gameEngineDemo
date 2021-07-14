/*
 * SceneLink.cpp
 *
 *  Created on: 12 Jul 2021
 *      Author: mchlp
 */

#include "SceneLink.h"
#include "../SceneManager.h"
#include "../GameplayScene.h"

SceneLink::SceneLink(SceneManager* pSceneManager, GameplayScene* pParentScene) :
	Entity(pParentScene, nullptr, nullptr),
	m_pSceneManager(pSceneManager),
	m_pParentScene(pParentScene),
	m_triggered(false),
	m_nextScene(0)
{}

void SceneLink::update(float deltaTime)
{
	m_body.update();
}

void SceneLink::render(float percent, glm::vec2 camera, float scale)
{
	m_sensor.render(percent, camera, scale);
}

void SceneLink::init(b2World* pWorld, glm::vec2 pos, float xExtent, float yExtent,
		DebugRenderer* pDebug, unsigned int target)
{
	addComponent(&m_body);
	addComponent(&m_sensor);

	m_nextScene = target;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.fixedRotation = true;
	m_body = Body(this, pWorld, bodyDef, pos);

	b2Filter sensorFilter;
	//animasensorFilterlFilter.groupIndex = -2;
	sensorFilter.categoryBits = 0x0004; // sensor
	sensorFilter.maskBits = 0x0002; // player

	m_sensor = Sensor(this, m_body.getBody(), pDebug,
			xExtent, yExtent, b2Vec2(), &m_sensor);
	m_sensor.initBegin(std::bind(&SceneLink::switchScene, this, std::placeholders::_1));
	m_sensor.getFixture()->SetFilterData(sensorFilter);
}

void SceneLink::switchScene(Fixture* contact)
{
	if (!m_triggered)
	{
		m_triggered = true;
		m_pSceneManager->switchScene(m_nextScene);
		m_pSceneManager->markForDeletion(m_pParentScene->getId());
	}
}
