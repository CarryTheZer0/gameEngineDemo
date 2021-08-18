/*
 * Style.cpp
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

// Style.cpp

#include "Style.h"
#include "../Rendering/UIRenderer.h"
#include "../Rendering/TextRenderer.h"
#include "../Rendering/ResourceManager.h"

Style::Style(UIRenderer* pRenderer, TextRenderer* pTextRenderer, const char* name, float marginLeft, float marginRight,
		float marginUpper, float marginLower) :
	m_pRenderer(pRenderer),
	m_pTextRenderer(pTextRenderer),
	m_marginLeft(marginLeft),
	m_marginRight(marginRight),
	m_marginUpper(marginUpper),
	m_marginLower(marginLower)
{
	m_pSpriteSheet = ResourceManager::getTexture(name);
}

void Style::drawPanel(glm::vec4 dimensions, int srcY)
{
	drawFill(dimensions);
	drawCorners(dimensions);
	drawHorizontals(dimensions);
	drawVerticals(dimensions);
	m_pRenderer->draw();
}

void Style::drawMenu(glm::vec4 dimensions)
{
	drawPanel(dimensions, 1);
}

void Style::drawButton(glm::vec4 dimensions)
{
	drawPanel(dimensions, 2);
}

void Style::drawCorners(glm::vec4 dimensions)
{
	// top left
	glm::vec4 srcRect = glm::vec4( 0.0f , 0.0f,
			m_marginLeft, m_marginUpper);

	glm::vec2 size = glm::vec2(m_pSpriteSheet.getWidth() * m_marginLeft,
			m_pSpriteSheet.getHeight() * m_marginUpper);

	m_pRenderer->drawSprite(m_pSpriteSheet, srcRect, glm::vec2(
		dimensions.x, dimensions.y), glm::vec2(), size);

	// top right
	srcRect = glm::vec4( 1.0f - m_marginRight , 0.0f,
			1.0f, m_marginUpper);

	size = glm::vec2(m_pSpriteSheet.getWidth() * m_marginRight,
			m_pSpriteSheet.getHeight() * m_marginUpper);

	m_pRenderer->drawSprite(m_pSpriteSheet, srcRect, glm::vec2(
		dimensions.x + dimensions.z - m_pSpriteSheet.getWidth() * m_marginRight, dimensions.y), glm::vec2(), size);

	// bottom left
	srcRect = glm::vec4( 0.0f , 1.0f - m_marginLower,
			m_marginLeft, 1.0f);

	size = glm::vec2(m_pSpriteSheet.getWidth() * m_marginLeft,
			m_pSpriteSheet.getHeight() * m_marginLower);

	m_pRenderer->drawSprite(m_pSpriteSheet, srcRect, glm::vec2(
		dimensions.x, dimensions.y + dimensions.w - m_pSpriteSheet.getHeight() * m_marginLower),
			glm::vec2(), size);

	// bottom right
	srcRect = glm::vec4( 1.0f - m_marginRight , 1.0f - m_marginLower,
			1.0f, 1.0f);

	size = glm::vec2(m_pSpriteSheet.getWidth() * m_marginRight,
			m_pSpriteSheet.getHeight() * m_marginLower);

	m_pRenderer->drawSprite(m_pSpriteSheet, srcRect, glm::vec2(
		dimensions.x + dimensions.z - m_pSpriteSheet.getWidth() * m_marginRight,
		dimensions.y + dimensions.w - m_pSpriteSheet.getHeight() * m_marginLower), glm::vec2(), size);


}

void Style::drawHorizontals(glm::vec4 dimensions)
{
	// draw upper
	glm::vec4 srcRect = glm::vec4( m_marginLeft , 0.0f,
			1.0f - m_marginRight, m_marginUpper);

	glm::vec2 size = glm::vec2(dimensions.z - m_pSpriteSheet.getWidth() * m_marginLeft - m_pSpriteSheet.getWidth() * m_marginRight,
			m_pSpriteSheet.getHeight() * m_marginUpper);

	m_pRenderer->drawSprite(m_pSpriteSheet, srcRect, glm::vec2(
		dimensions.x + m_pSpriteSheet.getWidth() * m_marginLeft, dimensions.y), glm::vec2(), size);

	// draw lower
	srcRect = glm::vec4( m_marginLeft , 1.0f - m_marginLower,
		1.0f - m_marginRight, 1.0f);

	size = glm::vec2(dimensions.z - m_pSpriteSheet.getWidth() * m_marginLeft - m_pSpriteSheet.getWidth() * m_marginRight,
			m_pSpriteSheet.getHeight() * m_marginLower);

	m_pRenderer->drawSprite(m_pSpriteSheet, srcRect, glm::vec2(dimensions.x + m_pSpriteSheet.getWidth() * m_marginLeft,
		dimensions.y + dimensions.w - m_pSpriteSheet.getHeight() * m_marginLower), glm::vec2(), size);
}

void Style::drawVerticals(glm::vec4 dimensions)
{
	// draw left
	glm::vec4 srcRect = glm::vec4( 0.0f , m_marginUpper,
			m_marginLeft, 1.0f - m_marginLower );

	glm::vec2 size = glm::vec2(m_pSpriteSheet.getWidth() * m_marginLeft,
			dimensions.w - m_pSpriteSheet.getHeight() * m_marginUpper - m_pSpriteSheet.getHeight() * m_marginLower);

	m_pRenderer->drawSprite(m_pSpriteSheet, srcRect, glm::vec2(dimensions.x,
			dimensions.y + m_pSpriteSheet.getHeight() * m_marginUpper), glm::vec2(), size);

	// draw right
	srcRect = glm::vec4( 1.0f - m_marginRight , m_marginUpper,
					1.0f, 1.0f - m_marginLower );
	size = glm::vec2(m_pSpriteSheet.getWidth() * m_marginRight,
				dimensions.w - m_pSpriteSheet.getHeight() * m_marginUpper - m_pSpriteSheet.getHeight() * m_marginLower);

	m_pRenderer->drawSprite(m_pSpriteSheet, srcRect,
			glm::vec2(dimensions.x + dimensions.z - m_pSpriteSheet.getWidth() * m_marginRight,
			dimensions.y + m_pSpriteSheet.getHeight() * m_marginUpper), glm::vec2(), size);
}

void Style::drawFill(glm::vec4 dimensions)
{
	glm::vec4 srcRect = glm::vec4( m_marginLeft , m_marginUpper,
			1.0f - m_marginRight, 1.0f - m_marginLower );

	glm::vec2 size = glm::vec2(dimensions.z - m_pSpriteSheet.getWidth() * m_marginLeft - m_pSpriteSheet.getWidth() * m_marginRight,
			dimensions.w - m_pSpriteSheet.getHeight() * m_marginUpper - m_pSpriteSheet.getHeight() * m_marginLower);

	m_pRenderer->drawSprite(m_pSpriteSheet, srcRect, glm::vec2(dimensions.x + m_pSpriteSheet.getWidth() * m_marginLeft,
			dimensions.y + m_pSpriteSheet.getHeight() * m_marginUpper), glm::vec2(), size);
}

void Style::drawImage(glm::vec4 dimensions, glm::vec4 srcRect, const char* fileName)
{
	Texture2D tex = ResourceManager::getTexture(fileName);

	glm::vec2 size = glm::vec2(dimensions.z, dimensions.w);

	m_pRenderer->drawSprite(tex, srcRect, glm::vec2(dimensions.x, dimensions.y),
			glm::vec2(), size);
	m_pRenderer->draw();
}

void Style::drawText(glm::vec4 dimensions, const char* text, float size)
{
	m_pTextRenderer->drawString(text, dimensions.x, dimensions.y, 1.0f);
	m_pTextRenderer->draw();
}
