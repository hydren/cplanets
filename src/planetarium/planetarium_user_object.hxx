/*
 * planetarium_user_object.hxx
 *
 *  Created on: 28 de set de 2016
 *      Author: carlosfaruolo
 */

//custom data to be carried by each Body2D
struct PlanetariumUserObject
{
	SDL_Color color;
	PlanetariumUserObject() {}
	PlanetariumUserObject(const SDL_Color& color) : color(color) {}
};
