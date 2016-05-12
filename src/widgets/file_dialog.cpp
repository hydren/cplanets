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

//xxx kludges to maintain references when calling FileDialog::folderOpened()
namespace FileDialog_static
{
	int lastId = 0;
	map<int, FileDialog*> references;

	map<RButWin*, FileDialog*> referencesRButWin;

	const char* modeTitle(FileDialog::FileDialogMode mode)
	{
		switch(mode)
		{
			case FileDialog::SELECT_FILE: 	return "Choose file...";
			case FileDialog::SAVE_FILE: 	return "Save file...";
			case FileDialog::SELECT_FOLDER: return "Choose folder...";
			default: 						return "File dialog...";
		}
	}

	const char* modeCurrentDirectory(FileDialog::FileDialogMode mode)
	{
		switch(mode)
		{
			case FileDialog::SELECT_FILE: 	return "Look in:  ";
			case FileDialog::SAVE_FILE: 	return "Save in:  ";
			case FileDialog::SELECT_FOLDER:	return "Folder:   ";
			default: 						return "Path:     ";
		}
	}

	const char* modeFilename(FileDialog::FileDialogMode mode)
	{
		switch(mode)
		{
			case FileDialog::SELECT_FILE:	return "Filename: ";
			case FileDialog::SAVE_FILE:		return "Name:     ";
			default:						return "<name>:   ";
		}
	}
}

FileDialog::FileDialog(FileDialogMode mode, void (*onFinished)(FileDialog*))
: DialogBgrWin(Rect(0, 0, 400, mode==SELECT_FOLDER? 96 : 160), FileDialog_static::modeTitle(mode)),
  onFinishedCallback(onFinished), selectedFilename(null), mode(mode),
  lblCurrentDirectory(this, Rect(), FileDialog_static::modeCurrentDirectory(mode)),
  cmdmCurrentDirectoryField(new Button(this, Style(4,0), Rect(0, 0, titleBarArea.w * 0.75, 1.5 * TTF_FontHeight(draw_ttf->ttf_font)), getcwd(buffer, 1024), FileDialog::triggerNavigation)),
  btnGoHome(this, 0, Rect(), " H ", FileDialog::navigateToHome),
  lblFilename(this, Rect(), FileDialog_static::modeFilename(mode)),
  dlgwFilenameField(this, Rect(0, 0, titleBarArea.w * 0.75, 1.25 * TTF_FontHeight(draw_ttf->ttf_font))),
  ddmFileType(null),
  layoutSouthButtons(0, 0, 400),
  btnOk(this, 0, Rect(), "  Ok  ", FileDialog::confirmation),
  btnCancel(this, 0, Rect(), "Cancel", FileDialog::cancellation)
{
	this->id = FileDialog_static::lastId++; //getting new (presumedly unique) id
	FileDialog_static::references[id.id1] = this; //binding this id to this address
	dlgwFilenameField.cmd = FileDialog::getFieldText; //setting callback for dok()
	dlgwFilenameField.cmd_id = id.id1; //setting id of this FileDialog of reference purposes
	this->btnClose.cmd = FileDialog::cancellation; //replacing callback when closing

	packLabeledComponent(&btnGoHome); //shaping btnGoHome

	layoutSouthButtons.spacing_h = 8;
	layoutSouthButtons.addComponent(new WidgetsExtra::Layout::Spacer(Rect()));
	layoutSouthButtons.addComponent(btnOk); packLabeledComponent(&btnOk);
	layoutSouthButtons.addComponent(btnCancel); packLabeledComponent(&btnCancel);

	if(mode == SELECT_FOLDER)
	{
		lblFilename.hidden = true;
		dlgwFilenameField.hidden = true;
	}
	else
	{
		DropDownMenuFactory ddmf;
		ddmf.setLabel("File type:  ");
		ddmf.setAppearance(DropDownMenuFactory::COMBOBOX);
		ddmf.addItem("All files");
		ddmf.setSize(Rect(0, 0, dlgwFilenameField.tw_area.w * 0.5, 1.25 * TTF_FontHeight(draw_ttf->ttf_font)));
		ddmf.setCallback(FileDialog::selectFileType);

		ddmFileType = ddmf.createAt(this);
		//fixme ddmFileType->cmdMenu->buttons is 0 at this point. Why? Maybe its buttons are created dynamically, which ruins the whole point (no way of holding reference to parent)
		FileDialog_static::referencesRButWin[ddmFileType->cmdMenu->buttons] = this; //binding this rbutwin pointer to this address
	}

	this->validate();
}

