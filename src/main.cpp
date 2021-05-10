/*
 * GL01Hello.cpp: Test OpenGL C/C++ Setup
 */

#include <stdio.h>
#include <stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include "Game.h"
#include "InputHandler.h"

int main()
{
	glewExperimental = true;
	if (!glfwInit() )
	{
		fprintf( stderr, "Failed to initialise GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

	GLFWwindow* window;
	window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Game myGame(1024, 768, window);
	myGame.init();
	myGame.run();

	glfwTerminate();
}
