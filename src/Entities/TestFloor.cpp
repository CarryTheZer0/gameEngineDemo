/*
 * TestFloor.cpp
 *
 *  Created on: 7 Apr 2021
 *      Author: mchlp
 */

#include "TestFloor.h"

#include "../Rendering/SpriteRenderer.h"
#include "../Rendering/DebugRenderer.h"
#include "../InputHandler.h"

Floor::Floor(SpriteRenderer* pRenderer, glm::vec2 pos, float parallax) :
	Entity(nullptr, nullptr, nullptr),
	m_sprite(this, pRenderer, "platform", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(), 0.8f),
	m_parallax(parallax)
{
	m_transform = pos;
}

void Floor::update(float deltaTime)
{}

void Floor::render(float percent, glm::vec2 camera, float scale)
{
	m_sprite.render(percent, camera - (camera * m_parallax), scale);
}
