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
#include "flow_layout.hpp"

struct FileChooserDialog extends BgrWin, SDL_util::Layout::WinBaseWrapper
{
	string currentDirectory;

	FileChooserDialog();
	virtual ~FileChooserDialog();

	virtual void setVisible(bool visible=true);
	virtual void setPosition(Point position);

	protected:
	Rect titleBarArea;
	Button btnClose;

	SDL_util::FlowLayout layoutNorth;
	SDL_util::LabelWin lblLookIn;
	DropDownMenu* cbLookIn;
	Button btnFolderUp, btnHome, btnNewFolder;

	string currentDirectoryAux;
	std::vector<string> currentDirectoryFiles;
	RButWin *entries;
	void fillEntries();
	static void draw(BgrWin*);
	static void close(Button* btn);
	static void custom_mwin_down(BgrWin* bgr,int x,int y,int but);
};



#endif /* WIDGETS_FILE_CHOOSER_HPP_ */
