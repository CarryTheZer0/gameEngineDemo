/*
 * PhotographSystem.cpp
 *
 *  Created on: 26 Apr 2021
 *      Author: mchlp
 */

#include "PhotographSystem.h"

#include <map>
#include <algorithm>
#include <string>
#include "UI/UIManager.h"
#include "Entities/TestChar.h"
#include "Components/Photographable.h"

PhotographSystem::PhotographSystem(Player* pPlayer, UIManager* pUI) :
	m_pPlayer(pPlayer),
	m_pUI(pUI),
	m_lookDir(),
	m_castOrigin()

{}

void PhotographSystem::takePhoto(glm::vec2 pos, glm::vec2 dir)
{
	float lensAngle = 0.3;
	glm::vec2 bottomDir;
	if (dir.x > 0)
		lensAngle = 6.28319f - lensAngle;  // 2 pi - angle

	bottomDir = glm::mat2(glm::cos(lensAngle), -glm::sin(lensAngle), glm::sin(lensAngle), glm::cos(lensAngle)) * dir;

	std::map<float, std::tuple<Entity*, float, float>> hits;

	// find the min and max corner of each entity relative to the bottom of the field of view
	for (Entity* ent : m_targets)
	{
		std::vector<glm::vec2> coords = computeCorners(ent);

		// set initial values for min/max
		glm::vec2 currentDir = glm::normalize(coords[0] - pos);
		float maxAngle = glm::acos(glm::dot(currentDir, bottomDir));
		float minAngle = maxAngle;

		// adjust sign
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

			// adjust sign
			crossProdSign = (-bottomDir.x) * (-currentDir.y) - (-currentDir.x) * (-bottomDir.y);
			if (crossProdSign < 0) currentAngle *= -1;
			if (dir.x > 0) currentAngle *= -1;

			// save the min/max values
			if (currentAngle < minAngle) minAngle = currentAngle;
			else if (currentAngle > maxAngle) maxAngle = currentAngle;
		}

		hits.emplace(-glm::length(ent->getPos() - pos), std::make_tuple(ent, minAngle, maxAngle));
	}

	Window* pWindow = m_pUI->loadUI("Resources/Data/UI/photoTest.xml", "PhotoTest");

	for (auto hit : hits)
	{
		Photographable* photo = std::get<0>(hit.second)->getComponent<Photographable>();
		if (photo)
		{
			float minPercent = std::get<1>(hit.second)/0.6f * 100.0f;
			float maxPercent = std::get<2>(hit.second)/0.6f * 100.0f;

			// only draw entities in range
			if ((minPercent < 0 && maxPercent < 0) || (minPercent > 100 && maxPercent > 100)) continue;

			Face* pParent = pWindow->getAnchor("ImageBase");
			Face* pNewFace = m_pUI->addElement(pParent, pWindow, "image", "ImagePanel", "test");
			pNewFace->setProperty("imageName", photo->getNameId());

			drawElement(minPercent, maxPercent, pNewFace, photo->getWidthRatio());
		}
	}

	using std::placeholders::_1;
	pWindow->addEvent("Exit", std::bind(&PhotographSystem::closeUI, this, _1, "PhotoTest"));
}

