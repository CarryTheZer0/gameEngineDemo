/*
 * UIManager.cpp
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>

#include "UIManager.h"
#include "../tinyxml2.h"

#include "Window.h"
#include "Panel.h"
#include "Button.h"
#include "ImagePanel.h"

UIManager::UIManager(InputHandler* pInput)
{
	m_pInput = pInput;
	createFactories();
}

void UIManager::update()
{
	for (auto& pair : m_roots)
	{
		if (m_pInput->wasMouseMoved())
		{
			pair.second->onCursorMoved(m_pInput->getMousePos());
		}

		if (m_pInput->wasMousePressed(GLFW_MOUSE_BUTTON_1))
		{
			pair.second->onCursorClick(m_pInput->getMousePos());
		}
	}

	for (std::string name : m_toDestroy)
	{
		delete m_roots[name];
		m_roots.erase(name);
	}

	m_toDestroy.clear();
}

void UIManager::render()
{
	for (auto& pair : m_roots)
	{
		pair.second->draw();
	}
}

void UIManager::addRoot(const char* name, Face* f)
{
	m_roots.insert({ name, f });
}

void UIManager::addStyle(const char* name, Style s)
{
	m_styles.insert({name, s});
}

Style UIManager::getStyle(const char* name)
{
	return m_styles[name];
}

Window* UIManager::loadUI(const char* filepath, const char* name)
{
	Window* newRoot = new Window(name, m_styles["myStyle1"], 1024, 768);

	tinyxml2::XMLDocument doc;
	assert(doc.LoadFile(filepath) == 0 && "No xml specification found!");

	tinyxml2::XMLElement* pUI = doc.FirstChildElement("UI");
	tinyxml2::XMLElement* pFace = pUI->FirstChildElement("Face");

	// Load UI Elements
	while (pFace)
	{
		loadElement(pFace, static_cast<Face*>(newRoot), newRoot);
		pFace = pFace->NextSiblingElement("Face");
	}

	addRoot(name, static_cast<Face*>(newRoot));

	return newRoot;
}

void UIManager::destroyUI(const char* name)
{
	m_toDestroy.emplace_back(name);
}

void UIManager::createFactories()
{
	m_factories.insert({ "Face", new UIFactory<Face> });
	m_factories.insert({ "Panel", new UIFactory<Panel> });
	m_factories.insert({ "Button", new UIFactory<Button> });
	m_factories.insert({ "ImagePanel", new UIFactory<ImagePanel> });
}

void UIManager::loadElement(tinyxml2::XMLElement* pFace, Face* pParent, Window* pContext)
{
	const char* type;
	const char* name;
	const char* style;

	pFace->QueryStringAttribute("Type", &type);
	pFace->QueryStringAttribute("Name", &name);
	pFace->QueryStringAttribute("Style", &style);

	Face* newFace = m_factories[type]->MakeFace(name, m_styles[style], pContext);
	pParent->addChild(newFace);


	// Load child properties
	tinyxml2::XMLElement* pProperty = pFace->FirstChildElement("Property");
	while (pProperty)
	{
		const char* propertyName;
		const char* propertyValue;

		pProperty->QueryStringAttribute("Name", &propertyName);
		pProperty->QueryStringAttribute("Value", &propertyValue);

		newFace->setProperty(propertyName, propertyValue);
		pProperty = pProperty->NextSiblingElement("Property");
	}

	// Load anchor points
	tinyxml2::XMLElement* pAnchor = pFace->FirstChildElement("Anchor");
	while (pAnchor)
	{
		const char* anchorName;

		pAnchor->QueryStringAttribute("Name", &anchorName);

		pContext->addAnchor(anchorName, newFace);
		pAnchor = pAnchor->NextSiblingElement("Anchor");
	}

	// Load child UI elements
	tinyxml2::XMLElement* pChild = pFace->FirstChildElement("Face");
	while (pChild)
	{
		loadElement(pChild, newFace, pContext);
		pChild = pChild->NextSiblingElement("Face");
	}
}

Face* UIManager::addElement(Face* pParent, Window* pContext, const char* name, const char* type,
		const char* style)
{
	Face* newFace = m_factories[type]->MakeFace(name, m_styles[style], pContext);
	pParent->addChild(newFace);

	return newFace;
}
