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

struct FileChooserDialog extends BgrWin
{
	string currentDirectory;

	FileChooserDialog();
	virtual ~FileChooserDialog();

	void setVisible(bool visible=true);

	protected:
	Rect titleBarArea;
	Button closeButton;
	SDL_util::LabelWin lblLookIn;
	DropDownMenu* cbLookIn;
	static void draw(BgrWin*);
	static void close(Button* btn);
};



#endif /* WIDGETS_FILE_CHOOSER_HPP_ */
