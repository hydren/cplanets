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

	const char* modeCurrentDirectory(FileDialog::FileDialogMode mode)
	{
		switch(mode)
		{
			case FileDialog::SELECT_FILE: return "Look in:";
			case FileDialog::SAVE_FILE: return "Save in:";
			case FileDialog::SELECT_FOLDER: return "Folder:";
			default: return "File dialog...";
		}
	}

	const char* modeFilename(FileDialog::FileDialogMode mode)
	{
		switch(mode)
		{
			case FileDialog::SELECT_FILE: return "Filename:";
			case FileDialog::SAVE_FILE: return "Name:";
			default: return "<name>:";
		}
	}
}

FileDialog::FileDialog(FileDialogMode mode, void (*onFinished)(FileDialog*))
: DialogBgrWin(Rect(0, 0, 400, 150), FileDialog_static::modeTitle(mode)),
  onFinishedCallback(onFinished), selectedFilename(null), mode(mode),
  lblCurrentDirectory(this, Rect(), FileDialog_static::modeCurrentDirectory(mode)),
  cmdmCurrentDirectoryField(new Button(this, Style(4,0), Rect(0, 0, titleBarArea.w * 0.75, 1.5 * TTF_FontHeight(draw_ttf->ttf_font)), getcwd(buffer, 1024), FileDialog::triggerNavigation)),
  btnGoHome(this, 0, Rect(), " H ", FileDialog::navigateToHome),
  lblFilename(this, Rect(), FileDialog_static::modeFilename(mode)),
  dlgwFilenameField(this, Rect(0, 0, titleBarArea.w * 0.75, 1.25 * TTF_FontHeight(draw_ttf->ttf_font))),
  layoutSouthButtons(0, 0, 400),
  btnOk(this, 0, Rect(), "  Ok  ", FileDialog::confirmation),
  btnCancel(this, 0, Rect(), "Cancel", DialogBgrWin::close)
{
	this->id = FileDialog_static::lastId++; //getting new (presumedly unique) id
	FileDialog_static::references[id.id1] = this; //binding this id to this address
	dlgwFilenameField.cmd = FileDialog::getFieldText; //setting callback for dok()
	dlgwFilenameField.cmd_id = id.id1; //setting id of this FileDialog of reference purposes

	packLabeledComponent(&btnGoHome);

	if(mode == SELECT_FOLDER)
	{
		lblFilename.hidden = true;
		dlgwFilenameField.hidden = true;
	}

	layoutSouthButtons.addComponent(new WidgetsExtra::Layout::Spacer(Rect()));
	layoutSouthButtons.addComponent(btnOk); packLabeledComponent(&btnOk);
	layoutSouthButtons.addComponent(btnCancel); packLabeledComponent(&btnCancel);
	layoutSouthButtons.addComponent(new WidgetsExtra::Layout::Spacer(Rect(0,0,8,0)));
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
	layoutSouthButtons.position.y = 120;
	layoutSouthButtons.pack();

	//todo find a better way to call all draw() functions on FileDialog::setPosition
	lblCurrentDirectory.draw_blit_upd();
	cmdmCurrentDirectoryField.src->draw_blit_upd();
	btnGoHome.draw_blit_upd();
	lblFilename.draw_blit_upd();
	dlgwFilenameField.draw_blit_upd();
	btnOk.draw_blit_upd();
	btnCancel.draw_blit_upd();
}

void FileDialog::replaceSelectedFilename(const char* path, const char* filename)
{
	string str = path;
	str = str + '/' + filename;
	if(this->selectedFilename != null)
		delete selectedFilename;
	this->selectedFilename = new string(str);
}

//called when clicking on the navigation field
void FileDialog::triggerNavigation(Button* navButton)
{
	FileDialog* self = static_cast<FileDialog*>(navButton->parent);
	if(self->mode == SELECT_FILE)
		file_chooser(FileDialog::fileSelected, self->id);
	else if(self->mode == SAVE_FILE || self->mode == SELECT_FOLDER)
		working_dir(FileDialog::folderOpened, self->id);
}

//called when selected a file when browsing
void FileDialog::fileSelected(const char* path, Id id)
{
	SDL_util::setWindowTitle("cplanets");
	FileDialog* self = FileDialog_static::references[id.id1];

	self->cmdmCurrentDirectoryField.src->label = getcwd(buffer, 1024);
	self->cmdmCurrentDirectoryField.src->draw_blit_upd();
	self->dlgwFilenameField.dialog_def(path, self->dlgwFilenameField.cmd, self->dlgwFilenameField.cmd_id);
	self->replaceSelectedFilename(getcwd(buffer, 1024), path); //retrieve filename
}

//called when changed folder when browsing
void FileDialog::folderOpened(const char* path, Id id)
{
	SDL_util::setWindowTitle("cplanets");
	FileDialog* self = FileDialog_static::references[id.id1];

	self->cmdmCurrentDirectoryField.src->label = getcwd(buffer, 1024);
	self->cmdmCurrentDirectoryField.src->draw_blit_upd();
	self->replaceSelectedFilename(getcwd(buffer, 1024)); //retrieve folder name
}

//called when the user press the "Home" button
void FileDialog::navigateToHome(Button* navButton) //fixme FileDialog::navigateToHome doesn't update file_chooser() bgr
{
	FileDialog* self = static_cast<FileDialog*>(navButton->parent);

	chdir(getenv("HOME")!=null? getenv("HOME") : getenv("USERPROFILE"));
	self->cmdmCurrentDirectoryField.src->label = getcwd(buffer, 1024);
	self->cmdmCurrentDirectoryField.src->draw_blit_upd();
}

//to be called by dlgwFilenameField when we call dok() on it in the FileDialog::confirmation() call
void FileDialog::getFieldText(const char* txt, int id1)
{
	FileDialog* self = FileDialog_static::references[id1];
	self->replaceSelectedFilename(self->cmdmCurrentDirectoryField.src->label.str, txt);
}

//called when we click Ok button
void FileDialog::confirmation(Button* okBtn)
{
	FileDialog* self = static_cast<FileDialog*>(okBtn->parent);
	if(self->mode == SAVE_FILE) //filename was not retrieved
		self->dlgwFilenameField.dok(); //retrieve filename (typed by user)
	if(self->onFinishedCallback != null) self->onFinishedCallback(self);
	DialogBgrWin::close(okBtn);
}

