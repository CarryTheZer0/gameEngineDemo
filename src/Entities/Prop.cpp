/*
 * TestFloor.cpp
 *
 *  Created on: 7 Apr 2021
 *      Author: mchlp
 */

#include "../Rendering/SpriteRenderer.h"
#include "../Rendering/DebugRenderer.h"
#include "../InputHandler.h"
#include "Prop.h"

Prop::Prop(SpriteRenderer* pRenderer, glm::vec2 pos, const char* name, float parallax) :
	Entity(nullptr, nullptr, nullptr),
	m_sprite(this, pRenderer, name, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(), 0.8f),
	m_parallax(parallax)
{
	m_transform = pos;
}

void Prop::update(float deltaTime)
{}

void Prop::render(float percent, glm::vec2 camera, float scale)
{
	m_sprite.render(percent, camera, scale);
}
