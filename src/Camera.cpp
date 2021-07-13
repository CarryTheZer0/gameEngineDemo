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
	m_scaleTarget(1.0f)
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
