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
#include <utility>

using std::pair;
using std::map;

#define create_theme(name, theme) available.insert(std::make_pair((name), (theme)))

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

	static map<string, const Theme> available;

	private:
	static void loadAvailableThemes()
	{
		//                     buttonStyle, toolbarBtnStyle, dialogStyle, scrollStyle, listWinStyle, bgCol

		//uses default SDL_widgets colors
		create_theme("classic", Theme());

		// default colors-focused themes
		create_theme("blue",  Theme(Style(0, 1), Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0),           0xBEDEEE));
		create_theme("grey",  Theme(Style(0, 1), Style(0, 1), Style(0, 1), Style(1, 0), Style(0, 0xE0E0E0, 2), 0xE0E0E0));
		create_theme("gray", available["grey"]);
		create_theme("green", Theme(Style(0, 1), Style(0, 2), Style(0, 2), Style(1, 0), Style(0, 0xA8DCA8, 2), 0xA8DCA8));
		create_theme("wheat", Theme(Style(0, 1), Style(0, 3), Style(0, 3), Style(1, 0), Style(0, 0xD8D8C0, 2), 0xD8D8C0));
		create_theme("rose",  Theme(Style(0, 1), Style(0, 4), Style(0, 4), Style(1, 0), Style(0, 0xF5C9D0, 2), 0xF5C9D0));

		// linux themes
		create_theme("clearlooks", Theme(Style(0, 1), Style(0, 1), Style(0, 0), Style(0, 0), Style(0, 0x86ABD9, 4),	0xF2F1F0));
		create_theme("gtk", available["clearlooks"]);

		// cplanets themes
		create_theme("asteria", Theme(Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0x39698A, 4), 0xDAE7E8));

		// others
		create_theme("osx-10.7", Theme(Style(0, 1), Style(0, 1), Style(0, 1), Style(0, 0), Style(0, 0x3497F0, 4), 0xF6F6F6));
		create_theme("osx", available["osx-10.7"]);

		create_theme("redmond", Theme(Style(0, 1), Style(0, 1), Style(0, 6), Style(1, 0), Style(0, 0x000080, 4), 0xBDBDBD));
		create_theme("win9x", available["redmond"]);
		create_theme("win-5.1", Theme(Style(0, 1), Style(0, 1), Style(0, 5), Style(0, 0), Style(0, 0x326BC5, 4), 0xECE9D8));
		create_theme("win-5.0", available["win-5.1"]);
		create_theme("win-6.1", Theme(Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0), Style(0, 0x3399FF, 4), 0xF1EDED));
		create_theme("win-6.0", available["win-6.1"]);
		create_theme("win-10.0", Theme(Style(0, 1), Style(0, 1), Style(1, 0x52B5FF), Style(1, 0), Style(0, 0x52B5FF, 4), 0xFAFAFA));
		create_theme("win-6.2", available["win-10.0"]);
		create_theme("win-6.3", available["win-10.0"]);

		create_theme("default", available["asteria"]);
	}

	public:
	static const Theme& getThemeByName(const string& str)
	{
		if(available.empty()) loadAvailableThemes();
		if(not str.empty())
		{
			map<string, const Theme>::iterator result = available.lower_bound(str);
			if(result != available.end())
				return result->second;
		}

		return available["default"];
	}
};

map<string, const Theme> Theme::available;