std::vector<glm::vec2> PhotographSystem::computeCorners(Entity* ent)
{
	b2Transform transform = ent->getComponent<Body>()->getBody()->GetTransform();
	std::vector<Fixture*> pColliders = ent->getComponents<Fixture>();

	std::vector<glm::vec2> coords;

	// add each corner of the AABB for each collider
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

std::vector<std::pair<float, glm::vec2>> PhotographSystem::generateShadows(
		std::vector<glm::vec2> nodes, float scale)
{
	std::vector<glm::vec2> rays;

	// add rays for FOV limits
	float lensAngle = 0.3f;
	glm::vec2 startRay = rotateVector(m_lookDir, lensAngle);
	rays.push_back(startRay);
	glm::vec2 endRay = rotateVector(m_lookDir, -lensAngle);
	rays.push_back(endRay);

	// add 3 rays for each corner
	for (glm::vec2 n : nodes)
	{
		glm::vec2 node = 80.0f * scale * n;
		glm::vec2 ray = node - m_castOrigin;
		ray = glm::normalize(ray);

		// cull OOB rays
		if (glm::acos(glm::dot(ray, m_lookDir)) > lensAngle) continue;

		float angle = 0.0001f;
		glm::vec2 rayUpper = rotateVector(ray, angle);
		glm::vec2 rayLower = rotateVector(ray, -angle);

		rays.push_back(ray);
		rays.push_back(rayUpper);
		rays.push_back(rayLower);
	}

	std::vector<std::pair<float, glm::vec2>> filterPolyVertices;

	// cast rays to closest intersection point
	glm::vec2 currentPoint;
	float currentLength;
	nodes.emplace_back(nodes[0]);
	for (glm::vec2 ray : rays)
	{
		float screenLimit = 1024.0f * 1.5f;
		currentPoint = m_castOrigin + screenLimit * ray;
		currentLength = screenLimit;

		// for each edge in the terrain, compute closest intersection
		for (auto i = 0; i < nodes.size() - 1; i++)
		{
			glm::vec2 nodeA = glm::vec2(nodes[i].x * 80.0f * scale, nodes[i].y * 80.0f * scale);
			glm::vec2 nodeB = glm::vec2(nodes[i + 1].x * 80.0f * scale, nodes[i + 1].y * 80.0f * scale);
			glm::vec2 output = findIntersection(screenLimit * ray, nodeB - nodeA, m_castOrigin, nodeA);
			if (output != glm::vec2())
			{
				float newLength = glm::length(output - m_castOrigin);
				if (newLength < currentLength)
				{
					currentPoint = output;
					currentLength = newLength;
				}
			}
		}

		// compute angle value for each ray - this is the angle between the ray and an arbitrary base vector
		glm::vec2 castDirection = currentPoint - m_castOrigin;
		castDirection = glm::normalize(castDirection);
		glm::vec2 baseAngle = rotateVector(endRay, -0.1f);
		float angle = glm::acos(glm::dot(baseAngle, castDirection));

		filterPolyVertices.push_back(std::make_pair(angle, currentPoint));
	}

	// sort according to angle
	std::sort(filterPolyVertices.begin(), filterPolyVertices.end(),
		[&](const std::pair<float, glm::vec2> &t1, const std::pair<float, glm::vec2> &t2)
		{
			return t1.first < t2.first;
		});

	return filterPolyVertices;
}

glm::vec2 PhotographSystem::findIntersection(glm::vec2 a, glm::vec2 b, glm::vec2 aMin, glm::vec2 bMin)
{
	float s, t;

	s = (-a.y * (aMin.x - bMin.x) + a.x * (aMin.y - bMin.y)) / (-b.x * a.y + a.x * b.y);
	t = (b.x * (aMin.y - bMin.y) - b.y * (aMin.x - bMin.x)) / (-b.x * a.y + a.x * b.y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
	    // Collision detected
	    return glm::vec2(aMin.x + (t * a.x), aMin.y + (t * a.y));
	}

	return glm::vec2();
}

glm::vec2 PhotographSystem::rotateVector(glm::vec2 v, float angle)
{
	return glm::mat2(glm::cos(angle), -glm::sin(angle), glm::sin(angle), glm::cos(angle)) * v;
}

glm::vec2 PhotographSystem::projectPointOnVector(glm::vec2 point, glm::vec2 vec)
{
	glm::vec2 returnVal;
	returnVal.x = glm::dot(point, vec) / dot(vec, vec) * vec.x;
	returnVal.y = glm::dot(point, vec) / dot(vec, vec) * vec.y;
	return returnVal;
}

void PhotographSystem::drawElement(float minPercent, float maxPercent, Face* panel, float widthHeightRatio)
{
	float overlapMin = 0.0f;
	float overlapMax = 0.0f;
	float height = maxPercent - minPercent;

	// cut off OOB pixels on the y axis, get amount of overlap
	if (maxPercent > 100.0f)
	{
		overlapMax = maxPercent - 100.0f;
		panel->setProperty("srcYMin", std::to_string(overlapMax / height));
	}

	if (minPercent < 0.0f)
	{
		overlapMin = -minPercent;
		panel->setProperty("srcYMax", std::to_string(1.0f - (overlapMin / height)));
	}

	float width = height * widthHeightRatio;
	float xPercent = 0.0f;
	if (width > 100.0f)
	{
		// cut off OOB pixels on the x axis
		float overlapWidth = ((width / 100.0f) - 1.0f) / 2;
		panel->setProperty("srcXMin", std::to_string(overlapWidth));
		panel->setProperty("srcXMax", std::to_string(1.0f - overlapWidth));
		width = 100.0f;
	}
	else
	{
		// random x position
		xPercent = rand() % static_cast<int>((100 - width));
	}

	// set dimensions: x, y, w, h
	panel->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(
			xPercent, 								// x
			std::max(100.0f - maxPercent, 0.0f),	// y
			width,									// w
			height - overlapMin - overlapMax));		// h
}

void PhotographSystem::closeUI(const char* buttonName, const char* UIName)
{
	m_pUI->destroyUI(UIName);
}
