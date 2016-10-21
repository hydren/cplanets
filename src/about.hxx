/*
 * about.hxx
 *
 *  Created on: 21 de out de 2016
 *      Author: carlosfaruolo
 */

// "About" dialog details
namespace aux_about_text
{
	// pseudo-constants
	string content, version_text, title_text;

	void init()
	{
		static bool once = false;
		if(once) return;

		content = string("This program is inspired by Yaron Minsky's \"planets\" program.\n\n").append(CPLANETS_LICENSE);
		version_text = string("Version ").append(CPLANETS_VERSION);
		title_text = string("cplanets, a interactive program to play with gravitation");

		once = true;
	}
}

void drawAboutDialog(BgrWin* bw)
{
	BgrWin* dialog = &sclpAboutLicense->content;
	WidgetsExtra::drawBgrWin(bw);

	int logoOffset = 0;
	if(APP_LOGO != null)
	{
		SDL_Rect position = {WIDGETS_SPACING, WIDGETS_SPACING, 0, 0};
		SDL_BlitSurface(APP_LOGO, null, bw->win, &position);
		logoOffset = APP_LOGO->w;
	}

	draw_title_ttf->draw_string(dialog->win, aux_about_text::title_text.c_str()  , Point(logoOffset + 3*WIDGETS_SPACING, 2.5*WIDGETS_SPACING));
	draw_title_ttf->draw_string(dialog->win, aux_about_text::version_text.c_str(), Point(logoOffset + 3*WIDGETS_SPACING, 2.5*WIDGETS_SPACING + TTF_FontHeight(draw_title_ttf->ttf_font)));

	mltAboutText->draw(dialog->win);
}

void adjustAboutDialogContent()
{
	const int headerSize = dialogAbout->titleBarArea.h + TTF_FontHeight(draw_title_ttf->ttf_font)*2;
	const int totalSize = headerSize + mltAboutText->getTextHeight();

	//expand BgrWin to fit the text
	if(totalSize > sclpAboutLicense->content.tw_area.h)
	{
		sclpAboutLicense->widenContent(mltAboutText->getTextWidth() - sclpAboutLicense->content.tw_area.w, totalSize - sclpAboutLicense->content.tw_area.h);
		sclpAboutLicense->refresh();
	}
}
