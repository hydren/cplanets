/*
 * list_win.hpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_LIST_WIN_HPP_
#define WIDGETS_LIST_WIN_HPP_

#include "SDL_widgets/SDL_widgets.h"
#include "list_models.hpp"

namespace WidgetsExtra
{
	/// An improved alternative of TextWin.
	struct ListWin extends BgrWin
	{
		UIListModel* model;
		std::vector<bool> selection;

		Point padding;
		unsigned spacing;
		RenderText* textRenderer;

		//can be called only after a TopWin has been constructed
		ListWin(WinBase *parent, Style, Rect rect, Id id=0);

		virtual ~ListWin();

		virtual void draw();

		void setListData(const std::vector<std::string>& data);

		template<typename StringCastableType>
		void setListData(const std::vector<StringCastableType>& data)
		{
			model = new StringableTypeUIListModel<StringCastableType>(data);
		}

		template<typename StringableType>
		void setListData(const std::vector<StringableType>& data, std::string (*stringfyFunction)(StringableType value))
		{
			model = new StringableTypeUIListModel<StringableType>(data, stringfyFunction);
		}

		private:
		//compatibility function (with BgrWin pointers)
		static void drawBgrWinAsListWin(BgrWin* bgrWin);
	};
}

#endif /* WIDGETS_LIST_WIN_HPP_ */
