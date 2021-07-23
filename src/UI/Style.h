/*
 * Style.h
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

#ifndef UI_STYLE_H_
#define UI_STYLE_H_

#include <glm/glm.hpp>
#include "../Rendering/Texture.h"

class UIRenderer;

class Style
{
public:
	Style() = default;
	Style(UIRenderer* pRenderer, const char* name, float marginLeft, float marginRight,
			float marginUpper, float marginLower);
	~Style() = default;

	void drawPanel(glm::vec4 dimensions, int srcY = 0);
	void drawMenu(glm::vec4 dimensions);
	void drawButton(glm::vec4 dimensions);
private:
	UIRenderer* m_pRenderer;
	Texture2D m_pSpriteSheet;
	float m_marginLeft;
	float m_marginRight;
	float m_marginUpper;
	float m_marginLower;

	void drawCorners(glm::vec4 dimensions);
	void drawHorizontals(glm::vec4 dimensions);
	void drawVerticals(glm::vec4 dimensions);
	void drawFill(glm::vec4 dimensions);
};

#endif /* UI_STYLE_H_ */
