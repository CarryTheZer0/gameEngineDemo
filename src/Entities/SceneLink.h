/*
 * SceneLink.h
 *
 *  Created on: 12 Jul 2021
 *      Author: mchlp
 */

#ifndef ENTITIES_SCENELINK_H_
#define ENTITIES_SCENELINK_H_

#include "../Entity.h"
#include "../Components/Body.h"
#include "../Components/Sensor.h"

class DebugRenderer;
class SceneManager;
class Scene;

class SceneLink : public Entity
{
public:
	SceneLink() = delete;
	SceneLink(SceneManager* pSceneManager, Scene* pParentScene);
	~SceneLink() = default;

	void init(b2World* pWorld, glm::vec2 pos, float xExtent, float yExtent,
			DebugRenderer* pDebug, unsigned int target);

	void update(float deltaTime) override;
	void render(float percent, glm::vec2 camera, float scale) override;
	void camUpdate() override {}

	void setTarget(unsigned int t) { m_nextScene = t; }
private:
	Sensor m_sensor;
	Body m_body;
	SceneManager* m_pSceneManager;
	unsigned int m_nextScene;
	Scene* m_pParentScene;

	bool m_triggered;

	void switchScene(Fixture* contact);
};



#endif /* ENTITIES_SCENELINK_H_ */
