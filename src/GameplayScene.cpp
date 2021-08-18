/*
 * GameplayScene.cpp
 *
 *  Created on: 9 Jul 2021
 *      Author: mchlp
 */

#include "GameplayScene.h"

#include "tinyxml2.h"
#include <assert.h>
#include <vector>

#include "Entities/Prop.h"
#include "Entity.h"
#include "InputHandler.h"
#include "Game.h"

#include "Rendering/Texture.h"
#include "Rendering/SpriteRenderer.h"
#include "Rendering/DebugRenderer.h"
#include "Rendering/UIRenderer.h"
#include "Rendering/ResourceManager.h"
#include "Rendering/RenderQueue.h"

#include "Entities/TestChar.h"
#include "Entities/SceneLink.h"
#include "Spawner.h"

GameplayScene::GameplayScene(InputHandler* pInput, SpriteRenderer* pRenderer, DebugRenderer* pDebug,
		UIRenderer* pUIRenderer, Game* pGame, SceneManager* pSceneManager, const char* filename) :
	Scene(pInput, pRenderer, pDebug, pUIRenderer, nullptr, pGame, pSceneManager),
	m_filename(filename),
	m_entityRemoved(false),
	m_parentGame(pGame),
	m_pPlayer(nullptr),
	m_photo(m_pPlayer, &m_ui, &m_env)
{
	b2Vec2 gravity(0.0f, 10.0f);
	m_pWorld = new b2World(gravity);
	m_env = Environment(m_pWorld, m_pDebug);
	m_camera = Camera(pGame->getWidth(), pGame->getHeight());
	m_ui = UIManager(pInput);

	Style myStyle = Style(pUIRenderer, nullptr, "UItest", 0.15f, 0.15f, 0.25f, 0.25f);

	m_ui.addStyle("test", myStyle);

	m_pWorld->SetContactListener(&m_contacts);
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
	// load xml file
	tinyxml2::XMLDocument doc;
	std::string filename = "Resources/Data/" + m_filename + ".xml";
	assert(doc.LoadFile(filename.c_str()) == 0 && "No xml specification found!");
	tinyxml2::XMLElement* pScene = doc.FirstChildElement("scene");

	// load level terrain data
	tinyxml2::XMLElement* pLevel = pScene->FirstChildElement("level");
	m_env.init(pLevel);

	tinyxml2::XMLElement* pSceneMax = pLevel->FirstChildElement("sceneMax");
	float x, y;
	pSceneMax->QueryFloatAttribute("x", &x);
	pSceneMax->QueryFloatAttribute("y", &y);
	m_camera.setMax(glm::vec2(x * 80.0f, y * 80.0f));

	tinyxml2::XMLElement* pSceneMin = pLevel->FirstChildElement("sceneMin");
	pSceneMin->QueryFloatAttribute("x", &x);
	pSceneMin->QueryFloatAttribute("y", &y);
	m_camera.setMin(glm::vec2(x * 80.0f, y * 80.0f));

	// load player
	tinyxml2::XMLElement* pPlayer = pScene->FirstChildElement("player");
	Player* player = new Player(m_parentGame, this, m_pRenderer, m_pDebug, m_pInput, &m_photo);
	player->init(m_pWorld, m_spawnPoint, m_pDebug);
	m_pPlayer = player;
	m_entities.emplace_back(player);

	// load entities
	tinyxml2::XMLElement* pEntities = pScene->FirstChildElement("entities");
	tinyxml2::XMLElement* pEntity = pEntities->FirstChildElement("entity");

	while (pEntity)
	{
		bool facingRight;
		const char* name;
		pEntity->QueryFloatAttribute("x", &x);
		pEntity->QueryFloatAttribute("y", &y);
		pEntity->QueryBoolAttribute("facingRight", &facingRight);
		pEntity->QueryStringAttribute("name", &name);

		Entity* ent = Spawner::spawnEntity(name, glm::vec2(x, y), facingRight,
				this, m_pRenderer, m_pDebug, m_pWorld);
		if (ent)
			m_entities.emplace_back(ent);
			m_photo.addEntity(ent);

		pEntity = pEntity->NextSiblingElement("entity");
	}

	// load scene links
	tinyxml2::XMLElement* pSceneLinks = pScene->FirstChildElement("sceneLinks");
	tinyxml2::XMLElement* pSceneLink = pSceneLinks->FirstChildElement("sceneLink");

	while (pSceneLink)
	{
		float x, y, w, h;
		const char* name;
		pSceneLink->QueryFloatAttribute("x", &x);
		pSceneLink->QueryFloatAttribute("y", &y);
		pSceneLink->QueryFloatAttribute("halfWidth", &w);
		pSceneLink->QueryFloatAttribute("halfHeight", &h);
		pSceneLink->QueryStringAttribute("linkName", &name);

		addSceneLink(glm::vec2(x, y), w, h, name);

		pSceneLink = pSceneLink->NextSiblingElement("sceneLink");
	}

	// load map
	tinyxml2::XMLElement* pSet = pScene->FirstChildElement("set");
	tinyxml2::XMLElement* pProp = pSet->FirstChildElement("prop");

	while (pProp)
	{
		float x, y, parallax;
		int depth;
		const char* name;
		pProp->QueryFloatAttribute("x", &x);
		pProp->QueryFloatAttribute("y", &y);
		pProp->QueryFloatAttribute("parallax", &parallax);
		pProp->QueryIntAttribute("depth", &depth);
		pProp->QueryStringAttribute("name", &name);

		Entity* prop = new Prop(m_pRenderer, glm::vec2(80.0f * x, 80.0f * y), name, parallax);
		prop->setDepth(depth);
		m_entities.emplace_back(prop);

		pProp = pProp->NextSiblingElement("prop");
	}
}

