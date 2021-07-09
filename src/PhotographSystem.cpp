/*
 * PhotographSystem.cpp
 *
 *  Created on: 26 Apr 2021
 *      Author: mchlp
 */

#include "PhotographSystem.h"

#include <map>

void PhotographSystem::takePhoto(glm::vec2 pos, glm::vec2 dir)
{
	float lensAngle = 0.3;
	glm::vec2 bottomDir;
	if (dir.x > 0)
		lensAngle = 6.28319f - lensAngle;  // 2 pi - angle

	bottomDir = glm::mat2(glm::cos(lensAngle), -glm::sin(lensAngle), glm::sin(lensAngle), glm::cos(lensAngle)) * dir;

	std::map<float, std::tuple<Entity*, float, float>> hits;

	for (Entity* ent : m_targets)
	{
		std::vector<glm::vec2> coords = computeCorners(ent);

		glm::vec2 currentDir = glm::normalize(coords[0] - pos);

		float maxAngle = glm::acos(glm::dot(currentDir, bottomDir));
		float minAngle = maxAngle;

		float crossProdSign = (-bottomDir.x) * (-currentDir.y) - (-currentDir.x) * (-bottomDir.y);
		if (crossProdSign < 0)
		{
			maxAngle *= -1;
			minAngle *= -1;
		}
		if (dir.x > 0)
		{
			maxAngle *= -1;
			minAngle *= -1;
		}

		for (glm::vec2 coord : coords)
		{
			currentDir = glm::normalize(coord - pos);
			float currentAngle = glm::acos(glm::dot(currentDir, bottomDir));

			crossProdSign = (-bottomDir.x) * (-currentDir.y) - (-currentDir.x) * (-bottomDir.y);
			if (crossProdSign < 0) currentAngle *= -1;
			if (dir.x > 0) currentAngle *= -1;

			if (currentAngle < minAngle) minAngle = currentAngle;
			else if (currentAngle > maxAngle) maxAngle = currentAngle;
		}

		hits.emplace(glm::length(ent->getPos() - pos), std::make_tuple(ent, minAngle, maxAngle));
	}
}

std::vector<glm::vec2> PhotographSystem::computeCorners(Entity* ent)
{
	b2Transform transform = ent->getComponent<Body>()->getBody()->GetTransform();
	std::vector<Fixture*> pColliders = ent->getComponents<Fixture>();

	std::vector<glm::vec2> coords;

	for(Fixture* pColl : pColliders)
	{
		b2Fixture* pFix = pColl->getFixture();
		if (pFix->IsSensor()) continue;

		b2Shape* pShape = pFix->GetShape();
		b2AABB aabb;
		pShape->ComputeAABB(&aabb, transform, 0);

		coords.push_back(glm::vec2(aabb.upperBound.x, aabb.upperBound.y) * 80.0f);
		coords.push_back(glm::vec2(aabb.lowerBound.x, aabb.lowerBound.y) * 80.0f);
		coords.push_back(glm::vec2(aabb.upperBound.x, aabb.lowerBound.y) * 80.0f);
		coords.push_back(glm::vec2(aabb.lowerBound.x, aabb.upperBound.y) * 80.0f);
	}

	return coords;
}

glm::vec2 PhotographSystem::projectPointOnVector(glm::vec2 point, glm::vec2 vec)
{
	glm::vec2 returnVal;
	returnVal.x = glm::dot(point, vec) / dot(vec, vec) * vec.x;
	returnVal.y = glm::dot(point, vec) / dot(vec, vec) * vec.y;
	return returnVal;
}
