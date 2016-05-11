/*
 * file_dialog.cpp
 *
 *  Created on: 11 de mai de 2016
 *      Author: carlosfaruolo
 */

#include "file_dialog.hpp"

#include <unistd.h>
#include <map>
#include "SDL_util.hpp"
#include "widgets_util.hpp"

using WidgetsExtra::FileDialog;
using std::map;

char buffer[1024];

//xxx kludge to maintain references when calling FileDialog::folderOpened()
namespace FileDialog_static
{
	int lastId = 0;
	map<int, FileDialog*> references;
}

const char* modeTitle(FileDialog::FileDialogMode mode)
{
	switch(mode)
	{
		case FileDialog::SELECT_FILE: return "Choose file...";
		case FileDialog::SAVE_FILE: return "Save file...";
		case FileDialog::SELECT_FOLDER: return "Choose folder...";
		default: return "File dialog...";
	}
}

FileDialog::FileDialog(FileDialogMode mode)
: DialogBgrWin(Rect(0, 0, 400, 120), modeTitle(mode)), mode(mode),
  lblCurrentDirectory(this, Rect(), mode==SAVE_FILE? "Save in:" : "Look in:"),
  cmdmCurrentDirectoryField(new Button(this, Style(4,0), Rect(0, 0, titleBarArea.w * 0.75, 1.5 * TTF_FontHeight(draw_ttf->ttf_font)), getcwd(buffer, 1024), FileDialog::triggerNavigation)),
  btnGoHome(this, 0, Rect(), " H ", FileDialog::navigateToHome),
  lblFilename(this, Rect(), mode==SAVE_FILE? "Name:" : "Filename:"),
  dlgwFilenameField(this, Rect(0, 0, titleBarArea.w * 0.75, 1.25 * TTF_FontHeight(draw_ttf->ttf_font)))
{
	this->id = FileDialog_static::lastId++;
	FileDialog_static::references[id.id1] = this;
	packLabeledComponent(&btnGoHome);
	setPosition(Point());
//	setPosition(Point(SDL_GetVideoSurface()->w - 200, SDL_GetVideoSurface()->h - 150));
}

FileDialog::~FileDialog(){}

void FileDialog::setPosition(Point position)
{
	DialogBgrWin::setPosition(position);
	setComponentPosition(&lblCurrentDirectory, Point(8, titleBarArea.y + titleBarArea.h + 12));
	setComponentPosition(cmdmCurrentDirectoryField.src, Point(lblCurrentDirectory.area.x + lblCurrentDirectory.tw_area.w + 6, lblCurrentDirectory.area.y - 2));
	setComponentPosition(&btnGoHome, Point(cmdmCurrentDirectoryField.src->area.x + cmdmCurrentDirectoryField.src->tw_area.w + 6, lblCurrentDirectory.area.y - 3));
	setComponentPosition(&lblFilename, Point(8, lblCurrentDirectory.area.y + cmdmCurrentDirectoryField.src->tw_area.h + 32));
	setComponentPosition(&dlgwFilenameField, Point(lblFilename.area.x + lblFilename.tw_area.w + 6, lblFilename.area.y - TTF_FontHeight(draw_ttf->ttf_font)));

	lblCurrentDirectory.draw_blit_upd();
	cmdmCurrentDirectoryField.src->draw_blit_upd();
	btnGoHome.draw_blit_upd();
	lblFilename.draw_blit_upd();
	dlgwFilenameField.draw_blit_upd();
}

void FileDialog::triggerNavigation(Button* navButton)
{
	FileDialog* self = static_cast<FileDialog*>(navButton->parent);
	if(self->mode == SELECT_FILE)
		file_chooser(FileDialog::fileSelected, self->id);
	else if(self->mode == SAVE_FILE)
		working_dir(FileDialog::folderOpened, self->id);
}

void FileDialog::fileSelected(const char* path, Id id)
{
	SDL_util::setWindowTitle("cplanets");
	FileDialog* self = FileDialog_static::references[id.id1];

	self->cmdmCurrentDirectoryField.src->label = getcwd(buffer, 1024);
	self->cmdmCurrentDirectoryField.src->draw_blit_upd();
	self->dlgwFilenameField.dialog_def(path, self->dlgwFilenameField.cmd, self->dlgwFilenameField.cmd_id);
}

void FileDialog::folderOpened(const char* path, Id id)
{
	SDL_util::setWindowTitle("cplanets");
	FileDialog* self = FileDialog_static::references[id.id1];

	self->cmdmCurrentDirectoryField.src->label = getcwd(buffer, 1024);
	self->cmdmCurrentDirectoryField.src->draw_blit_upd();
}

void FileDialog::navigateToHome(Button* navButton) //fixme FileDialog::navigateToHome doesn't update file_chooser() bgr
{
	FileDialog* self = static_cast<FileDialog*>(navButton->parent);

	chdir(getenv("HOME")!=null? getenv("HOME") : getenv("USERPROFILE"));
	self->cmdmCurrentDirectoryField.src->label = getcwd(buffer, 1024);
	self->cmdmCurrentDirectoryField.src->draw_blit_upd();
}

