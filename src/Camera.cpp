/*
 * Camera.cpp
 *
 *  Created on: 21 May 2021
 *      Author: mchlp
 */

#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight),
	m_scale(1.0f),
	m_scaleTarget(1.0f),
	m_sceneMin(0.0f, 0.0f),
	m_sceneMax(screenWidth, screenHeight)
{}

void Camera::setTargetPos(glm::vec2 pos)
{
	m_posTarget = pos * m_scale;
}

void Camera::setPos(glm::vec2 pos)
{
	m_pos = pos * m_scale;
}

glm::vec2 Camera::getPos()
{
	glm::vec2 pos = m_pos;
	pos.x -= m_screenWidth * 0.5;
	pos.y -= m_screenHeight * 0.5;
	return pos;
}

void Camera::update(float deltaTime)
{
	glm::vec2 temp = (m_posTarget - m_pos);
	temp.x *= 0.05;
	temp.y *= 0.05;
	m_pos += temp;

	float newScale = m_scale + (m_scaleTarget - m_scale) * 0.05f;

	m_pos *= newScale/m_scale;
	m_scale = newScale;
}

void Camera::clamp()
{
	if (m_pos.x - m_screenWidth / 2 < m_sceneMin.x) m_pos.x = m_sceneMin.x + m_screenWidth / 2;
	if (m_pos.y - m_screenHeight / 2 < m_sceneMin.y) m_pos.y = m_sceneMin.y + m_screenHeight / 2;
	if (m_pos.x + m_screenWidth / 2 > m_sceneMax.x) m_pos.x = m_sceneMax.x - m_screenWidth / 2;
	if (m_pos.y + m_screenHeight / 2 > m_sceneMax.y) m_pos.y = m_sceneMax.y - m_screenHeight / 2;
}
