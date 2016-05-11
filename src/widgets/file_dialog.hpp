/*
 * file_dialog.hpp
 *
 *  Created on: 11 de mai de 2016
 *      Author: carlosfaruolo
 */

#ifndef WIDGETS_FILE_DIALOG_HPP_
#define WIDGETS_FILE_DIALOG_HPP_

#include "dialog_bgrwin.hpp"
#include "label_win.hpp"

namespace WidgetsExtra
{
	struct FileDialog extends WidgetsExtra::DialogBgrWin
	{
		enum FileDialogMode { SELECT_FILE, SAVE_FILE, SELECT_FOLDER };
		FileDialog(FileDialogMode mode=SELECT_FILE);
		virtual ~FileDialog();

		virtual void setPosition(Point position);

		protected:
		FileDialogMode mode;
		LabelWin lblCurrentDirectory;
		CmdMenu cmdmCurrentDirectoryField;
		Button btnGoHome;
		LabelWin lblFilename;
		DialogWin dlgwFilenameField;

		static void triggerNavigation(Button*);
		static void folderOpened(const char* path,Id);
		static void navigateToHome(Button*);
	};
}

#endif /* WIDGETS_FILE_DIALOG_HPP_ */
