/*
 * GameplayScene.h
 *
 *  Created on: 9 Jul 2021
 *      Author: mchlp
 */

#ifndef GAMEPLAYSCENE_H_
#define GAMEPLAYSCENE_H_

#include "Scene.h"
#include <Box2d/Box2d.h>
#include <glm/glm.hpp>
#include <unordered_map>

#include "Camera.h"
#include "Environment.h"
#include "PhotographSystem.h"
#include "ContactListener.h"

class Entity;
class SceneLink;

class GameplayScene : public Scene
{
public:
	GameplayScene() = delete;
	GameplayScene(InputHandler* pInput, SpriteRenderer* pRenderer, DebugRenderer* pDebug,
			Game* pGame, SceneManager* pSceneManager, const char* filename);
	virtual ~GameplayScene();

	void loadScene() override;
	void saveScene() override;
	void startScene() override;
	void stopScene() override;

	void addSceneLink(glm::vec2 pos, float xExtent, float yExtent, const char* name);
	void linkTo(const char* name, unsigned int target);

	void update(float deltaTime) override;
	void render(float percent) override;

	void addEntity(Entity* pObject);
	void setCameraPos(glm::vec2 pos) { m_camera.setPos(pos); }

	Camera& getCamera() { return m_camera; }
	void setSpawnPoint(glm::vec2 spawn);
	void onEntityRemoved() { m_entityRemoved = true; }

	Player* getPlayer() { return m_pPlayer; }
protected:
	Camera m_camera;
	glm::vec2 m_spawnPoint;
	b2World* m_pWorld;
	Environment m_env;
	PhotographSystem m_photo;
	ContactListener m_contacts;
	Player* m_pPlayer;
	Game* m_parentGame;

	std::string m_filename;

	bool m_entityRemoved;
	std::vector<Entity*> m_entities;

	std::unordered_map<std::string, SceneLink*> m_sceneMap;

	void loadMap(const char* filepath);
};

#endif /* GAMEPLAYSCENE_H_ */
