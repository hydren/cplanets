/*
 * dialog_bgrwin.hpp
 *
 *  Created on: 11 de mai de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_DIALOG_BGRWIN_HPP_
#define WIDGETS_DIALOG_BGRWIN_HPP_

#include <string>

#include "SDL_widgets/SDL_widgets.h"

#include "abstract_layout.hpp"
#include "futil/general/language.hpp"

namespace WidgetsExtra
{
	struct DialogBgrWin extends BgrWin, Layout::WinBaseWrapper
	{
		void (*onClosedCallback)(DialogBgrWin*);
		std::string titleStr;
		Rect titleBarArea; //automatically set

		DialogBgrWin(Rect bounds, std::string title="", void (*onClosed)(DialogBgrWin*)=null);
		virtual ~DialogBgrWin();

		virtual void setVisible(bool visible=true);
		virtual void setPosition(Point position);
		void setPositionOnCenter();

		protected:
		int titleStrOffset;
		Button btnClose;

		virtual void bind();
		virtual void validate();

		static void draw(BgrWin*);
		static void close(Button* btn);
		static void custom_mwin_down(BgrWin* bgr,int x,int y,int but);
	};
}




#endif /* WIDGETS_DIALOG_BGRWIN_HPP_ */
