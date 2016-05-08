/*
 * file_chooser.cpp
 *
 *  Created on: 5 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "file_chooser.hpp"

#include <unistd.h>

using SDL_util::LabelWin;


FileChooserDialog::FileChooserDialog()
: BgrWin(null, Rect(0, 0, 400, 300), null, FileChooserDialog::draw, null, null, null, 0),
  titleBarArea(Rect(1, 1, this->tw_area.w-2, 1.5 * TTF_FontHeight(draw_title_ttf->ttf_font) -2)),
  closeButton(this, 0, Rect(titleBarArea.w - titleBarArea.h + 2, 3, titleBarArea.h-4, titleBarArea.h-4), "X", null),
  lblLookIn(this, Rect(4, titleBarArea.h + 4, 0, 0), "Look in:"),
  cbLookIn(null)
{
	static char buffer[1024];
	this->currentDirectory = getcwd(buffer, 1024);
	DropDownMenuFactory dfactory;
	dfactory.setAppearance(DropDownMenuFactory::COMBOBOX);
	dfactory.setCallback(null);
	dfactory.addItem(currentDirectory.c_str());
	dfactory.setSize(Rect(0, 0, titleBarArea.w*0.6, TTF_FontHeight(draw_title_ttf->ttf_font)));
	cbLookIn = dfactory.createAt(this, Point(lblLookIn.area.x + lblLookIn.tw_area.w + 4, lblLookIn.area.y));
}

FileChooserDialog::~FileChooserDialog()
{}

void FileChooserDialog::setVisible(bool visible)
{
	if(visible)
	{
		if(this->parent == null)
		{
			this->keep_on_top(); //binds to main window
			SDL_util::setComponentPosition(this, Point(this->parent->tw_area.w*0.5-this->tw_area.w*0.5, this->parent->tw_area.h*0.5-this->tw_area.h*0.5)); //centers the dialog
			this->bgcol = parent->bgcol; //inherit background color
			this->draw_blit_recur();
			this->upd();
		}
		else this->show();
	}
	else
	{
		if(this->parent != null)
			this->hide();
	}
}

void FileChooserDialog::draw(BgrWin* bwSelf)
{
	FileChooserDialog* self = static_cast<FileChooserDialog*>(bwSelf);

	self->init_gui();
	self->draw_raised(0, self->bgcol, true);

	SDL_util::Color5::GradientBlue.draw_gradient(self, self->titleBarArea);

	draw_title_ttf->draw_string(self->win, "Choose file", Point(4, 4));
}
