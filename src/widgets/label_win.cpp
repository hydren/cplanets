
#include "label_win.hpp"

using std::string;

using WidgetsExtra::LabelWin;

LabelWin::LabelWin(WinBase* pw, Rect bounds, string str, Id id)
: WinBase(pw, null, bounds.x, bounds.y, (bounds.w > 0? bounds.w : draw_ttf->text_width(str.c_str())), (bounds.h > 0? bounds.h : TTF_FontHeight(draw_ttf->ttf_font)), 0, id),
  txt(str), textRenderer(draw_ttf)
{}

LabelWin::~LabelWin(){}

void LabelWin::draw()
{
	this->init_gui();
	SDL_FillRect(this->win, 0, this->parent->bgcol);
	this->textRenderer->draw_string(this->win, this->txt.c_str(), Point(0, 0));
}

void LabelWin::setTextRenderer(RenderText* txtrnd)
{
	this->textRenderer = txtrnd;
	this->widen(this->tw_area.w - txtrnd->text_width(this->txt.c_str()), this->tw_area.h - TTF_FontHeight(txtrnd->ttf_font)); //fixme this overrides user-specified size
}
