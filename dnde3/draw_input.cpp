#include "main.h"
#include "draw.h"

using namespace draw;

static bool		break_modal;
static int		break_result;
eventproc		draw::domodal;

void draw::execute(eventproc proc, int param) {
	domodal = proc;
	hot.key = 0;
	hot.param = param;
}

void draw::breakmodal(int result) {
	break_modal = true;
	break_result = result;
}

void draw::buttoncancel() {
	breakmodal(0);
}

void draw::buttonok() {
	breakmodal(1);
}

int draw::getresult() {
	return break_result;
}

static void standart_domodal() {
	hot.key = draw::rawinput();
	switch(hot.key) {
	case 0:
		exit(0);
		break;
	}
}

bool draw::ismodal() {
	hot.cursor = CursorArrow;
	domodal = standart_domodal;
	if(!break_modal)
		return true;
	break_modal = false;
	return false;
}

static void setdark() {
	colors::active = color::create(172, 128, 0);
	colors::border = color::create(73, 73, 80);
	colors::button = color::create(0, 122, 204);
	colors::form = color::create(32, 32, 32);
	colors::window = color::create(64, 64, 64);
	colors::text = color::create(255, 255, 255);
	colors::edit = color::create(38, 79, 120);
	colors::h1 = colors::text.mix(colors::edit, 64);
	colors::h2 = colors::text.mix(colors::edit, 96);
	colors::h3 = colors::text.mix(colors::edit, 128);
	colors::special = color::create(255, 244, 32);
	colors::border = colors::window.mix(colors::text, 128);
	colors::tips::text = color::create(255, 255, 255);
	colors::tips::back = color::create(100, 100, 120);
	colors::tabs::back = color::create(255, 204, 0);
	colors::tabs::text = colors::black;
}

void draw::initialize() {
	setdark();
	draw::font = metrics::font;
	draw::fore = colors::text;
	draw::fore_stroke = colors::blue;
}

static void breakparam() {
	breakmodal(hot.param);
}

int answeri::choosev(bool interactive, bool clear_text, bool return_single, const char* format) const {
	return 0;
}

void gamei::intialize() {
	draw::initialize();
	draw::create(-1, -1, 800, 600, WFResize|WFMinmax, 32);
}

const int		gui_border = 8;
const int		gui_padding = 4;

static areas window(rect rc, bool disabled = false, int border = 0) {
	if(border == 0)
		border = gui_border;
	rc.offset(-border, -border);
	color c = colors::form;
	color b = colors::form;
	auto rs = draw::area(rc);
	auto op = 192;
	if(disabled)
		op = op / 2;
	draw::rectf(rc, c, op);
	draw::rectb(rc, b);
	return rs;
}

static int windowf(int x, int y, int width, const char* string) {
	rect rc = {x, y, x + width, y};
	draw::state push;
	draw::font = metrics::font;
	auto height = draw::textf(rc, string);
	rc.x2 = rc.x1 + width;
	window(rc, false);
	draw::textf(x, y, rc.width(), string);
	return height + gui_border * 2 + gui_padding;
}