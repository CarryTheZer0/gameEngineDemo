/*
 * textBox.h
 *
 *  Created on: 10 Aug 2021
 *      Author: mchlp
 */

#ifndef UI_TEXTBOX_H_
#define UI_TEXTBOX_H_

#include "Face.h"

class TextBox : public Face
{
public:
	TextBox() = default;
	TextBox(const char* name, Style style, Window* pContext);
	~TextBox() = default;

	void draw() override;

	void setProperty(std::string name, std::string value) override;
private:
	std::string m_text;
	float m_textSize = m_textSize;
};

#endif /* UI_TEXTBOX_H_ */
