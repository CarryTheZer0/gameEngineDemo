/*
 * GameplayScene.cpp
 *
 *  Created on: 9 Jul 2021
 *      Author: mchlp
 */

#include "GameplayScene.h"

#include "Entity.h"
#include "SpriteRenderer.h"
#include "DebugRenderer.h"
#include "InputHandler.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Game.h"

#include "Entities/TestChar.h"
#include "Entities/TestFloor.h"
#include "Entities/Charger.h"
#include "Entities/MushroomMonster.h"
#include "Entities/SceneLink.h"

GameplayScene::GameplayScene(InputHandler* pInput, SpriteRenderer* pRenderer, DebugRenderer* pDebug,
		Game* pGame, SceneManager* pSceneManager, const char* filename) :
	Scene(pInput, pRenderer, pDebug, pGame, pSceneManager),
	m_filename(filename),
	m_entityRemoved(false)
{
	b2Vec2 gravity(0.0f, 10.0f);
	m_pWorld = new b2World(gravity);
	m_env = Environment(m_pWorld, m_pDebug);
	m_camera = Camera(pGame->getWidth(), pGame->getHeight());

	m_pWorld->SetContactListener(&m_contacts);

	// temp code TODO make init method & load data from file
	Player* playerTest = new Player(pGame, this, m_pRenderer, m_pDebug, m_pInput, &m_photo);
	playerTest->init(m_pWorld, glm::vec2(5.0f, 1.0f), m_pDebug);

	Charger* animalTest = new Charger(m_pRenderer, m_pDebug, playerTest);
	animalTest->init(m_pWorld, glm::vec2(12.0f, 9.0f), m_pDebug, true);

	MushroomMonster* animal2 = new MushroomMonster(m_pRenderer, m_pDebug);
	animal2->init(m_pWorld, glm::vec2(8.0f, 9.0f), m_pDebug, true);

	MushroomMonster* animal3 = new MushroomMonster(m_pRenderer, m_pDebug);
	animal3->init(m_pWorld, glm::vec2(17.0f, 9.0f), m_pDebug, false);

	addSceneLink(glm::vec2(-4.5f, 1.5f), 0.5f, 2.5f, "next");

	m_entities.emplace_back(playerTest);
	m_entities.emplace_back(animalTest);
	m_entities.emplace_back(animal2);
	m_entities.emplace_back(animal3);

	m_photo.addEntity(animalTest);
	m_photo.addEntity(animal2);
}

GameplayScene::~GameplayScene()
{
	delete m_pWorld;
	for (Entity* e : m_entities)
	{
		delete e;
	}
}

void GameplayScene::loadScene()
{
	m_env.init();
}

void GameplayScene::saveScene() {}

void GameplayScene::startScene()
{
    GameplayScene* nextScene = new GameplayScene(
    		m_pInput, m_pRenderer, m_pDebug, m_pGame, m_pSceneManager, "test");
    unsigned int sceneID = m_pSceneManager->addScene(nextScene);

    linkTo("next", sceneID);
}

void GameplayScene::stopScene() {}

void GameplayScene::addSceneLink(glm::vec2 pos, float xExtent, float yExtent, const char* name)
{
	SceneLink* link = new SceneLink(m_pSceneManager, this);
	link->init(m_pWorld, pos, xExtent, yExtent, m_pDebug, 0); // link to main menu

	m_entities.emplace_back(link);
	m_sceneMap.emplace(name, link);
}

void GameplayScene::linkTo(const char* name, unsigned int target)
{
	SceneLink* pLink = m_sceneMap[name];
	pLink->setTarget(target);
}

void GameplayScene::update(float deltaTime)
{
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	m_pWorld->Step(deltaTime, velocityIterations, positionIterations);
    m_pRenderer->setShadows(glm::vec2(), glm::vec2());

    for (Entity* e : m_entities)
    {
    	e->update(deltaTime);
    }
	m_pInput->clearKeys();

	m_camera.update(deltaTime);
	for (Entity* e : m_entities)
    {
    	e->camUpdate();
    }
}

void GameplayScene::render(float percent)
{
    Texture2D texBack = ResourceManager::getTexture("background");
    m_pRenderer->drawSprite(texBack, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
    		m_camera.getPos(), glm::vec2(), glm::vec2(texBack.getWidth() * 0.7, texBack.getHeight() * 0.7));

    m_env.render(m_camera.getPos(), m_camera.getScale());

    for (Entity* e : m_entities)
    {
    	e->render(percent, -m_camera.getPos(), m_camera.getScale());
    }

	m_pRenderer->draw();
	m_pDebug->draw();
}

void GameplayScene::addEntity(Entity* pObject) {}

void GameplayScene::setSpawnPoint(glm::vec2 spawn) {}

void GameplayScene::loadMap(const char* filepath) {}

