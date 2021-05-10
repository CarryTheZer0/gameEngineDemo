/*
 * Component.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mchlp
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

class Entity;

class Component
{
public:
	Component() = default;
	Component(Entity* owner) : m_pOwner(owner) {}

	virtual Entity* getOwner() { return m_pOwner; }
protected:
	Entity* m_pOwner;
};

#endif /* COMPONENT_H_ */
