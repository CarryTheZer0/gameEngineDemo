/*
 * Panel.h
 *
 *  Created on: 22 Jul 2021
 *      Author: mchlp
 */

#ifndef UI_PANEL_H_
#define UI_PANEL_H_

#include "Face.h"

class Panel : public Face
{
public:
	Panel() = default;
	Panel(const char* name, Style style, Window* pContext);
	~Panel() = default;

	void draw() override;
};

#endif /* UI_PANEL_H_ */
