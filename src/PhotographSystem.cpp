/*
 * PhotographSystem.cpp
 *
 *  Created on: 26 Apr 2021
 *      Author: mchlp
 */

#include "PhotographSystem.h"

void PhotographSystem::takePhoto(glm::vec2 pos, glm::vec2 dir)
{
	for (Animal* an : m_animals)
	{
		glm::vec2 dir2 = an->getPos() - pos;
		dir2 = glm::normalize(dir2);
		float temp = glm::dot(dir, dir2);
		temp = glm::acos(temp);
		if (temp < 0.3f)
		{
			an->proc();
		}
	}
}
