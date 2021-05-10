/*
 * ContactListener.h
 *
 *  Created on: 13 Apr 2021
 *      Author: mchlp
 */

#include <box2d/box2d.h>

#ifndef CONTACTLISTENER_H_
#define CONTACTLISTENER_H_

class ContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif /* CONTACTLISTENER_H_ */
