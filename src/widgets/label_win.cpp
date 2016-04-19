
#include "label_win.hpp"

using std::string;

LabelWin::LabelWin(WinBase* pw, Rect bounds, string str, Id id)
: WinBase(pw, null, bounds.x, bounds.y, bounds.w, bounds.h, 0, id), txt(str)
{}

LabelWin::~LabelWin(){}

void LabelWin::draw()
{
	init_gui();
	SDL_FillRect(win,0,parent->bgcol);
	draw_ttf->draw_string(win, txt.c_str(), Point(0, 0));
}
