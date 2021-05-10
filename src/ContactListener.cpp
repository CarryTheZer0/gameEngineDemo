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

void ContactListener::BeginContact(b2Contact* contact)
{
	Sensor* a = reinterpret_cast<Sensor*>(contact->GetFixtureA()->GetUserData().pointer);
	Sensor* b = reinterpret_cast<Sensor*>(contact->GetFixtureB()->GetUserData().pointer);
	if (a)
	{
		a->invokeBegin();
	}

	if (b)
	{
		b->invokeBegin();
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	Sensor* a = reinterpret_cast<Sensor*>(contact->GetFixtureA()->GetUserData().pointer);
	Sensor* b = reinterpret_cast<Sensor*>(contact->GetFixtureB()->GetUserData().pointer);
	if (a)
	{
		a->invokeEnd();
	}

	if (b)
	{
		b->invokeEnd();
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
