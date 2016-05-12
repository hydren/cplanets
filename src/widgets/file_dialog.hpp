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
#include "flow_layout.hpp"

namespace WidgetsExtra
{
	struct FileDialog extends WidgetsExtra::DialogBgrWin
	{
		void (*onFinishedCallback)(FileDialog* dialog);
		string* selectedFilename;

		enum FileDialogMode { SELECT_FILE, SAVE_FILE, SELECT_FOLDER };
		FileDialog(FileDialogMode mode=SELECT_FILE, void (*onFinished)(FileDialog* dialog)=null);
		virtual ~FileDialog();

		protected:
		FileDialogMode mode;
		LabelWin lblCurrentDirectory;
		CmdMenu cmdmCurrentDirectoryField;
		Button btnGoHome;
		LabelWin lblFilename;
		DialogWin dlgwFilenameField;
		FlowLayout layoutSouthButtons;
		Button btnOk, btnCancel;

		virtual void bind();
		virtual void validate();

		void replaceSelectedFilename(const char* path, const char* filename="");

		static void triggerNavigation(Button*);
		static void fileSelected(const char* path,Id);
		static void folderOpened(const char* path,Id);
		static void navigateToHome(Button*);
		static void getFieldText(const char* txt,int);
		static void confirmation(Button*);
	};
}

#endif /* WIDGETS_FILE_DIALOG_HPP_ */
