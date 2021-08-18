/*
 * TextBox.cpp
 *
 *  Created on: 10 Aug 2021
 *      Author: mchlp
 */


#include "TextBox.h"
#include "Style.h"
#include "Window.h"

TextBox::TextBox(const char* name, Style style, Window* pContext) :
	Face(name, style, pContext),
	m_text(""),
	m_textSize(1.0f)
{}

void TextBox::draw()
{
	m_style.drawText(m_dimensions, m_text.c_str(), m_textSize);

	Face::draw();
}

void TextBox::setProperty(std::string name, std::string value)
{
	Face::setProperty(name, value);

	if (name == "text")
		m_text = value;
	if (name == "textSize")
		m_textSize = stof(value);
}
