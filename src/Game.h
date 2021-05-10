/*
 * game.h
 *
 *  Created on: 24 Mar 2021
 *      Author: mchlp
 */

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InputHandler.h"

class SpriteRenderer;
class Entity;

class Game
{
public:
	Game();
	Game(int width, int height, GLFWwindow* pWindow);
	~Game() = default;

	void init();

	int run();
	void exitGame();

	glm::vec2 getCamera() { return m_cameraPos; }
	void setCamera(glm::vec2 c) { m_cameraPos = c; }
private:
	SpriteRenderer* m_pRenderer;

	bool m_running;
	int m_width;
	int m_height;
	GLFWwindow* m_pWindow;
	InputHandler m_input;
	glm::vec2 m_cameraPos;

	std::vector<Entity*> m_entities;
};
