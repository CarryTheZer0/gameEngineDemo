/*
 * Sensor.cpp
 *
 *  Created on: 15 Apr 2021
 *      Author: mchlp
 */

#include "Sensor.h"
#include "../Entity.h"

void dummy() {}

Sensor::Sensor(Entity* pOwner, b2Body* pBody, b2FixtureDef def) :
	Fixture(pOwner, pBody, def)
{
	m_f_invokeBegin = dummy;
	m_f_invokeEnd = dummy;
}

void Sensor::initBegin(std::function<void()> func)
{
	m_f_invokeBegin = func;
}

void Sensor::initEnd(std::function<void()> func)
{
	m_f_invokeEnd = func;
}

void Sensor::invokeBegin()
{
	m_f_invokeBegin();
}

void Sensor::invokeEnd()
{
	m_f_invokeEnd();
}
