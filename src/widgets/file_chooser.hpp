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

struct FileChooserDialog extends BgrWin
{
	string currentDirectory;

	FileChooserDialog();
	virtual ~FileChooserDialog();

	void setVisible(bool visible=true);

	protected:
	Rect titleBarArea;
	static void draw(BgrWin*);
};



#endif /* WIDGETS_FILE_CHOOSER_HPP_ */
