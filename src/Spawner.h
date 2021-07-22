/*
 * Spawner.h
 *
 *  Created on: 14 Jul 2021
 *      Author: mchlp
 */

#ifndef SPAWNER_H_
#define SPAWNER_H_

#include <string>
#include <glm/glm.hpp>

class Entity;
class SpriteRenderer;
class DebugRenderer;
class GameplayScene;
class b2World;

class Spawner
{
public:
	Spawner() = default;
	~Spawner() = default;

	static Entity* spawnEntity(std::string name, glm::vec2 pos, bool facingRight,
			GameplayScene* pScene, SpriteRenderer* pRenderer, DebugRenderer* pDebug,
			b2World* pWorld);
};

#endif /* SPAWNER_H_ */
