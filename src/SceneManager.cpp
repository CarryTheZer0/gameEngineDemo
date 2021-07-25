/*
 * SceneManager.cpp
 *
 *  Created on: 9 Jul 2021
 *      Author: mchlp
 */

#include "SceneManager.h"

SceneManager::SceneManager() :
	m_currentScene(0),
	m_nextId(0)
{}

SceneManager::~SceneManager()
{
	for (auto scenePair : m_sceneMap)
	{
		delete scenePair.second;
	}
}

unsigned int SceneManager::addScene(Scene* s)
{
	s->loadScene();
	m_sceneMap.insert({ m_nextId, s });
	s->setId(m_nextId);
	return m_nextId++;
}

void SceneManager::switchScene(unsigned int id)
{
	if (id != m_currentScene)
	{
		m_sceneMap[m_currentScene]->stopScene();

		m_currentScene = id;
		m_sceneMap[id]->startScene();
	}
}

void SceneManager::removeScene(unsigned int id)
{
	delete m_sceneMap[id];
	m_sceneMap.erase(id);
}

std::vector<unsigned int> SceneManager::getSceneIds()
{
	std::vector<unsigned int> returnList;
	for (auto pair : m_sceneMap)
	{
		returnList.push_back(pair.first);
	}
	return returnList;
}

void SceneManager::clean()
{
	for (unsigned int id : m_toDelete)
	{
		removeScene(id);
	}
	m_toDelete.clear();
}
