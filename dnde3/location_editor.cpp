#include "main.h"

using namespace draw;

static location*	current_location;
static indext		current_index;
static tile_s		current_tile = Sea;

static int render_info(int x, int y, int width) {
	auto y0 = y;
	char temp[512]; stringbuilder sb(temp);
	auto tile = current_location->gettile(current_index);
	sb.adds("Это %1.", getstr(tile));
	sb.adds("Кординаты %1i,%2i (индекс %3i).",
		current_location->getx(current_index), current_location->gety(current_index), current_index);
	y += detail(x, y, width, sb);
	return y - y0;
}

static void help() {

}

static void put_tile() {
	current_location->set(current_index, current_tile);
}

static void choose_tile_1() {
	current_tile = Plain;
}

static void choose_tile_2() {
	current_tile = Sea;
}

static void choose_tile_3() {
	current_tile = Foothills;
}

static void choose_tile_4() {
	current_tile = Mountains;
}

static void choose_tile_5() {
	current_tile = CloudPeaks;
}

static void choose_tile_6() {
	current_tile = Forest;
}

static hotkey hotkeys[] = {/*{"# ", "Вывести текущий тайл", put_tile},*/
{"1", getstr(Plain), choose_tile_1},
{"2", getstr(Sea), choose_tile_2},
{"3", getstr(Foothills), choose_tile_3},
{"4", getstr(Mountains), choose_tile_4},
{"5", getstr(CloudPeaks), choose_tile_5},
{"6", getstr(Forest), choose_tile_6},
{}};

static int render_keys(int x, int y, int width) {
	auto x0 = x;
	char temp[260]; stringbuilder sb(temp);
	sb.add("Вывести [%1]", getstr(current_tile));
	x += button(x, y, temp, "# ", 0);
	x += detaih(x, y, width, hotkeys);
	return x - x0;
}

static void render_bottom(int x, int y, int width) {
	//y += detail(x, y, 160, "Выбрано", 100, getstr(current_tile));
	y += render_info(x, y, width);
	render_keys(x, y, width - 168); y += texth() + 2;
}

static void render_bottom() {
	auto w = 700;
	auto h = texth()*2;
	rect r1;
	r1.x1 = (getwidth() - w - gui_border * 2) / 2;
	r1.x2 = r1.x1 + w;
	r1.y1 = getheight() - h - gui_border * 2;
	r1.y2 = r1.y1 + h;
	window(r1, false);
	render_bottom(r1.x1, r1.y1, r1.width());
}

static void render() {
	picture effects[1]; effects[0].setcursor(current_index, 1);
	current_location->worldmap(getcamera(), true);
	render(effects);
	render_bottom();
}

static void controls() {
	current_index = translate(current_index);
	shortcuts(hotkeys);
}

void location::editor() {
	current_location = this;
	setbackground(render);
	widget(0, controls);
}