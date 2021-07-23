/*
 * Face.cpp
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

// Face.cpp

#include "Face.h"
#include "Window.h"

Face::Face(const char* name, Style style, Window* pContext) :
	m_dimensions( 0, 0, 0, 0 ),
	m_style(style),
	m_name(name),
	m_pContext(pContext)
{}

Face::~Face()
{
	for (Face* f : m_children)
	{
		delete f;
	}
}

void Face::draw()
{
	for (Face* pFace : m_children)
	{
		pFace->draw();
	}
}

void Face::onCursorClick(glm::vec2 pos)
{
	for (Face* pFace : m_children)
	{
		pFace->onCursorClick(pos);
	}
}

void Face::onCursorMoved(glm::vec2 pos)
{
	for (Face* pFace : m_children)
	{
		pFace->onCursorMoved(pos);
	}
}

void Face::addChild(Face* pChild)
{
	m_children.emplace_back(pChild);
	pChild->setParent(this);
	pChild->setRect({ 0, 0, 0, 0 }, { 0, 0, 0, 0 });
}

void Face::setParent(Face* pParent)
{
	m_pParent = pParent;
}

glm::vec4 Face::getRect()
{
	return m_dimensions;
}

void Face::setRect(glm::vec4 pixels, glm::vec4 percentage)
{
	glm::vec4 dimensions = m_pParent->getRect();
	pixels.x += dimensions.x;
	pixels.y += dimensions.y;
	percentage.x *= dimensions.z;
	percentage.y *= dimensions.w;
	percentage.z *= dimensions.z;
	percentage.w *= dimensions.w;

	m_dimensions = { pixels.x + percentage.x / 100, pixels.y + percentage.y / 100, pixels.z + percentage.z / 100, pixels.w + percentage.w / 100 };
}

void Face::setProperty(std::string name, std::string value)
{
	if (name == "xPixels")
		m_dimensions.x += std::stoi(value);
	else if (name == "yPixels")
		m_dimensions.y += std::stoi(value);

	else if (name == "wPixels")
		m_dimensions.z += std::stoi(value);
	else if (name == "hPixels")
		m_dimensions.w += std::stoi(value);

	if (name == "xPercentage")
		m_dimensions.x += m_pParent->getRect().z * std::stof(value) / 100;
	else if (name == "yPercentage")
		m_dimensions.y += m_pParent->getRect().w * std::stof(value) / 100;

	else if (name == "wPercentage")
		m_dimensions.z += m_pParent->getRect().z * std::stof(value) / 100;
	else if (name == "hPercentage")
		m_dimensions.w += m_pParent->getRect().w * std::stof(value) / 100;
}
