/*
 * UIFactory.h
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

#ifndef UI_UIFACTORY_H_
#define UI_UIFACTORY_H_

#include "Face.h"
#include "Window.h"
#include "Style.h"

class IUIFactory
{
public:
	virtual Face* MakeFace(const char* name, Style style, Window* pContext) = 0;
};

template <class UIElement>
class UIFactory : public IUIFactory
{
public:
	Face* MakeFace(const char* name, Style style, Window* pContext) override final { return new UIElement(name, style, pContext); }
};

#endif /* UI_UIFACTORY_H_ */
