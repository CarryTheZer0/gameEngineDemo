/*
 * Button.cpp
 *
 *  Created on: 23 Jul 2021
 *      Author: mchlp
 */

#include <functional>

#include "Button.h"
#include "Style.h"
#include "Window.h"

Button::Button(const char* name, Style style, Window* pContext) :
	Panel(name, style, pContext),
	m_selected(false)
{}

void Button::draw()
{
	if (!m_selected)
	{
		m_style.drawButton(m_dimensions);
	}

	Face::draw();
}

void Button::onCursorMoved(glm::vec2 pos)
{
	m_selected = isInBounds(pos);
}

void Button::onCursorClick(glm::vec2 pos)
{
	if (m_selected)
	{
		m_pContext->callEvent(m_eventName.c_str());
	}
}

bool Button::isInBounds(glm::vec2 pos)
{
	if (pos.x > m_dimensions.x && pos.y > m_dimensions.y &&
		pos.x < m_dimensions.x + m_dimensions.z && pos.y < m_dimensions.y + m_dimensions.w)
		return true;
	else
		return false;
}

void Button::setProperty(std::string name, std::string value)
{
	Face::setProperty(name, value);

	if (name == "eventName")
		m_eventName = value;
}
