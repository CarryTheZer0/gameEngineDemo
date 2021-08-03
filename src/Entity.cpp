/*
 * Entity.cpp
 *
 *  Created on: 19 Jul 2021
 *      Author: mchlp
 */

#include "Entity.h"

#include "GameplayScene.h"

Entity::Entity(GameplayScene* pParentScene, SpriteRenderer* pRenderer, DebugRenderer* pDebug) :
	m_pParentScene(pParentScene),
	m_pRenderer(pRenderer),
	m_pDebug(pDebug),
	m_transform(glm::vec2(0.0f, 0.0f)),
	m_depth(0)
{}
