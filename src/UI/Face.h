/*
 * Face.h
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

#ifndef UI_FACE_H_
#define UI_FACE_H_

#include <vector>
#include <string>
#include <functional>

#include "Style.h"

class Window;

class Face
{
public:
	Face(const char* name, Style style, Window* pContext);
	virtual ~Face();

	virtual void draw();
	virtual void onCursorMoved(glm::vec2 pos);
	virtual void onCursorClick(glm::vec2 pos);
	virtual void setProperty(std::string name, std::string value);

	void addChild(Face* pChild);
	glm::vec4 getRect();
	void setRect(glm::vec4 pixels, glm::vec4 percentage);
protected:
	Window* m_pContext = nullptr;
	Face* m_pParent = nullptr;

	std::string m_name;
	glm::vec4 m_dimensions;

	std::vector<Face*> m_children;
	Style m_style;
private:
	void setParent(Face* pParent);
};

#endif /* UI_FACE_H_ */
