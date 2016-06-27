/*
 * list_win.hpp
 *
 *  Created on: 14 de jun de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_LIST_WIN_HPP_
#define WIDGETS_LIST_WIN_HPP_

#include "SDL_widgets/SDL_widgets.h"
#include "list_model.hpp"
#include "list_selection_model.hpp"

namespace WidgetsExtra
{
	/// An improved alternative of TextWin.
	struct ListWin extends BgrWin
	{
		UIListModel* model;
		ListSelectionModel selection;

		Point padding;
		unsigned spacing;
		RenderText* textRenderer, *textRendererCaseSelected;
		Uint32 bgcolCaseSelected;

		/// (Advanced) Flag used to tweak the behavior of clicking on the list.
		bool preventRedrawOnClick;

		//can be called only after a TopWin has been constructed
		ListWin(WinBase *parent, Style, Rect rect, Id id=0);

		virtual ~ListWin();

		virtual void draw();

		void setListData(const std::vector<std::string>& data, bool redrawImmediately=false);
		void setListModel(UIListModel* model, bool redrawImmediately=false, bool deletePrevious=true);
		void updateListData(void* data, bool redrawImmediately=false);
		virtual unsigned getListHeight();
		virtual void clickList(const Point& point);

		protected:
		virtual void onMouseDown(Point point, int buttonNumber);
		virtual void onMouseMove(Point point, int buttonNumber); // by default does nothing
		virtual void onMouseUp(Point point, int buttonNumber); // by default does nothing

		private:
		//compatibility function (with BgrWin pointers)
		static void drawBgrWinAsListWin(BgrWin* bgrWin);
		static void onMouseDown(BgrWin* bgr,int x,int y,int but);
		static void onMouseMove(BgrWin* bgr,int x,int y,int but);
		static void onMouseUp(BgrWin* bgr,int x,int y,int but);
	};
}

#endif /* WIDGETS_LIST_WIN_HPP_ */
