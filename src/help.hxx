/*
 * help.hxx
 *
 *  Created on: 6 de out de 2016
 *      Author: carlosfaruolo
 */

namespace aux_help_text {

	const char* content = "Welcome to CPlanets!"
			"\n" "CPlanets is a simple orbital planetary simulator, inspired by Yaron Minsky's \"Planets\"."
			"\n" "More info on Yaron Minsky's \"Planets\" (http://planets.homedns.org/)"
			"\n"
			"\n" "The following is a list of button descriptions: ";

	unsigned extra_height, listings_max_width;

	const unsigned
		offset_btn_icons = 8*WIDGETS_SPACING,
		offset_btn_desc = 16*WIDGETS_SPACING,
		offset_keybind_key = 4*WIDGETS_SPACING,
		offset_keybind_desc = 36*WIDGETS_SPACING;

	vector<SDL_Surface*> btn_icons;
	vector<string> btn_desc;
	static void p(SDL_Surface* surf, const char* desc) { btn_icons.push_back(surf); btn_desc.push_back(desc); }

	vector<string> keybind_key, keybind_desc;
	static void s(const char* key, const char* desc) { keybind_key.push_back(key); keybind_desc.push_back(desc); }

	static void init()
	{
		static bool once = false;
		if(once) return;

		p(btnNew->icon.image,					"Creates a new universe.");
		p(btnLoad->icon.image,					"Loads universe from a file");
		p(btnSave->icon.image,					"Saves universe to a file.");
		p(btnUndo->icon.image,					"Undoes last body insertion/removal.");
		p(btnRewind->icon.image,				"Goes back / Rewind to last body insertion/removal.");
		p(btnHelp->icon.image,					"Opens this help dialog.");
		p(btnAbout->icon.image,					"Opens the \"About\" dialog.");
		p(tglHideToolbars->icon.image,			"Hides/Shows toolbars.");
		p(tglCollapseLeftPanel->icon.image,		"Hides/Shows left toolbar.");
		p(btnPause->icon.image,					"Pauses the simulation.");
		p(btnRun->icon.image,					"Run/Unpauses the simulation.");
		p(btnAddBody->icon.image,				"Adds a body (interactively)");
		p(btnAddRandom->icon.image,				"Adds a random body.");
		p(btnAddRandomOrbiting->icon.image,		"Adds a random body (orbiting current center of mass).");
		p(btnRemove->icon.image,				"Removes currently selected bodies.");
		p(btnClear->icon.image,					"Removes all bodies.");
		p(btnRecolorAll->icon.image,			"Recolor all bodies.");
		p(btnFollowSelection->icon.image,		"Follows (set reference frame) currently selected bodies (activates C-O-M tracking)");
		p(btnResetReferenceFrame->icon.image,	"Resets reference frame (cancels C-O-M tracking).");
		p(tgbTraceOrbit->icon.image,			"Toogles orbit tracing");
		p(btnDoubleTraceLength->icon.image,		"Doubles orbit trace length.");
		p(btnHalveTraceLentgh->icon.image,		"Halves orbit trace length.");
		p(tgbAA->icon.image,					"Toogles anti-aliasing.");

		s("shift+h", 		"Display this help dialog");
		s("a", 				"Add body");
		s("+, =, PageUp", 	"Zoom in");
		s("-, _, PageDown", "Zoom out");
		s("b", 				"Toogle bounce on collision mode");
		s("c, spacebar", 	"Center (follow global center of mass)");
		s("k", 				"Hide/show toolbars");
		s("o", 				"Change all colors");
		s("q, espace", 		"Quit");
		s("e", 				"Reset to empty universe");
		s("s", 				"Save universe");
		s("l", 				"Load universe");
		s("u", 				"Undo (undoes last body insertion/removal)");
		s("g", 				"Go back/Rewind (goes back to last body insertion/removal)");
		s("p", 				"Toogle pause");
		s("t", 				"Toogle orbit tracing");
		s("d", 				"Double trace length");
		s("h", 				"Halve trace length");
		s("j", 				"Place random orbiting body");
		s("shift+j", 		"Place random orbiting body (unidirectional)");
		s("r", 				"Place random body");
		s("x", 				"Cancel C-O-M tracking/Stop follow");
		s("Up arrow", 		"Pan up");
		s("Down arrow", 	"Pan down");
		s("Left arrow", 	"Pan left");
		s("Right arrow", 	"Pan right");

		extra_height = (keybind_desc.size()+2)*TTF_FontHeight(draw_mono_ttf->ttf_font) + 1.5*btnAbout->icon.image->h*btn_icons.size();

		listings_max_width = 0;
		for(unsigned i = 0; i < btn_desc.size(); i++)
		{
			unsigned size = offset_btn_desc + draw_blue_ttf->text_width(btn_desc[i].c_str());
			if(i == 0 or size > listings_max_width)
				listings_max_width = size;
		}

		for(unsigned i = 0; i < keybind_desc.size(); i++)
		{
			unsigned size = offset_keybind_desc + draw_mono_ttf->text_width(keybind_desc[i].c_str());
			if(size > listings_max_width)
				listings_max_width = size;
		}

		once = true;
	}
}

