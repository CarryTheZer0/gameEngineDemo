/*
 * ContactListener.cpp
 *
 *  Created on: 13 Apr 2021
 *      Author: mchlp
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ContactListener.h"
#include "Components/Sensor.h"

#include "Entities/TestChar.h"

void ContactListener::BeginContact(b2Contact* contact)
{
	Fixture* a = reinterpret_cast<Fixture*>(contact->GetFixtureA()->GetUserData().pointer);
	Fixture* b = reinterpret_cast<Fixture*>(contact->GetFixtureB()->GetUserData().pointer);

	if (a && a->getFixture()->IsSensor())
	{
		Sensor* sensorA = dynamic_cast<Sensor*>(a);
		sensorA->invokeBegin(b);
	}

	if (b && b->getFixture()->IsSensor())
	{
		Sensor* sensorB = dynamic_cast<Sensor*>(b);
		sensorB->invokeBegin(a);
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	Fixture* a = reinterpret_cast<Fixture*>(contact->GetFixtureA()->GetUserData().pointer);
	Fixture* b = reinterpret_cast<Fixture*>(contact->GetFixtureB()->GetUserData().pointer);

	if (a && a->getFixture()->IsSensor())
	{
		Sensor* sensorA = dynamic_cast<Sensor*>(a);
		sensorA->invokeEnd(b);
	}

	if (b && b->getFixture()->IsSensor())
	{
		Sensor* sensorB = dynamic_cast<Sensor*>(b);
		sensorB->invokeEnd(a);
	}
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	// process
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	// process
}
