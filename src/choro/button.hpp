/*
 * button.hpp
 *
 *  Created on: 28 de jan de 2016
 *      Author: carlosfaruolo
 */

#ifndef CHORO_BUTTON_HPP_
#define CHORO_BUTTON_HPP_

#include <string>
#include <list>

#include <SDL/SDL.h>

#include "action_listener.hpp"

class CButton
{
	std::string text;
	SDL_Surface* icon;

	std::list<CActionListener*> actionListeners;

public:
	CButton(std::string text, SDL_Surface* icon);

	void addActionListener(CActionListener* listener);
};



#endif /* CHORO_BUTTON_HPP_ */
