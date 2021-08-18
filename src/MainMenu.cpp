/*
 * MainMenu.cpp
 *
 *  Created on: 23 Jul 2021
 *      Author: mchlp
 */

#include "MainMenu.h"
#include "Game.h"

#include "Rendering/Texture.h"
#include "Rendering/SpriteRenderer.h"
#include "Rendering/DebugRenderer.h"
#include "Rendering/UIRenderer.h"
#include "Rendering/TextRenderer.h"
#include "Rendering/ResourceManager.h"

#include "GameplayScene.h"

MainMenu::MainMenu(InputHandler* pInput, SpriteRenderer* pRenderer, DebugRenderer* pDebug,
		UIRenderer* pUIRenderer, TextRenderer* pTextRenderer, Game* pGame, SceneManager* pSceneManager) :
	Scene(pInput, pRenderer, pDebug, pUIRenderer, pTextRenderer, pGame, pSceneManager)
{
	m_uiManager = UIManager(pInput);

	Style myStyle = Style(pUIRenderer, pTextRenderer, "UItest", 0.15f, 0.15f, 0.25f, 0.25f);

	m_uiManager.addStyle("test", myStyle);
}

void MainMenu::loadScene()
{
	Window* window = m_uiManager.loadUI("Resources/Data/UI/mainMenu.xml", "MainMenu");

	using std::placeholders::_1;
	window->addEvent("Exit", std::bind(&MainMenu::exitGame, this, _1));
	window->addEvent("StartGame", std::bind(&MainMenu::startGame, this, _1));
	window->addEvent("LoadGame", std::bind(&MainMenu::startGame, this, _1));
}

void MainMenu::update(float deltaTime)
{
	m_uiManager.update();
	m_pInput->clearKeys();
}

void MainMenu::render(float deltaTime)
{
    Texture2D texBack = ResourceManager::getTexture("background");
    m_pRenderer->drawSprite(texBack, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
    		glm::vec2(), glm::vec2(), glm::vec2(texBack.getWidth() * 0.7, texBack.getHeight() * 0.7));

    m_pRenderer->draw();

	m_uiManager.render();
}

void MainMenu::exitGame(const char* buttonName)
{
	m_pGame->exitGame();
}

void MainMenu::startGame(const char* buttonName)
{
	GameplayScene* sc1 = new GameplayScene(m_pInput, m_pRenderer, m_pDebug,
			m_pUIRenderer, m_pGame, m_pSceneManager, "level1");
	sc1->setSpawnPoint(glm::vec2(-1.5f, 3.5f));

	unsigned int sc1ID = m_pSceneManager->addScene(sc1);
	sc1->startScene();
	m_pSceneManager->switchScene(sc1ID);
}
