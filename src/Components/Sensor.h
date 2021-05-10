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

class Sensor : public Fixture
{
public:
	Sensor() = default;
	Sensor(Entity* pOwner, b2Body* pBody, b2FixtureDef def);
	~Sensor() = default;

	void initBegin(std::function<void()> func);
	void initEnd(std::function<void()> func);
	void invokeBegin();
	void invokeEnd();
private:
	std::function<void()> m_f_invokeBegin;
	std::function<void()> m_f_invokeEnd;
};

#endif /* COMPONENTS_SENSOR_H_ */
