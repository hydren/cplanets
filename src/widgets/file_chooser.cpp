/*
 * file_chooser.cpp
 *
 *  Created on: 5 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "file_chooser.hpp"

#include <unistd.h>
#include <dirent.h>
#include <string>

using SDL_util::LabelWin;
using SDL_util::setComponentPosition;
using std::vector;

vector<string> getFilenamesWithinDirectory(const string& directoryPath);

const int NAV_BTN_SIZE = 18;
const unsigned LOOK_IN_COMBOBOX_MAX_CHARS = 36;

FileChooserDialog::FileChooserDialog()
: BgrWin(null, Rect(0, 0, 400, 300), null, FileChooserDialog::draw, FileChooserDialog::custom_mwin_down, mwin::move, mwin::up, 0),
  WinBaseWrapper(this),
  titleBarArea(Rect(0, 0, this->tw_area.w-2, 1.5 * TTF_FontHeight(draw_title_ttf->ttf_font) -2)),
  btnClose(this, Style(0,1), Rect(0, 0, titleBarArea.h-4, titleBarArea.h-4), "X", FileChooserDialog::close),
  layoutNorth(4, titleBarArea.h + 4, 400),
  lblLookIn(this, Rect(), "Look in:"),
  cbLookIn(null),
  btnFolderUp(this, 0, Rect(0, 0, NAV_BTN_SIZE, NAV_BTN_SIZE), "/\\", null),
  btnHome(this, 0, Rect(0, 0, NAV_BTN_SIZE, NAV_BTN_SIZE), "H", null),
  btnNewFolder(this, 0, Rect(0, 0, NAV_BTN_SIZE, NAV_BTN_SIZE), "*", null)
{
	fillEntries();

	if(currentDirectory.size() > LOOK_IN_COMBOBOX_MAX_CHARS)
		currentDirectoryAux = "..." + currentDirectory.substr(currentDirectory.size() - LOOK_IN_COMBOBOX_MAX_CHARS);
	else
		currentDirectoryAux = currentDirectory;

	DropDownMenuFactory dfactory;
	dfactory.setAppearance(DropDownMenuFactory::COMBOBOX);
	dfactory.setCallback(null);
	dfactory.addItem(currentDirectoryAux.c_str());
	//todo add all parent hierarchy on file dialog
	dfactory.setSize(Rect(0, 0, titleBarArea.w*0.65, 1.25*TTF_FontHeight(draw_title_ttf->ttf_font)));
	cbLookIn = dfactory.createAt(this, Point(lblLookIn.area.x + lblLookIn.tw_area.w + 4, lblLookIn.area.y));
	cbLookIn->cmdMenu->sticky = false;
	cbLookIn->offset.y -= 1;

	layoutNorth.addComponent(lblLookIn);
	layoutNorth.addComponent(cbLookIn);
	layoutNorth.addComponent(new SDL_util::Layout::Spacer(Rect(0,0,4,0)));
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

void FileChooserDialog::setPosition(Point position)
{
	WinBaseWrapper::setPosition(position); //set bgrwin position as a WinBaseWrapper

	//shift properly all components inside it
	titleBarArea = Rect(1, 0, this->tw_area.w-2, 1.5 * TTF_FontHeight(draw_title_ttf->ttf_font) - 2);
	setComponentPosition(&btnClose, Point(titleBarArea.w - titleBarArea.h + 2, 2));

	layoutNorth.position = Point(12, titleBarArea.h + 12);
	layoutNorth.pack();

	setComponentPosition(entries, Point(12, titleBarArea.h + 24));
}

void FileChooserDialog::fillEntries()
{
	static char buffer[1024];
	currentDirectory = getcwd(buffer, 1024);

	currentDirectoryFiles = getFilenamesWithinDirectory(currentDirectory);
	entries = new RButWin(this, 0, Rect(), null, false, null);

	for(unsigned i = 0; i < currentDirectoryFiles.size(); i++)
		entries->add_rbut(currentDirectoryFiles[i].c_str());
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

void FileChooserDialog::custom_mwin_down(BgrWin* bgr,int x,int y,int but)
{
	Rect& titleBarArea = static_cast<FileChooserDialog*>(bgr)->titleBarArea;
	if(x > titleBarArea.x && x < titleBarArea.x + titleBarArea.w
	&& y > titleBarArea.y && y < titleBarArea.y + titleBarArea.h)
		mwin::down(bgr, x, y, but);
}

vector<string> getFilenamesWithinDirectory(const string& directoryPath)
{
	vector<string> filenames;

	DIR *dir;
	struct dirent *ent;
	if((dir = opendir (directoryPath.c_str())) != NULL)
	{
		while((ent = readdir (dir)) != NULL)
		{
			if(string(ent->d_name).compare("..") != 0 && string(ent->d_name).compare(".") != 0)  // exclude . and ..
				filenames.push_back(directoryPath+"/"+string(ent->d_name));
		}
		closedir (dir);
	}
	else futil::throw_exception("Could not open directory \"%s\"", directoryPath.c_str());
	return filenames;
}

