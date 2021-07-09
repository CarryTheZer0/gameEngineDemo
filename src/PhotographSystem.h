/*
 * PhotographSystem.h
 *
 *  Created on: 26 Apr 2021
 *      Author: mchlp
 */

#ifndef PHOTOGRAPHSYSTEM_H_
#define PHOTOGRAPHSYSTEM_H_

#include <vector>

#include "Entities/Charger.h"

class Player;

class PhotographSystem
{
public:
	void takePhoto(glm::vec2 pos, glm::vec2 dir);
	void addEntity(Entity* a) { m_targets.push_back(a); }

	std::vector<glm::vec2> computeCorners(Entity* ent);
	glm::vec2 projectPointOnVector(glm::vec2 point, glm::vec2 vec);
private:
	Player* m_pPlayer;
	std::vector<Entity*> m_targets;
};

#endif /* PHOTOGRAPHSYSTEM_H_ */
