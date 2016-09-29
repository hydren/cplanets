/*
 * themes.hxx
 *
 *  Created on: 29 de set de 2016
 *      Author: carlosfaruolo
 */

//  ================ THEMES =================

#include "futil/string/actions.hpp"

struct Theme
{
	Theme()
	: buttonStyle(0), dialogStyle(0), bgcol(0xD8D8C0), ready(false)
	{}

	Theme(Style btnSt, Style dlgSt, int bgCol)
	: buttonStyle(btnSt), dialogStyle(dlgSt), bgcol(bgCol), ready(false)
	{}

	Style buttonStyle, dialogStyle;
	int bgcol;

	const Theme& init() const;
	static const Theme& parseString(string str);

	private:bool ready;
};

const Theme THEME_CLASSIC, THEME_DEFAULT,
	THEME_BLUE		(Style(0, 0), Style(0, 0), 0xBEDEEE),
	THEME_GREY		(Style(0, 1), Style(0, 1), 0xE0E0E0),
	THEME_GREEN		(Style(0, 2), Style(0, 2), 0xA8DCA8),
	THEME_WHEAT		(Style(0, 3), Style(0, 3), 0xD8D8C0),
	THEME_ROSE		(Style(0, 4), Style(0, 4), 0xF5C9D0);

const Theme& Theme::init() const
{
	if(not ready)
	{
		Theme& self = const_cast<Theme&>(*this);
		self.bgcol = calc_color(bgcol);
		self.ready = true;
	}
	return *this;
}

const Theme& Theme::parseString(string str)
{
	str = to_lower(trim(str));
	if(str.empty()) return THEME_DEFAULT;

	else if(str == "classic") return THEME_CLASSIC;
	else if(str == "blue") return THEME_BLUE;
	else if(str == "grey" or str == "gray") return THEME_GREY;
	else if(str == "green") return THEME_GREEN;

	else return THEME_DEFAULT;
}

//old code
//void initThemes()
//{
//	theme
//	theme["default"] = Theme();
//	theme["classic"] = Style();
//
//	theme["blue"] = Style(0, 0);
//	theme["grey"] = Style(0, 1);
//	theme["green"] = Style(0, 2);
//	theme["wheat"] = Style(0, 3);
//	theme["rose"] = Style(0, 4);
//
//	theme["clearlooks"] = theme["gtk"] = theme["gtk+"] = Style(0, 1);
//
//	theme["clearlooks"] = Style(0, 1);
//
//	theme["win-4.10"] = theme["win-4.0"] = Style(0, 3);
//	theme["win-4.90"] = Style(0, 1);
//
//	theme["win-5.1"] = Style(0, 1);
//	theme["win-6.0"] = Style(0, 1);
//	theme["win-6.1"] = Style(0, 1);
//
//	theme["win-6.2"] = theme["win-6.3"] = Style(0, 1);
//	theme["win-10.0"] = Style(0, 1);
//}
