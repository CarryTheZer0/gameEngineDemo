/*
 * Button.h
 *
 *  Created on: 23 Jul 2021
 *      Author: mchlp
 */

#ifndef UI_BUTTON_H_
#define UI_BUTTON_H_

#include "Panel.h"

class Button : public Panel
{
public:
	Button() = default;
	Button(const char* name, Style style, Window* pContext);
	~Button() = default;

	void draw() override;

	void onCursorMoved(glm::vec2 pos) override;
	void onCursorClick(glm::vec2 pos) override;
	void setProperty(std::string name, std::string value) override;
protected:
	bool m_selected;
	std::string m_eventName;
private:
	bool isInBounds(glm::vec2 pos);
};

#endif /* UI_BUTTON_H_ */
