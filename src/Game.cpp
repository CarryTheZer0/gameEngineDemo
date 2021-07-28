/*
 * game.cpp
 *
 *  Created on: 24 Mar 2021
 *      Author: mchlp
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Box2d/Box2d.h>

#include "Game.h"
#include "ContactListener.h"
#include "PhotographSystem.h"
#include "Environment.h"
#include "MainMenu.h"

#include "Rendering/SpriteRenderer.h"
#include "Rendering/DebugRenderer.h"
#include "Rendering/UIRenderer.h"
#include "Rendering/Camera.h"
#include "Rendering/ResourceManager.h"

#include "Entities/TestChar.h"
#include "Entities/TestFloor.h"
#include "Entities/Charger.h"
#include "Entities/MushroomMonster.h"

Game::Game() :
		m_running(true),
		m_width(1024),
		m_height(768)
{}

Game::Game(int w, int h, GLFWwindow* pWindow) :
		m_running(true),
		m_width(w),
		m_height(h),
		m_pWindow(pWindow)
{}

void Game::init()
{
    // load shaders
    ResourceManager::loadShader("src/SpriteVertexShader.glsl", "src/SpriteFragmentShader.glsl", nullptr, "sprite");
    ResourceManager::loadShader("src/DebugVertexShader.glsl", "src/DebugFragmentShader.glsl", nullptr, "debug");
    ResourceManager::loadShader("src/UIVertexShader.glsl", "src/UIFragmentShader.glsl", nullptr, "ui");

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_width),
            static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);

    // debug
    ResourceManager::getShader("debug").use();
    ResourceManager::getShader("debug").setMatrix4("projection", projection);

    // sprite
    ResourceManager::getShader("sprite").use().setInteger("images", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);

    // ui
    ResourceManager::getShader("ui").use().setInteger("images", 0);
    ResourceManager::getShader("ui").setMatrix4("projection", projection);

    // set render-specific controls
    Shader debugShader = ResourceManager::getShader("debug");
    m_pDebugRenderer = new DebugRenderer(debugShader);

    Shader shader = ResourceManager::getShader("sprite");
    m_pRenderer = new SpriteRenderer(shader);

    Shader uiShader = ResourceManager::getShader("ui");
    m_pUIRenderer = new UIRenderer(uiShader);

    // load textures
    ResourceManager::loadTexture("Resources/Textures/SpritesheetPlayer.png", true, "run");
    ResourceManager::loadTexture("Resources/Textures/SpritesheetAnimal.png", true, "animal");
    ResourceManager::loadTexture("Resources/Textures/PhotoAnimal.png", true, "animalPhoto");
    ResourceManager::loadTexture("Resources/Textures/spritesheetMushroom.png", true, "mush");
    ResourceManager::loadTexture("Resources/Textures/PhotoMushroom.png", true, "mushPhoto");
    ResourceManager::loadTexture("Resources/Textures/platformsketch.png", true, "platform");
    ResourceManager::loadTexture("Resources/Textures/dplan.png", true, "dplan");
    ResourceManager::loadTexture("Resources/Textures/sketchBackground.png", true, "background");
    ResourceManager::loadTexture("Resources/Textures/UI/UItest.png", true, "UItest");
    ResourceManager::loadTexture("Resources/Textures/UI/ExitButton.png", true, "ExitButton");

    glfwSetWindowUserPointer( m_pWindow, &m_input );

    glfwSetKeyCallback( m_pWindow, []( GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        InputHandler* pInput = static_cast<InputHandler*>( glfwGetWindowUserPointer( window ) );
        pInput->keyCallback( key, scancode, action, mods );
    } );

    glfwSetMouseButtonCallback( m_pWindow, []( GLFWwindow* window, int button, int action, int mods)
    {
        InputHandler* pInput = static_cast<InputHandler*>( glfwGetWindowUserPointer( window ) );
        pInput->mouseCallback( button, action, mods );
    } );

    glfwSetCursorPosCallback( m_pWindow, []( GLFWwindow* window, double x, double y)
    {
        InputHandler* pInput = static_cast<InputHandler*>( glfwGetWindowUserPointer( window ) );
        pInput->mouseMoveCallback( x, y );
    } );

    // TODO maybe not use dynamically allocated scenes
//    GameplayScene* sc1 = new GameplayScene(&m_input, m_pRenderer, m_pDebugRenderer,
//    		m_pUIRenderer, this, &m_sceneManager, "level1");

    MainMenu* sc1 = new MainMenu(&m_input, m_pRenderer, m_pDebugRenderer,
    	m_pUIRenderer, this, &m_sceneManager);

    unsigned int sc1ID = m_sceneManager.addScene(sc1);
    sc1->startScene();
    m_sceneManager.switchScene(sc1ID);
}

int Game::run()
{
	const float dt = 0.016;
	float currentTime = glfwGetTime();
	float accumulator = 0.0f;

	while (m_running)
	{
		glfwPollEvents();
		float newTime = glfwGetTime();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			accumulator -= dt;
			// process

			if (glfwWindowShouldClose(m_pWindow))
			{
				m_running = false;
			}

			m_sceneManager.getCurrentScene()->update(dt);

		}

		// render
		glClearColor(0.18f, 0.38f, 0.37f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float percent = (accumulator / dt);

        m_sceneManager.getCurrentScene()->render(percent);
        m_sceneManager.clean();

		glfwSwapBuffers(m_pWindow);
	}

	return 0;
}

void Game::exitGame()
{
	m_running = false;
}

void Game::setCamera(glm::vec2 c)
{
	m_pCamera->setPos(c);
}
