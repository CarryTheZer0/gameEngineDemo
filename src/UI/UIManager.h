/*
 * UIManager.h
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

#ifndef UI_UIMANAGER_H_
#define UI_UIMANAGER_H_

#include <vector>
#include <functional>
#include <unordered_map>
#include <string>

#include "../tinyxml2.h"

#include "Style.h"
#include "../InputHandler.h"
#include "Face.h"
#include "UIFactory.h"

class UIManager
{
public:
	UIManager() = default;
	UIManager(InputHandler* pInput);
	~UIManager() = default;

	void update();
	void render();
	void addRoot(const char* name, Face* f);
	void addStyle(const char* name, Style s);
	Style getStyle(const char* name);
	Window* loadUI(const char* filepath, const char* name);
	void destroyUI(const char* name);
private:
	InputHandler* m_pInput;
	// TODO Memeory leaks here!!
	std::unordered_map<std::string, Face*> m_roots;
	std::unordered_map<std::string, Style> m_styles;
	std::unordered_map<std::string, IUIFactory*> m_factories;

	std::vector<std::string> m_toDestroy;

	void createFactories();
	void loadElement(tinyxml2::XMLElement* pFace, Face* pParent, Window* pContext);
};



#endif /* UI_UIMANAGER_H_ */
