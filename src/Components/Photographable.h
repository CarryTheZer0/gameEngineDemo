/*
 * Photographable.h
 *
 *  Created on: 26 Jul 2021
 *      Author: mchlp
 */

#ifndef COMPONENTS_PHOTOGRAPHABLE_H_
#define COMPONENTS_PHOTOGRAPHABLE_H_

#include "../Component.h"

class Entity;

class Photographable : public Component
{
public:
	Photographable() = default;
	Photographable(Entity* pOwner, const char* nameId);
	~Photographable() = default;

	const char* getNameId() { return m_nameId; }
	float getWidthRatio() { return m_widthRatio; }
private:
	const char* m_nameId;
	float m_widthRatio;
};


#endif /* COMPONENTS_PHOTOGRAPHABLE_H_ */
