/*
 * RenderQueue.cpp
 *
 *  Created on: 2 Aug 2021
 *      Author: mchlp
 */

#include "RenderQueue.h"

void RenderQueue::sort(std::vector<Entity*> &renderQueue)
{
    for(int i = 0; i < renderQueue.size(); i++)
    {
        Entity* temp = renderQueue[i];
        int j = i - 1;
        while( j >= 0 && temp->getDepth() < renderQueue[j]->getDepth() )
        {
        	renderQueue[j + 1] = renderQueue[j];
            j -= 1;
        }
        renderQueue[j + 1] = temp;
    }
}
