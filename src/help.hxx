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

	unsigned extra_height;

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

		once = true;
	}
}

void drawHelpDialog(BgrWin* bw)
{
	BgrWin* dialog = &sclpHelpText->content;
	WidgetsExtra::drawBgrWin(bw);
	mltHelpText->draw(dialog->win);

	Rect rt;
	rt.x = 8*WIDGETS_SPACING;
	Point pt(16*WIDGETS_SPACING, 0);

	unsigned offset = mltHelpText->getTextHeight() + WIDGETS_SPACING;
	for(unsigned i = 0; i < aux_help_text::btn_icons.size(); i++, offset += btnAbout->icon.image->h*1.5)
	{
		rt.y = offset; pt.y = offset;
		SDL_BlitSurface(aux_help_text::btn_icons[i], null, dialog->win, &rt);
		draw_blue_ttf->draw_string(dialog->win, aux_help_text::btn_desc[i].c_str(), pt);
	}

	offset += 2*TTF_FontHeight(draw_ttf->ttf_font);

	pt.x = 4*WIDGETS_SPACING;
	pt.y = offset;
	draw_ttf->draw_string(dialog->win, "The following is a list of key bindings:" , pt);

	offset += 2*TTF_FontHeight(draw_ttf->ttf_font);

	for(unsigned i = 0; i < aux_help_text::keybind_key.size(); i++, offset += TTF_FontHeight(draw_mono_ttf->ttf_font))
	{
		draw_mono_ttf->draw_string(dialog->win, aux_help_text::keybind_key[i].c_str(), Point(4*WIDGETS_SPACING, offset));
		draw_mono_ttf->draw_string(dialog->win, aux_help_text::keybind_desc[i].c_str(), Point(36*WIDGETS_SPACING, offset));
	}
}