void GameplayScene::saveScene() {}

void GameplayScene::startScene()
{
	// load file
	tinyxml2::XMLDocument doc;
	std::string filename = "Resources/Data/" + m_filename + ".xml";
	assert(doc.LoadFile(filename.c_str()) == 0 && "No xml specification found!");

	tinyxml2::XMLElement* pScene = doc.FirstChildElement("scene");
	tinyxml2::XMLElement* pSceneLinks = pScene->FirstChildElement("sceneLinks");
	tinyxml2::XMLElement* pSceneLink = pSceneLinks->FirstChildElement("sceneLink");

	// initialise and link adjacent scenes
	while (pSceneLink)
	{
		const char* linkName;
		const char* fileName;
		float x, y;
		pSceneLink->QueryStringAttribute("linkName", &linkName);
		pSceneLink->QueryStringAttribute("fileName", &fileName);
		pSceneLink->QueryFloatAttribute("spawnX", &x);
		pSceneLink->QueryFloatAttribute("spawnY", &y);

	    GameplayScene* nextScene = new GameplayScene(
	    		m_pInput, m_pRenderer, m_pDebug, m_pUIRenderer, m_pGame,
				m_pSceneManager, fileName);

	    nextScene->setSpawnPoint(glm::vec2(x, y));
	    unsigned int sceneID = m_pSceneManager->addScene(nextScene);
	    linkTo(linkName, sceneID);

		pSceneLink = pSceneLink->NextSiblingElement("sceneLink");
	}
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
	if (pLink)
		pLink->setTarget(target);
}

void GameplayScene::update(float deltaTime)
{
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	m_pWorld->Step(deltaTime, velocityIterations, positionIterations);
    m_pRenderer->setShadowOrigin(glm::vec2());

    for (Entity* e : m_entities)
    {
    	e->update(deltaTime);
    }
	m_ui.update();
	m_pInput->clearKeys();

	m_camera.update(deltaTime);
	for (Entity* e : m_entities)
    {
    	e->camUpdate();
    }
	m_camera.clamp();
}

void GameplayScene::render(float percent)
{
    m_env.render(m_camera.getPos(), m_camera.getScale());

    // process filter for camera line of sight
    std::vector<std::pair<float, glm::vec2>> nodes = m_photo.generateShadows(m_camera.getScale());
    for (auto node : nodes)
    {
    	//m_pDebug->drawLine(node.second, m_pPlayer->getPos() * m_camera.getScale() - glm::vec2(0.0f, 60.0f * m_camera.getScale()), glm::vec2());
        m_pRenderer->drawLineOfSightFilter(glm::vec2(
        		-m_camera.getPos().x + node.second.x ,
    			m_camera.getPos().y - node.second.y +  m_pGame->getHeight()
        ));
    }

    RenderQueue::sort(m_entities);
    for (Entity* e : m_entities)
    {
    	e->render(percent, -m_camera.getPos(), m_camera.getScale());
    }

	m_pRenderer->draw();
	//m_pDebug->draw();

	m_ui.render();
	m_pUIRenderer->draw();
}

void GameplayScene::addEntity(Entity* pObject)
{
	m_entities.emplace_back(pObject);
	if (true)
		m_photo.addEntity(pObject);
}

void GameplayScene::loadMap(const char* filepath) {}
