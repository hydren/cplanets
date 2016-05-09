/*
 * file_chooser.hpp
 *
 *  Created on: 5 de mai de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_FILE_CHOOSER_HPP_
#define WIDGETS_FILE_CHOOSER_HPP_

#include "SDL_widgets/SDL_widgets.h"
#include "futil/futil.hpp"
#include "widgets_util.hpp"
#include "label_win.hpp"
#include "drop_menu.hpp"
#include "abstract_layout.hpp"

struct FileChooserDialog extends BgrWin, SDL_util::Layout::WinBaseWrapper
{
	string currentDirectory;

	FileChooserDialog();
	virtual ~FileChooserDialog();

	virtual void setVisible(bool visible=true);
	virtual void setPosition(Point position);

	protected:
	Rect titleBarArea;
	Button closeButton;
	SDL_util::LabelWin lblLookIn;
	DropDownMenu* cbLookIn;
	static void draw(BgrWin*);
	static void close(Button* btn);
};



#endif /* WIDGETS_FILE_CHOOSER_HPP_ */
