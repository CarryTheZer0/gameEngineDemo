/*
 * RenderQueue.h
 *
 *  Created on: 2 Aug 2021
 *      Author: mchlp
 */

#ifndef RENDERING_RENDERQUEUE_H_
#define RENDERING_RENDERQUEUE_H_

#include <vector>

#include "../Entity.h"

class RenderQueue
{
public:
	static void sort(std::vector<Entity*> &renderQueue);
private:
	// private constructor - class uses static methods only
	RenderQueue() { }
};



#endif /* RENDERING_RENDERQUEUE_H_ */
