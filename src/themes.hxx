/*
 * themes.hxx
 *
 *  Created on: 29 de set de 2016
 *      Author: carlosfaruolo
 */

//  ================ THEMES =================

#include "futil/string/actions.hpp"
#include "futil/collection/map_actions.hpp"
#include <map>
using std::map;

struct Theme
{
	Style buttonStyle, toolbarButtonStyle, dialogStyle, scrollStyle, listWinStyle;
	int bgcol;
	private:bool ready;

	public:

	Theme()
	: buttonStyle(0), toolbarButtonStyle(0), dialogStyle(0), scrollStyle(0), listWinStyle(0),
	  bgcol(0xD8D8C0), ready(false)
	{}

	Theme(Style buttonStyle, Style toolbarButtonStyle, Style dialogStyle, Style scrollStyle, Style listWinStyle, int bgCol)
	: buttonStyle(buttonStyle), toolbarButtonStyle(toolbarButtonStyle), dialogStyle(dialogStyle),
	  scrollStyle(scrollStyle), listWinStyle(listWinStyle),
	  bgcol(bgCol), ready(false)
	{}

	const Theme& init() const
	{
		if(not ready)
		{
			Theme& self = const_cast<Theme&>(*this);
			self.bgcol = calc_color(bgcol);
			self.ready = true;
		}
		return *this;
	}

	static map<string, const Theme*> available;
	static void loadAvailableThemes()
	{
		//uses default SDL_widgets colors
		available["classic"] = new Theme();

		// default colors-focused themes
		available["blue"] = new Theme(Style(0, 1), Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0), 			0xBEDEEE);
		available["grey"] = new Theme(Style(0, 1), Style(0, 1), Style(0, 1), Style(1, 0), Style(0, 0xE0E0E0, 2),	0xE0E0E0);
		available["gray"] = available["grey"];
		available["green"] = new Theme(Style(0, 1), Style(0, 2), Style(0, 2), Style(1, 0), Style(0, 0xA8DCA8, 2),	0xA8DCA8);
		available["wheat"] = new Theme(Style(0, 1), Style(0, 3), Style(0, 3), Style(1, 0), Style(0, 0xD8D8C0, 2),	0xD8D8C0);
		available["rose"] = new Theme(Style(0, 1), Style(0, 4), Style(0, 4), Style(1, 0), Style(0, 0xF5C9D0, 2),	0xF5C9D0);

		available["clearlooks"] = new Theme(Style(0, 1), Style(0, 1), Style(0, 0), Style(0, 0), Style(0, 0x86ABD9, 4),	0xF2F1F0);
		available["gtk"] = available["clearlooks"];

		available["redmond"] = new Theme(Style(0, 1), Style(0, 1), Style(0, 1), Style(1, 0), Style(0, 0x000080, 4),	0xBDBDBD);
		available["win-5.1"] = new Theme(Style(0, 1), Style(0, 1), Style(0, 0), Style(0, 0), Style(0, 0x326BC5, 4),	0xECE9D8);
		available["win-6.1"] = new Theme(Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0xB2CCEC, 4),	0xEDEDED);
		available["win-6.0"] = available["win-6.1"];
		available["win-10.0"] = new Theme(Style(0, 1), Style(0, 1), Style(1, 0x52B5FF), Style(1, 0), Style(0, 0x52B5FF, 4),	0xFAFAFA);

		available["asteria"] = new Theme(Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0x39698A, 4),	0xDAE7E8);

		available["default"] = available["asteria"];
	}

	static const Theme* parseString(const string& str)
	{
		if(available.empty()) loadAvailableThemes();
		return coalesce(available, to_lower(trim(str)), available["default"]);
	}
};

map<string, const Theme*> Theme::available;
