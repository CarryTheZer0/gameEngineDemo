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
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "DebugRenderer.h"
#include "ContactListener.h"
#include "PhotographSystem.h"

#include "Entities/TestChar.h"
#include "Entities/TestFloor.h"
#include "Entities/TestAnimal.h"

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

    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_width),
            static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);

    // debug
    ResourceManager::getShader("debug").use();
    ResourceManager::getShader("debug").setMatrix4("projection", projection);

    // sprite
    ResourceManager::getShader("sprite").use().setInteger("images", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);

    // set render-specific controls
    Shader debugShader = ResourceManager::getShader("debug");
    m_pDebugRenderer = new DebugRenderer(debugShader);

    Shader shader = ResourceManager::getShader("sprite");
    m_pRenderer = new SpriteRenderer(shader);

    // load textures
    ResourceManager::loadTexture("textures/Spritesheet.png", true, "run");
    ResourceManager::loadTexture("textures/SpritesheetAnimal.png", true, "animal");
    ResourceManager::loadTexture("textures/platformsketch.png", true, "dplan");
    ResourceManager::loadTexture("textures/sketchBackground.png", true, "background");

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
}

int Game::run()
{
	const float dt = 0.016;
	float currentTime = glfwGetTime();
	float accumulator = 0.0f;

	b2Vec2 gravity(0.0f, 10.0f);
	b2World world(gravity);
	ContactListener contacts;
	world.SetContactListener(&contacts);

	PhotographSystem photo;

	Floor floorTest = Floor(m_pRenderer, m_pDebugRenderer);
	floorTest.init(&world, glm::vec2(0.0f, 5.0f), m_pDebugRenderer);
	m_entities.push_back(&floorTest);

	Floor floorTest2 = Floor(m_pRenderer, m_pDebugRenderer);
	floorTest2.init(&world, glm::vec2(9.0f, 6.0f), m_pDebugRenderer);
	m_entities.push_back(&floorTest2);

	Floor floorTest3 = Floor(m_pRenderer, m_pDebugRenderer);
	floorTest3.init(&world, glm::vec2(18.0f, 4.5f), m_pDebugRenderer);
	m_entities.push_back(&floorTest3);

	Floor floorTest4 = Floor(m_pRenderer, m_pDebugRenderer);
	floorTest4.init(&world, glm::vec2(27.0f, 3.5f), m_pDebugRenderer);
	m_entities.push_back(&floorTest4);

	Player playerTest = Player(this, m_pRenderer, m_pDebugRenderer, &m_input, &photo);
	playerTest.init(&world, glm::vec2(0.0f, -1.0f), m_pDebugRenderer);
	m_entities.push_back(&playerTest);

	Animal animalTest = Animal(m_pRenderer, m_pDebugRenderer, &playerTest);
	animalTest.init(&world, glm::vec2(1.0f, -2.0f), m_pDebugRenderer);
	m_entities.push_back(&animalTest);

	photo.addEntity(&animalTest);

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

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

		    world.Step(dt, velocityIterations, positionIterations);
			m_pRenderer->setShadows(glm::vec2(), glm::vec2());
		    for (Entity* e : m_entities)
		    {
		    	e->update(dt);
		    }

			m_input.clearKeys();
		}

		// render
		glClearColor(0.18f, 0.38f, 0.37f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float percent = (accumulator / dt);
                glm::vec2 position = playerTest.getPos();
        m_cameraPos = glm::vec2(-position.x + m_width / 2, -position.y + m_height / 2);

        Texture2D texBack = ResourceManager::getTexture("background");
        m_pRenderer->drawSprite(texBack, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
        		-m_cameraPos - glm::vec2(0.0f, 200.0f), glm::vec2(), glm::vec2(texBack.getWidth() * 0.7, texBack.getHeight() * 0.7));

	    for (Entity* e : m_entities)
	    {
	    	e->render(percent, m_cameraPos);
	    }
		m_pRenderer->draw();
		m_pDebugRenderer->draw();

		glfwSwapBuffers(m_pWindow);
	}

	return 0;
}

void Game::exitGame()
{
	m_running = false;
}