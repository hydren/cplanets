/*
 * file_chooser.cpp
 *
 *  Created on: 5 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "file_chooser.hpp"

#include <unistd.h>

using SDL_util::LabelWin;

using SDL_util::setComponentPosition;

int NAVIGATION_BUTTONS_SIZE = 18;

FileChooserDialog::FileChooserDialog()
: BgrWin(null, Rect(0, 0, 400, 300), null, FileChooserDialog::draw, mwin::down, mwin::move, mwin::up, 0),
  WinBaseWrapper(this),
  titleBarArea(Rect(0, 0, this->tw_area.w-2, 1.5 * TTF_FontHeight(draw_title_ttf->ttf_font) -2)),
  btnClose(this, Style(0,1), Rect(0, 0, titleBarArea.h-4, titleBarArea.h-4), "X", FileChooserDialog::close),
  layoutNorth(4, titleBarArea.h + 4, 400),
  lblLookIn(this, Rect(), "Look in:"),
  cbLookIn(null),
  btnFolderUp(this, 0, Rect(0, 0, NAVIGATION_BUTTONS_SIZE, NAVIGATION_BUTTONS_SIZE), "Up", null),
  btnHome(this, 0, Rect(0, 0, NAVIGATION_BUTTONS_SIZE, NAVIGATION_BUTTONS_SIZE), "H", null),
  btnNewFolder(this, 0, Rect(0, 0, NAVIGATION_BUTTONS_SIZE, NAVIGATION_BUTTONS_SIZE), "*", null)
{
	static char buffer[1024];
	currentDirectory = getcwd(buffer, 1024);
	this->currentDirectory = "..." + currentDirectory.substr(Math::max(0, (int)currentDirectory.size()-36));
	DropDownMenuFactory dfactory;
	dfactory.setAppearance(DropDownMenuFactory::COMBOBOX);
	dfactory.setCallback(null);
	dfactory.addItem(currentDirectory.c_str());
	//todo add all parent hierarchy on file dialog
	dfactory.setSize(Rect(0, 0, titleBarArea.w*0.67, 1.25*TTF_FontHeight(draw_title_ttf->ttf_font)));
	cbLookIn = dfactory.createAt(this, Point(lblLookIn.area.x + lblLookIn.tw_area.w + 4, lblLookIn.area.y));

	layoutNorth.addComponent(lblLookIn);
	layoutNorth.addComponent(cbLookIn);
	layoutNorth.addComponent(btnFolderUp);
	layoutNorth.addComponent(btnHome);
	layoutNorth.addComponent(btnNewFolder);

	//centers the dialog
	setPosition(Point(SDL_GetVideoSurface()->w*0.5-this->tw_area.w*0.5, SDL_GetVideoSurface()->h*0.5-this->tw_area.h*0.5));

	this->keep_on_top(); //binds to main window
	this->bgcol = parent->bgcol; //inherit background color
	this->draw_blit_recur();
	this->upd();
}

FileChooserDialog::~FileChooserDialog()
{}

void FileChooserDialog::setVisible(bool visible)
{
	if(visible && this->hidden) this->show();
	else if(not visible && not this->hidden) this->hide();
}

void FileChooserDialog::draw(BgrWin* bwSelf)
{
	FileChooserDialog* self = static_cast<FileChooserDialog*>(bwSelf);

	self->init_gui();
	self->draw_raised(0, self->bgcol, true);

	SDL_util::Color5::GradientBlue.draw_gradient(self, self->titleBarArea);

	draw_title_ttf->draw_string(self->win, "Choose file", Point(4, 4));
}

void FileChooserDialog::close(Button* btn)
{
	FileChooserDialog* self = static_cast<FileChooserDialog*>(btn->parent);
	self->setVisible(false);
}

void FileChooserDialog::setPosition(Point position)
{
	WinBaseWrapper::setPosition(position); //set bgrwin position as a WinBaseWrapper

	//shift properly all components inside it
	titleBarArea = Rect(0, 0, this->tw_area.w-2, 1.5 * TTF_FontHeight(draw_title_ttf->ttf_font) -2);
	setComponentPosition(&btnClose, Point(titleBarArea.w - titleBarArea.h + 2, 3));

	layoutNorth.position = Point(4, titleBarArea.h + 4);
	layoutNorth.pack();

//	setComponentPosition(&lblLookIn, Point(4, titleBarArea.h + 4));
//	cbLookIn->setPosition(Point(lblLookIn.area.x + lblLookIn.tw_area.w + 4, lblLookIn.area.y));
//	setComponentPosition(&btnFolderUp, Point(cbLookIn->getPosition().x + cbLookIn->getSize().w + 4, cbLookIn->getPosition().y));
//	setComponentPosition(&btnHome, Point(btnFolderUp.area.x + btnFolderUp.tw_area.w + 4, btnFolderUp.area.y));
//	setComponentPosition(&btnNewFolder, Point(btnHome.area.x + btnHome.tw_area.w + 4, btnHome.area.y));
}
