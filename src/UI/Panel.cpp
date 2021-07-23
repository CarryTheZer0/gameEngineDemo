/*
 * Panel.cpp
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

#include "Panel.h"

Panel::Panel(const char* name, Style style, Window* pContext) :
	Face(name, style, pContext)
{}

void Panel::draw()
{
	m_style.drawPanel(m_dimensions);

	Face::draw();
}
