#include "main.h"
#include "draw.h"

static eventproc current_background;
static eventproc current_layer;

int draw::headof(int& x, int y, int& width, const char* format) {
	auto dy = header(x, y, width, format);
	x += metrics::padding;
	width -= metrics::padding * 2;
	return dy;
}

int draw::header(int x, int y, int width, const char* format) {
	auto pf = font; font = metrics::h3;
	auto pc = fore; fore = colors::white;
	rect rc = {x, y, x + width, y + texth() + metrics::padding * 2};
	gradv(rc, colors::black, colors::black.mix(colors::window, 192));
	text(x + metrics::padding + (width - textw(format)) / 2, y + metrics::padding, format);
	font = pf;
	fore = pc;
	return rc.height();
}

int	draw::detail(int x, int y, int width, const char* format) {
	return textf(x, y, width, format);
}

int	draw::detail(int x, int y, int width, const char* format, int width_right, int v1) {
	char temp[16]; stringbuilder sb(temp); sb.add("%1i", v1);
	return detail(x, y, width, format, width_right, temp);
}

int	draw::detail(int x, int y, int width, const char* format, int width_right, int v1, int v2) {
	char temp[16]; stringbuilder sb(temp); sb.add("%1i/%2i", v1, v2);
	return detail(x, y, width, format, width_right, temp);
}

int	draw::detail(int x, int y, int width, const char* format, int width_right, const char* text_value) {
	auto d1 = textf(x, y, width - width_right - metrics::padding, format);
	auto pc = fore; fore = fore.mix(colors::window, 192);
	text(x + width - width_right + (width_right - textw(text_value)) / 2, y, text_value);
	fore = pc;
	return d1;
}

int	draw::detaih(int x, int y, int width, const hotkey* pk) {
	auto x0 = x;
	for(auto p = pk; *p; p++)
		x += button(x, y, p->name, p->key, p->proc);
	return x - x0;
}

static int buttonr(int x, int y, int w1, const char* key) {
	auto w = draw::textw(key);
	if(w1 == -1)
		w1 = w;
	rect rc = {x - 2, y - 1, x + w1 + 2, y + draw::texth()};
	draw::rectf(rc, colors::button);
	draw::rectb(rc, colors::border.mix(colors::form));
	draw::text(x + (w1 - w)/2, y, key);
	return w1 + 5;
}

int	draw::button(int x, int y, const char* format, const char* key, eventproc proc) {
	auto x0 = x;
	x += buttonr(x, y, -1, key);
	if(format) {
		int max_width = 0;
		textf(x, y, 200, format, &max_width);
		x += max_width + metrics::padding*2;
	}
	return x - x0;
}

void draw::setbackground(eventproc proc) {
	current_background = proc;
}

void draw::setnextlayer(eventproc proc) {
	current_layer = proc;
}

int	draw::widget(eventproc before, eventproc after) {
	while(ismodal()) {
		if(current_background)
			current_background();
		if(before)
			before();
		domodal();
		if(after)
			after();
	}
	return getresult() != 0;
}

void draw::layer() {
	while(current_layer) {
		auto proc = current_layer;
		current_layer = 0;
		proc();
	}
}