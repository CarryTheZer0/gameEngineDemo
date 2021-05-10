/*
 * inputHandler.h
 *
 *  Created on: 29 Mar 2021
 *      Author: mchlp
 */

#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include <map>
#include <glm/glm.hpp>

class GLFWwindow;

class InputHandler
{
public:
	InputHandler() = default;
	InputHandler(GLFWwindow* pWindow);

	void keyCallback(int key, int scancode, int action, int mods);
	void mouseCallback(int button, int action, int mods);
	void mouseMoveCallback(double xPos, double yPos);

	void clearKeys();
	void onKeyPressed(int key);
	void onKeyReleased(int key);
	void onMousePressed(int button);
	void onMouseReleased(int button);
	void onMouseMove();

	bool wasKeyPressed(int key);
	bool wasKeyReleased(int key);
	bool isKeyHeld(int key);

	bool wasMousePressed(int button);
	bool wasMouseReleased(int button);
	glm::vec2 getMousePos();
	bool isMouseHeld(int button);
private:
	GLFWwindow* m_pWindow;

	glm::vec2 m_mousePos;

	std::map<int, bool> m_keyPressed;
	std::map<int, bool> m_keyReleased;
	std::map<int, bool> m_keyHeld;

	std::map<int, bool> m_mousePressed;
	std::map<int, bool> m_mouseReleased;
	std::map<int, bool> m_mouseHeld;
};

#endif /* INPUTHANDLER_H_ */