FileDialog::~FileDialog()
{
	delete ddmFileType;
}

void FileDialog::bind()
{
	this->keep_on_top(); //binds to main window
	this->bgcol = parent->bgcol; //inherit background color
	this->dlgwFilenameField.bgcol = parent->bgcol; //black prior to this call
	this->draw_blit_recur();
	this->upd();
}

void FileDialog::validate()
{
	//shift properly all components inside it
	DialogBgrWin::validate();
	setComponentPosition(&lblCurrentDirectory, 8, titleBarArea.y + titleBarArea.h + 12);
	setComponentPosition(cmdmCurrentDirectoryField.src, lblCurrentDirectory.area.x + lblCurrentDirectory.tw_area.w + 6, lblCurrentDirectory.area.y - 2);
	setComponentPosition(&btnGoHome, cmdmCurrentDirectoryField.src->area.x + cmdmCurrentDirectoryField.src->tw_area.w + 6, lblCurrentDirectory.area.y - 3);
	setComponentPosition(&lblFilename, 8, lblCurrentDirectory.area.y + cmdmCurrentDirectoryField.src->tw_area.h + 16);
	setComponentPosition(&dlgwFilenameField, cmdmCurrentDirectoryField.src->area.x, lblFilename.area.y - TTF_FontHeight(draw_ttf->ttf_font));
	if(ddmFileType != null) ddmFileType->setPosition(Point(8, dlgwFilenameField.area.y + dlgwFilenameField.tw_area.h + 12));

	layoutSouthButtons.position.y = this->tw_area.h - btnOk.tw_area.h - 6;
	layoutSouthButtons.pack();
}

void FileDialog::replaceSelectedFilename(const char* path, const char* filename)
{
	string str = path;
	str = str + '/' + filename; //todo review usage of / character as file separator here.
	if(this->selectedFilename != null)
		delete selectedFilename;
	this->selectedFilename = new string(str);
}

//================================= callbacks / static functions ===========================================

//to be called by dlgwFilenameField when we call dok() on it in the FileDialog::confirmation() call
void FileDialog::getFieldText(const char* txt, int id1)
{
	FileDialog* self = FileDialog_static::references[id1];
	self->replaceSelectedFilename(self->cmdmCurrentDirectoryField.src->label.str, txt);
}

//called when clicking on the navigation field
void FileDialog::triggerNavigation(Button* navButton)
{
	FileDialog* self = static_cast<FileDialog*>(navButton->parent);
	Point fchooserPos(navButton->tw_area.x + navButton->tw_area.w * 0.5, navButton->tw_area.y + navButton->tw_area.h * 0.5);

	if(self->mode == SELECT_FILE)
		file_chooser(FileDialog::fileSelected, self->id, true, fchooserPos.x, fchooserPos.y);
	else if(self->mode == SAVE_FILE || self->mode == SELECT_FOLDER)
		working_dir(FileDialog::folderOpened, self->id, true, fchooserPos.x, fchooserPos.y);
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

void FileDialog::selectFileType(RButWin* selection, int nr, int fire)
{
	if(fire)
	{
		map<RButWin*, FileDialog*>* hh = &FileDialog_static::referencesRButWin;
		for(map<RButWin*, FileDialog*>::iterator it = hh->begin(); it != hh->end(); it++)
		{
			std::cout << (*it).first << " " << (*it).second << std::endl;
		}

		FileDialog* self = FileDialog_static::referencesRButWin[selection];
		self->ddmFileType->cmdMenu->src->label = selection->act_button()->label.str;
		self->ddmFileType->cmdMenu->src->draw_blit_upd();
	}
}

//called when we click Ok button
void FileDialog::confirmation(Button* okBtn)
{
	FileDialog* self = static_cast<FileDialog*>(okBtn->parent);
	if(self->mode == SAVE_FILE) //filename was not retrieved
		self->dlgwFilenameField.dok(); //retrieve filename (typed by user)
	if(self->onFinishedCallback != null) self->onFinishedCallback(self);
	DialogBgrWin::close(okBtn);
	close_file_chooser();
}

//called when we click Cancel or 'X' button
void FileDialog::cancellation(Button* okBtn)
{
	FileDialog* self = static_cast<FileDialog*>(okBtn->parent);
	if(self->selectedFilename != null) { delete self->selectedFilename; self->selectedFilename = null; }
	if(self->onFinishedCallback != null) self->onFinishedCallback(self);
	DialogBgrWin::close(okBtn);
	close_file_chooser();
}

