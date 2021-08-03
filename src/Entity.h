/*
 * Entity.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "Component.h"

#include "Rendering/SpriteRenderer.h"
#include "Rendering/DebugRenderer.h"

class GameplayScene;

class Entity
{
public:
	Entity() = delete;
	Entity(GameplayScene* pParentScene, SpriteRenderer* pRenderer, DebugRenderer* pDebug);
	virtual ~Entity() = default;

	virtual void update(float deltaTime) = 0;
	virtual void camUpdate() = 0;
	virtual void render(float percent, glm::vec2 camera = glm::vec2(), float scale = 1.0f) = 0;

	glm::vec2 getPos() { return m_transform; }
	void move(glm::vec2 pos) { m_transform += pos; }
	void setPos(glm::vec2 pos) { m_transform = pos; }
	void setDepth(int d) { m_depth = d; }
	int getDepth() { return m_depth; }

	void addComponent(Component* pComp) { m_components.push_back(pComp); }

	// returns the first component of type CompType
	template <class CompType>
	CompType* getComponent();

	// returns a vector of components of type CompType
	template <class CompType>
	std::vector<CompType*> getComponents();
protected:
	std::vector<Component*> m_components;
	glm::vec2 m_transform;
	int m_depth;

	GameplayScene* m_pParentScene;
	SpriteRenderer* m_pRenderer;
	DebugRenderer* m_pDebug;
};

template <class CompType>
CompType* Entity::getComponent()
{
	for (Component* pComp : m_components)
	{
		CompType* pCompType = dynamic_cast<CompType*>(pComp);
		if (pCompType) return pCompType;
	}
	return nullptr;
}

template <class CompType>
std::vector<CompType*> Entity::getComponents()
{
	std::vector<CompType*> returnArray;
	for (Component* pComp : m_components)
	{
		CompType* pCompType = dynamic_cast<CompType*>(pComp);
		if (pCompType)
		{
			returnArray.push_back(pCompType);
		}
	}
	return returnArray;
}

#endif /* ENTITY_H_ */
