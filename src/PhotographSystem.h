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

class UIManager;
class Player;

class PhotographSystem
{
public:
	PhotographSystem(Player* pPlayer, UIManager* pUI);
	~PhotographSystem() = default;
	void takePhoto(glm::vec2 pos, glm::vec2 dir);
	void addEntity(Entity* a) { m_targets.push_back(a); }

	std::vector<glm::vec2> computeCorners(Entity* ent);
	glm::vec2 projectPointOnVector(glm::vec2 point, glm::vec2 vec);

	std::vector<std::pair<float, glm::vec2>> generateShadows(std::vector<glm::vec2> nodes, float scale);
	glm::vec2 findIntersection(glm::vec2 a, glm::vec2 b, glm::vec2 aMin, glm::vec2 bMin);
	glm::vec2 rotateVector(glm::vec2, float angle);

	void setLookDir(glm::vec2 d) { m_lookDir = d; }
	void setCastOrigin(glm::vec2 c) { m_castOrigin = c; }
private:
	Player* m_pPlayer;
	UIManager* m_pUI;
	std::vector<Entity*> m_targets;
	glm::vec2 m_lookDir;
	glm::vec2 m_castOrigin;

	void closeUI(const char* buttonName, const char* UIName);
};

#endif /* PHOTOGRAPHSYSTEM_H_ */
