/*
 * Window.h
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

#ifndef UI_WINDOW_H_
#define UI_WINDOW_H_

#include <unordered_map>
#include <string>

#include "Face.h"

class Window : public Face
{
public:
	Window() = default;
	Window(const char* name, Style style, int width, int height);
	~Window() = default;

	void callEvent(const char*);
	void addEvent(const char*, std::function<void(const char*)>);

	void addAnchor(const char* name, Face* face);
	Face* getAnchor(const char* name) { return m_anchorPoints[name]; }
private:
	std::unordered_map<std::string, std::function<void(const char*)>> m_eventCalls;
	std::unordered_map<std::string, Face*> m_anchorPoints;
};


#endif /* UI_WINDOW_H_ */
