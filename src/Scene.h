/*
 * Scene.h
 *
 *  Created on: 9 Jul 2021
 *      Author: mchlp
 */

#ifndef SCENE_H_
#define SCENE_H_


#include <vector>
#include <string>

class InputHandler;
class SceneManager;
class Game;
class SpriteRenderer;
class DebugRenderer;
class UIRenderer;

class Scene
{
public:
	Scene() = delete;
	Scene(InputHandler* pInput, SpriteRenderer* pRenderer, DebugRenderer* pDebug,
			UIRenderer* pUIRenderer, Game* pGame, SceneManager* pSceneManager) :
		m_pInput(pInput),
		m_pRenderer(pRenderer),
		m_pDebug(pDebug),
		m_pUIRenderer(pUIRenderer),
		m_pGame(pGame),
		m_pSceneManager(pSceneManager),
		m_id(0)
	{}
	virtual ~Scene() = default;

	/* void LoadScene
	 * Loads scene data from an xml file
	 */
	virtual void loadScene() = 0;

	/* void SaveScene
	 * Saves scene data to an xml file
	 */
	virtual void saveScene() = 0;

	virtual void startScene() {}

	virtual void stopScene() {}

	virtual void update(float deltaTime) = 0;
	virtual void render(float percent) = 0;

	unsigned int getId() { return m_id; }
	void setId(unsigned int id) { m_id = id; }

protected:
	unsigned int m_id;
	SceneManager* m_pSceneManager;
	Game* m_pGame;
	InputHandler* m_pInput;
	SpriteRenderer* m_pRenderer;
	DebugRenderer* m_pDebug;
	UIRenderer* m_pUIRenderer;
};

#endif /* SCENE_H_ */
