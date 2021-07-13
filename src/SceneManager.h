/*
 * SceneManager.h
 *
 *  Created on: 9 Jul 2021
 *      Author: mchlp
 */

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include <vector>
#include <unordered_map>

#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	unsigned int addScene(Scene* s);
	void switchScene(unsigned int id);
	void removeScene(unsigned int id);

	Scene* getCurrentScene() { return m_sceneMap[m_currentScene]; }
	std::vector<unsigned int> getSceneIds();

	void markForDeletion(unsigned int s) { m_toDelete.emplace_back(s); }
	void clean();
private:
	std::unordered_map<unsigned int, Scene*> m_sceneMap;
	unsigned int m_currentScene;
	unsigned int m_nextId;

	std::vector<unsigned int> m_toDelete;
};

#endif /* SCENEMANAGER_H_ */
