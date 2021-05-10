/*
 * PhotographSystem.h
 *
 *  Created on: 26 Apr 2021
 *      Author: mchlp
 */

#ifndef PHOTOGRAPHSYSTEM_H_
#define PHOTOGRAPHSYSTEM_H_

#include <vector>

#include "Entities/TestAnimal.h"

class Player;

class PhotographSystem
{
public:
	void takePhoto(glm::vec2 pos, glm::vec2 dir);
	void addEntity(Animal* a) { m_animals.push_back(a); }
private:
	Player* m_pPlayer;
	std::vector<Animal*> m_animals;
};

#endif /* PHOTOGRAPHSYSTEM_H_ */