void drawHelpDialog(BgrWin* bw)
{
	BgrWin* dialog = &sclpHelpText->content;
	WidgetsExtra::drawBgrWin(bw);
	mltHelpText->draw(dialog->win);

	Rect rt;
	rt.x = aux_help_text::offset_btn_icons;
	Point pt(aux_help_text::offset_btn_desc, 0);

	unsigned offset = mltHelpText->getTextHeight() + WIDGETS_SPACING;
	for(unsigned i = 0; i < aux_help_text::btn_icons.size(); i++, offset += btnAbout->icon.image->h*1.5)
	{
		rt.y = pt.y = offset;
		SDL_BlitSurface(aux_help_text::btn_icons[i], null, dialog->win, &rt);
		draw_blue_ttf->draw_string(dialog->win, aux_help_text::btn_desc[i].c_str(), pt);
	}

	offset += 2*TTF_FontHeight(draw_ttf->ttf_font);

	pt.x = aux_help_text::offset_keybind_key;
	pt.y = offset;
	draw_ttf->draw_string(dialog->win, "The following is a list of key bindings:" , pt);

	offset += 2*TTF_FontHeight(draw_ttf->ttf_font);

	for(unsigned i = 0; i < aux_help_text::keybind_key.size(); i++, offset += TTF_FontHeight(draw_mono_ttf->ttf_font))
	{
		pt.y = offset;
		draw_mono_ttf->draw_string(dialog->win, aux_help_text::keybind_key[i].c_str(), pt);
		draw_mono_ttf->draw_string(dialog->win, aux_help_text::keybind_desc[i].c_str(), Point(aux_help_text::offset_keybind_desc, offset));
	}
}

void adjustHelpDialogContent()
{
	using aux_help_text::keybind_desc;
	using aux_help_text::btn_desc;

	aux_help_text::init();

	const int headerSize = dialogHelp->titleBarArea.h + TTF_FontHeight(draw_title_ttf->ttf_font)*2;
	const int totalSize = headerSize + mltHelpText->getTextHeight() + aux_help_text::extra_height;
	const unsigned totalWidth = max(mltHelpText->getTextWidth(), aux_help_text::listings_max_width);

	//expand BgrWin to fit the text
	if(totalSize > sclpHelpText->content.tw_area.h)
	{
		sclpHelpText->widenContent(totalWidth - sclpHelpText->content.tw_area.w, totalSize - sclpHelpText->content.tw_area.h);
		sclpHelpText->refresh();
	}

	sclpHelpText->scrollingSpeedVertical = totalSize/20;
}

