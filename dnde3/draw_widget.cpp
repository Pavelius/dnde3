#include "main.h"
#include "draw.h"

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

int	draw::widget(widgetproc before, widgetproc after) {
	while(ismodal()) {
		before();
		domodal();
		after();
	}
	return getresult() != 0;
}