/*
 * help.h
 *
 *  Created on: 6 de out de 2016
 *      Author: carlosfaruolo
 */

const char* HELP_TEXT = "The following is a ist of keybindings: "
;

vector<string> aux_help_text_keybind_key, aux_help_text_keybind_desc;

namespace aux_help_text_keybind {
	static void s(const char* key, const char* desc) { aux_help_text_keybind_key.push_back(key); aux_help_text_keybind_desc.push_back(desc); }

	static void init()
	{
		static bool once = false;
		if(once) return;

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

		once = true;
	}
}
