/*
 * Sensor.h
 *
 *  Created on: 15 Apr 2021
 *      Author: mchlp
 */

#ifndef COMPONENTS_SENSOR_H_
#define COMPONENTS_SENSOR_H_

#include <functional>

#include "Fixture.h"

class Entity;
class DebugRenderer;

class Sensor : public Fixture
{
public:
	Sensor() = default;
	Sensor(Entity* pOwner, b2Body* pBody, DebugRenderer* pDebug, float xExtent, float yExtent,
			b2Vec2 offset, Sensor* pointer);
	~Sensor() = default;

	void initBegin(std::function<void()> func);
	void initEnd(std::function<void()> func);
	void invokeBegin();
	void invokeEnd();

	void render(float percent, glm::vec2 camera);
private:
	DebugRenderer* m_pDebug;
	float m_xExtent;
	float m_yExtent;
	b2Vec2 m_offset;

	std::function<void()> m_f_invokeBegin;
	std::function<void()> m_f_invokeEnd;
};

#endif /* COMPONENTS_SENSOR_H_ */