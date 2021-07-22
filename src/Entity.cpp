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
	m_pDebug(pDebug)
{}
