/*
 * Photographable.cpp
 *
 *  Created on: 26 Jul 2021
 *      Author: mchlp
 */

#include "../Entity.h"
#include "Photographable.h"
#include "../Rendering/ResourceManager.h"

Photographable::Photographable(Entity* pOwner, const char* nameId) :
	Component(pOwner),
	m_nameId(nameId)
{
	float w = ResourceManager::getTexture(nameId).getWidth();
	float h = ResourceManager::getTexture(nameId).getHeight();

	m_widthRatio = w/h;
}
