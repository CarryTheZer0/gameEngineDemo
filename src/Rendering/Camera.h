/*
 * Camera.h
 *
 *  Created on: 21 May 2021
 *      Author: mchlp
 */

#include <glm/glm.hpp>

#ifndef CAMERA_H_
#define CAMERA_H_

class Camera
{
public:
	Camera() = default;
	Camera(float screenWidth, float screenHeight);
	~Camera() = default;

	void setTargetPos(glm::vec2 pos);
	void setPos(glm::vec2 pos);
	glm::vec2 getPos();
	void setScale(float scale) { m_scale = scale; }
	void setTargetScale(float scale) { m_scaleTarget = scale;}
	float getScale() { return m_scale; }
	void setMin(glm::vec2 min) { m_sceneMin = min; }
	void setMax(glm::vec2 max) { m_sceneMax = max; }

	void update(float deltaTime);
	void clamp();
private:
	float m_screenWidth;
	float m_screenHeight;
	glm::vec2 m_pos;
	glm::vec2 m_posTarget;
	float m_scale;
	float m_scaleTarget;
	glm::vec2 m_sceneMin;
	glm::vec2 m_sceneMax;
};

#endif /* CAMERA_H_ */
