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
class DebugRenderer;
class Entity;
class Camera;

class Game
{
public:
	Game();
	Game(int width, int height, GLFWwindow* pWindow);
	~Game() = default;

	void init();

	int run();
	void exitGame();

	Camera* getCamera() { return m_pCamera; }
	void setCamera(glm::vec2 c);
private:
	SpriteRenderer* m_pRenderer;
	DebugRenderer* m_pDebugRenderer;

	bool m_running;
	int m_width;
	int m_height;
	GLFWwindow* m_pWindow;
	InputHandler m_input;
	Camera* m_pCamera;

	std::vector<Entity*> m_entities;
};
