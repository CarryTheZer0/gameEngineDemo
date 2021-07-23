/*
 * inputHandler.cpp
 *
 *  Created on: 29 Mar 2021
 *      Author: mchlp
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InputHandler.h"

InputHandler::InputHandler(GLFWwindow* pWindow) :
	m_pWindow(pWindow)
{}

void InputHandler::keyCallback(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        onKeyPressed(key);
    else if (action == GLFW_RELEASE)
    	onKeyReleased(key);
}

void InputHandler::mouseCallback(int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        onMousePressed(button);
    else if (action == GLFW_RELEASE)
    	onMouseReleased(button);
}

void InputHandler::mouseMoveCallback(double xPos, double yPos)
{
	m_mousePos.x = xPos;
	m_mousePos.y = yPos;
	onMouseMove();
}

void InputHandler::clearKeys()
{
	m_keyPressed.clear();
	m_keyReleased.clear();
	m_mousePressed.clear();
	m_mouseReleased.clear();
	m_mouseMoved = false;
}

void InputHandler::onKeyPressed(int key)
{
	m_keyPressed[key] = true;
	m_keyHeld[key] = true;
}

void InputHandler::onKeyReleased(int key)
{
	m_keyReleased[key] = true;
	m_keyHeld[key] = false;
}

void InputHandler::onMousePressed(int button)
{
	m_mousePressed[button] = true;
	m_mouseHeld[button] = true;
}

void InputHandler::onMouseReleased(int button)
{
	m_mouseReleased[button] = true;
	m_mouseHeld[button] = false;
}

void InputHandler::onMouseMove()
{
	m_mouseMoved = true;
}

bool InputHandler::wasKeyPressed(int key)
{
	return m_keyPressed[key];
}

bool InputHandler::wasKeyReleased(int key)
{
	return m_keyReleased[key];
}

bool InputHandler::isKeyHeld(int key)
{
	return m_keyHeld[key];
}

bool InputHandler::wasMousePressed(int button)
{
	return m_mousePressed[button];
}

bool InputHandler::wasMouseReleased(int button)
{
	return m_mouseReleased[button];
}

bool InputHandler::wasMouseMoved()
{
	return m_mouseMoved;
}

bool InputHandler::isMouseHeld(int button)
{
	return m_mouseHeld[button];
}

glm::vec2 InputHandler::getMousePos()
{
	return m_mousePos;
}
