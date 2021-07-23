/*
 * MainMenu.h
 *
 *  Created on: 23 Jul 2021
 *      Author: mchlp
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "Scene.h"
#include "UI/UIManager.h"

class MainMenu : public Scene
{
public:
	MainMenu() = delete;
	MainMenu(InputHandler* pInput, SpriteRenderer* pRenderer, DebugRenderer* pDebug,
			UIRenderer* pUIRenderer, Game* pGame, SceneManager* pSceneManager);

	void loadScene() override;
	void saveScene() override {};
	void update(float deltaTime) override;
	void render(float deltaTime) override;

	void exitGame(const char* buttonName);
	void startGame(const char* buttonName);
private:
	UIManager m_uiManager = NULL;
};

#endif /* MAINMENU_H_ */
