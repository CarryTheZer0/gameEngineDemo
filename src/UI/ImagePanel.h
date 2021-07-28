/*
 * ImagePanel.h
 *
 *  Created on: 25 Jul 2021
 *      Author: mchlp
 */

#ifndef UI_IMAGEPANEL_H_
#define UI_IMAGEPANEL_H_

#include "Face.h"

class ImagePanel : public Face
{
public:
	ImagePanel() = default;
	ImagePanel(const char* name, Style style, Window* pContext);
	~ImagePanel() = default;

	void draw() override;

	void setProperty(std::string name, std::string value) override;
protected:
	std::string m_imageName;
	glm::vec4 m_srcRect;
};

#endif /* UI_IMAGEPANEL_H_ */
