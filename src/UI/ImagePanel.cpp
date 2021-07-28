/*
 * ImagePanel.cpp
 *
 *  Created on: 25 Jul 2021
 *      Author: mchlp
 */

#include <functional>

#include "ImagePanel.h"
#include "Style.h"
#include "Window.h"

ImagePanel::ImagePanel(const char* name, Style style, Window* pContext) :
	Face(name, style, pContext),
	m_imageName(""),
	m_srcRect({ 0.0f, 0.0f, 1.0f, 1.0f })
{}

void ImagePanel::draw()
{
	m_style.drawImage(m_dimensions, m_srcRect, m_imageName.c_str());

	Face::draw();
}

void ImagePanel::setProperty(std::string name, std::string value)
{
	Face::setProperty(name, value);

	if (name == "imageName")
		m_imageName = value;
	else if (name == "srcXMin")
		m_srcRect.x = stof(value);
	else if (name == "srcYMin")
		m_srcRect.y = stof(value);
	else if (name == "srcXMax")
		m_srcRect.z = stof(value);
	else if (name == "srcYMax")
		m_srcRect.w = stof(value);
}

