/*
 * Spawner.cpp
 *
 *  Created on: 21 Jul 2021
 *      Author: mchlp
 */

#include "Spawner.h"

#include "Entity.h";
#include "SpriteRenderer.h";
#include "DebugRenderer.h";
#include "GameplayScene.h";
#include <Box2d/Box2d.h>

#include "Entities/Charger.h"
#include "Entities/MushroomMonster.h"

Entity* Spawner::spawnEntity(std::string name, glm::vec2 pos, bool facingRight,
		GameplayScene* pScene, SpriteRenderer* pRenderer, DebugRenderer* pDebug,
		b2World* pWorld)
{
	if (name == "charger")
	{
		Charger* toReturn = new Charger(pScene, pRenderer, pDebug);
		toReturn->init(pWorld, pos, pDebug, facingRight);
		return toReturn;
	}
	else if (name == "mushroom")
	{
		MushroomMonster* toReturn = new MushroomMonster(pScene, pRenderer, pDebug);
		toReturn->init(pWorld, pos, pDebug, facingRight);
		return toReturn;
	}
}
