#include "main.h"
#include "draw.h"

using namespace draw;

namespace draw {
struct hotkey {
	int				key;
	const char*		name;
	vproc			proc;
	int				param;
	explicit operator bool() const { return key != 0; }
};
struct hotkeym {
	int				key;
	direction_s		direction;
	explicit operator bool() const { return key != 0; }
};
}

namespace colors {
color				fow;
}
const int			gui_border = 8;
const int			gui_padding = 4;
const int			mmaps = 5;
static bool			show_gui_panel = true;
static eventproc	current_background;
static indext		current_index;
static tile_s		current_tile = Sea;
static bool			break_modal;
static int			break_result;
eventproc			draw::domodal;
const int			elx = 64; // Map tile element width
const int			ely = 48; // Map tile element height
const int			scrx = 32;
const int			scry = 32;
static point		viewport;
static point		camera;
static char			message_text[1024];
stringbuilder		sb(message_text);
answeri				an;

struct imgi {
	const char*		name;
	const char*		folder;
	const sprite*	data;
	bool			notfound;
};
imgi bsmeta<imgi>::elements[] = {{""},
{"blood", "art"},
{"grass", "art"},
{"grass_w", "art"},
{"dungeon", "art"},
{"dungeon_w", "art"},
{"shadow", "art"},
{"road", "art"},
{"water", "art"},
{"monsters", "art"},
{"items", "art"},
{"doors", "art"},
{"fog", "art"},
{"features", "art"},
{"sea", "art"},
{"plain", "art"},
{"hills", "art"},
{"mount", "art"},
{"tmount", "art"},
{"decals", "art"},
{"ui", "art"},
{"pcmar", "art"},
{"pcmbd", "art"},
{"pcmac", "art"},
};
assert_enum(img, ResPCmac);

static hotkeym move_keys[] = {{KeyLeft, Left},
{KeyHome, LeftUp},
{KeyUp, Up},
{KeyPageUp, RightUp},
{KeyRight, Right},
{KeyPageDown, RightDown},
{KeyDown, Down},
{KeyEnd, LeftDown},
};

static const sprite* gres(img_s i) {
	auto& e = bsmeta<imgi>::elements[i];
	if(e.notfound)
		return 0;
	if(!e.data) {
		char temp[260]; stringbuilder sb(temp);
		if(e.folder) {
			sb.add(e.folder);
			sb.add("/");
		}
		sb.add("%1.pma", e.name);
		e.data = (sprite*)loadb(temp);
		if(!e.data)
			e.notfound = true;
	}
	return bsmeta<imgi>::elements[i].data;
}

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
		//loc.write("overland.map");
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

static void no_background() {
	rectf({0, 0, getwidth(), getheight()}, colors::window);
}

void gamei::intialize() {
	draw::initialize();
	draw::create(-1, -1, 800, 600, WFResize | WFMinmax, 32);
	current_background = no_background;
}

static indext gets2i(const point& camera) {
	return location::get(camera.x / elx, camera.y / ely);
}

static void window(rect rc, bool disabled, int border) {
	if(border == 0)
		border = gui_border;
	rc.offset(-border, -border);
	color c = colors::form;
	color b = colors::form;
	auto op = 192;
	if(disabled)
		op = op / 2;
	draw::rectf(rc, c, op);
	draw::rectb(rc, b);
}

static int buttonr(int x, int y, int w1, const char* name) {
	auto w = textw(name);
	if(w1 == -1)
		w1 = w;
	rect rc = {x - 2, y - 1, x + w1 + 2, y + texth()};
	rectf(rc, colors::button);
	rectb(rc, colors::border.mix(colors::form));
	text(x + (w1 - w) / 2, y, name);
	return w1 + 5;
}

static void windowf(const char* string, const char* press_key) {
	rect rc = {0, 0, 400, 0};
	draw::textf(rc, string);
	if(press_key)
		rc.y2 += texth() + 4;
	auto w = rc.width();
	auto h = rc.height();
	rc.x1 = (getwidth() - w) / 2;
	rc.y1 = gui_padding * 3;
	rc.x2 = rc.x1 + w;
	rc.y2 = rc.y1 + h;
	window(rc, false, 0);
	draw::textf(rc.x1, rc.y1, w, string);
	if(press_key)
		buttonr(rc.x1, rc.y2 - texth(), -1, press_key);
}

static void dialogw(int& x, int& y, int width, int height, const char* title, int* y1 = 0) {
	rect rc;
	rc.x1 = (getwidth() - width) / 2;
	rc.y1 = 30;
	rc.x2 = rc.x1 + width;
	rc.y2 = rc.y1 + height;
	window(rc, false, 0);
	draw::state push;
	font = metrics::h1;
	fore = colors::special;
	auto tw = textw(title);
	x = rc.x1; y = rc.y1;
	text((getwidth() - tw) / 2, y, title);
	if(y1) {
		auto& fr = metrics::font->get(metrics::font->glyph('A'));
		*y1 = y + height - fr.sy - 2;
	}
	y += texth() + 2;
}

static void correct(point& camera) {
	auto mx = mmx * elx;
	auto my = mmy * ely;
	if(camera.x < 0)
		camera.x = 0;
	if(camera.y < 0)
		camera.y = 0;
	if(camera.x > mx - viewport.x)
		camera.x = mx - viewport.x;
	if(camera.y > my - viewport.y)
		camera.y = my - viewport.y;
}

void location::worldmap(point camera, bool show_fow) const {
	auto decals = gres(ResDecals);
	viewport.x = draw::getwidth();
	viewport.y = draw::getheight();
	correct(camera);
	rect rc;
	rc.x1 = camera.x / elx;
	rc.y1 = camera.y / ely;
	rc.x2 = rc.x1 + viewport.x / elx + 2;
	rc.y2 = rc.y1 + viewport.y / ely + 3;
	int x0 = 0;
	int y0 = 0;
	// Нижний уровень
	for(auto my = rc.y1; my <= rc.y2; my++) {
		if(my < 0)
			continue;
		if(my >= mmy)
			break;
		for(auto mx = rc.x1; mx <= rc.x2; mx++) {
			if(mx < 0 || mx >= mmx)
				continue;
			auto i = get(mx, my);
			auto x = x0 + mx * elx - camera.x;
			auto y = y0 + my * ely - camera.y;
			auto t = gettile(i);
			auto r = getrand(i) % 4;
			switch(t) {
			case Plain:
			case Mountains:
			case Forest:
			case Swamp:
				draw::image(x, y, gres(ResPlains), r, 0);
				break;
			case Sea:
				draw::image(x, y, gres(ResSea), getindex(i, Sea), 0);
				break;
			case Foothills:
				draw::image(x, y, gres(ResPlains), r, 0);
				draw::image(x, y, gres(ResFoothills), getindex(i, Foothills), 0);
				break;
			}
		}
	}
	// Средний уровень
	for(auto my = rc.y1; my <= rc.y2; my++) {
		if(my < 0)
			continue;
		if(my >= mmy)
			break;
		for(auto mx = rc.x1; mx <= rc.x2; mx++) {
			if(mx < 0 || mx >= mmx)
				continue;
			auto x = x0 + mx * elx - camera.x;
			auto y = y0 + my * ely - camera.y;
			auto i = get(mx, my);
			auto t = gettile(i);
			auto r = getrand(i) % 4;
			switch(t) {
			case Mountains:
				draw::image(x, y, gres(ResMountains), getindex(i, Mountains), 0);
				break;
			case CloudPeaks:
				draw::image(x, y, gres(ResCloudPeaks), getindex(i, CloudPeaks), 0);
				break;
			case Forest:
				draw::image(x, y, decals, 0 + (getrand(i) % 3), 0);
				break;
			case Swamp:
				draw::image(x, y, decals, 3 + (getrand(i) % 3), 0);
				break;
			}
		}
	}
}

static bool controlmap() {
	switch(hot.key) {
	case KeyLeft: camera.x -= mmx; break;
	case KeyRight: camera.x += mmx; break;
	case KeyUp: camera.y -= mmy; break;
	case KeyDown: camera.y += mmy; break;
	case KeyHome: camera.x -= mmx; camera.y -= mmy; break;
	case KeyPageUp: camera.x += mmx; camera.y -= mmy; break;
	case KeyEnd: camera.x -= mmx; camera.y += mmy; break;
	case KeyPageDown: camera.x += mmx; camera.y += mmy; break;
	default: return false;
	}
	correct(camera);
	return true;
}

static void correct(point& camera, indext i1) {
	auto x = location::getx(i1);
	auto y = location::gety(i1);
	auto x1 = camera.x / elx;
	auto x2 = x1 + viewport.x / elx;
	auto y1 = camera.y / ely;
	auto y2 = y1 + viewport.y / ely;
	if(x < x1)
		camera.x = x * elx;
	if(x > x2 - 1)
		camera.x = (x - viewport.x / elx)*elx;
	if(y < y1)
		camera.y = y * ely;
	if(y > y2 - 1)
		camera.y = (y - viewport.y / ely)*ely;
	correct(camera);
}

static indext translate(indext i) {
	indext i1 = Blocked;
	switch(hot.key) {
	case KeyLeft: i1 = location::to(i, Left); break;
	case KeyRight: i1 = location::to(i, Right); break;
	case KeyUp: i1 = location::to(i, Up); break;
	case KeyDown: i1 = location::to(i, Down); break;
	case KeyHome: i1 = location::to(i, LeftUp); break;
	case KeyPageUp: i1 = location::to(i, RightUp); break;
	case KeyEnd: i1 = location::to(i, LeftDown); break;
	case KeyPageDown: i1 = location::to(i, RightDown); break;
	default: return i;
	}
	if(i1 == Blocked)
		return i;
	hot.key = 0;
	correct(camera, i1);
	return i1;
}

void picture::set(short x, short y) {
	this->x = x * elx;
	this->y = y * ely;
}

void picture::render(int x, int y) const {
	image(this->x + x, this->y + y, gres(img), frame, flags, alpha);
}

static void render(aref<picture> source) {
	for(auto& e : source)
		e.render(-camera.x, -camera.y);
}

static int header(int x, int y, int width, const char* format) {
	auto pf = font; font = metrics::h3;
	auto pc = fore; fore = colors::white;
	rect rc = {x, y, x + width, y + texth() + metrics::padding * 2};
	gradv(rc, colors::black, colors::black.mix(colors::window, 192));
	text(x + metrics::padding + (width - textw(format)) / 2, y + metrics::padding, format);
	font = pf;
	fore = pc;
	return rc.height();
}

static int headof(int& x, int y, int& width, const char* format) {
	auto dy = header(x, y, width, format);
	x += metrics::padding;
	width -= metrics::padding * 2;
	return dy;
}

static int detail(int x, int y, int width, const char* format, int width_right, const char* text_value) {
	auto d1 = textf(x, y, width - width_right - metrics::padding, format);
	auto pc = fore; fore = fore.mix(colors::window, 192);
	text(x + width - width_right + (width_right - textw(text_value)) / 2, y, text_value);
	fore = pc;
	return d1;
}

static int detail(int x, int y, int width, const char* format) {
	return textf(x, y, width, format);
}

static int detail(int x, int y, int width, const char* format, int width_right, int v1) {
	char temp[16]; stringbuilder sb(temp); sb.add("%1i", v1);
	return detail(x, y, width, format, width_right, temp);
}

static int detail(int x, int y, int width, const char* format, int width_right, int v1, int v2) {
	char temp[16]; stringbuilder sb(temp); sb.add("%1i/%2i", v1, v2);
	return detail(x, y, width, format, width_right, temp);
}

static void getkeyname(stringbuilder& sb, int key) {
	*sb.get() = 0;
	if(key&Ctrl) {
		sb.add("Ctrl+");
		key &= ~Ctrl;
	}
	if(key&Alt) {
		sb.add("Alt+");
		key &= ~Alt;
	}
	if(key&Shift) {
		sb.add("Shift+");
		key &= ~Shift;
	}
	char temp[2];
	switch(key) {
	case KeyEscape: sb.add("Esc"); break;
	case KeySpace: sb.add("Space"); break;
	case KeyDown: sb.add("Вниз"); break;
	case KeyUp: sb.add("Вверх"); break;
	case KeyLeft: sb.add("Лево"); break;
	case KeyRight: sb.add("Право"); break;
	default:
		temp[0] = key - Alpha;
		temp[1] = 0;
		sb.add(temp);
		break;
	}
}

static int buttonr(int x, int y, int w1, int key) {
	char temp[32]; stringbuilder sb(temp);
	getkeyname(sb, key);
	auto w = textw(temp);
	if(w1 == -1)
		w1 = w;
	rect rc = {x - 2, y - 1, x + w1 + 2, y + texth()};
	rectf(rc, colors::button);
	rectb(rc, colors::border.mix(colors::form));
	text(x + (w1 - w) / 2, y, temp);
	return w1 + 5;
}

static bool button(int x, int y, const char* format, int key, int* width) {
	auto x0 = x;
	x += buttonr(x, y, -1, key);
	if(format) {
		int max_width = 0;
		textf(x, y, 200, format, &max_width);
		x += max_width + metrics::padding * 2;
	}
	auto result = false;
	if(hot.key == key)
		result = true;
	if(width)
		*width = x - x0;
	return result;
}

static void button(int& x, int y, const char* format, int key, eventproc proc, int param) {
	auto w = 0;
	if(button(x, y, format, key, &w))
		execute(proc, param);
	x += w;
}

static int detaih(int x, int y, int width, const hotkey* pk) {
	auto x0 = x;
	for(auto p = pk; *p; p++) {
		auto w = 0;
		if(button(x, y, p->name, p->key, &w))
			execute(p->proc.pinp, p->param);
		x += w;
	}
	return x - x0;
}

static void setbackground(eventproc proc) {
	current_background = proc;
}

static int render_info(int x, int y, int width) {
	auto y0 = y;
	char temp[512]; stringbuilder sb(temp);
	auto tile = loc.gettile(current_index);
	sb.adds("Это %1.", getstr(tile));
	sb.adds("Кординаты %1i,%2i (индекс %3i).",
		loc.getx(current_index), loc.gety(current_index), current_index);
	y += detail(x, y, width, sb);
	return y - y0;
}

static void help() {

}

static void put_tile() {
	loc.set(current_index, current_tile);
}

static void choose_tile() {
	current_tile = (tile_s)hot.param;
}

static hotkey hotkeys[] = {{Alpha + '1', getstr(Plain), choose_tile, Plain},
{Alpha + '2', getstr(Sea), choose_tile, Sea},
{Alpha + '3', getstr(Foothills), choose_tile, Foothills},
{Alpha + '4', getstr(Mountains), choose_tile, Mountains},
{Alpha + '5', getstr(CloudPeaks), choose_tile, CloudPeaks},
{Alpha + '6', getstr(Forest), choose_tile, Forest},
{}};

static int render_keys(int x, int y, int width) {
	auto x0 = x;
	char temp[260]; stringbuilder sb(temp);
	sb.add("Вывести [%1]", getstr(current_tile));
	button(x, y, temp, KeySpace, put_tile, current_tile);
	x += detaih(x, y, width, hotkeys);
	return x - x0;
}

static void render_bottom(int x, int y, int width) {
	y += render_info(x, y, width);
	render_keys(x, y, width - 168); y += texth() + 2;
}

static void render_bottom() {
	auto w = 700;
	auto h = texth() * 2;
	rect r1;
	r1.x1 = (getwidth() - w - gui_border * 2) / 2;
	r1.x2 = r1.x1 + w;
	r1.y1 = getheight() - h - gui_border * 2;
	r1.y2 = r1.y1 + h;
	window(r1, false, 0);
	render_bottom(r1.x1, r1.y1, r1.width());
}

static void render_editor() {
	loc.worldmap(camera, true);
	picture effects[1]; effects[0].setcursor(current_index, 1);
	render(effects);
	render_bottom();
}

static void header(int x, int y, const char* name) {
	char temp[128]; stringbuilder sb(temp);
	draw::state push;
	sb.add("%1:", name);
	draw::text(x, y, sb);
}

static int fielw(int x, int y, int v1, int b1, const char* format = "%1i") {
	auto p_fore = fore;
	if(b1 > 0)
		fore = colors::text.mix(colors::red, 96);
	else if(b1 < 0)
		fore = colors::text.mix(colors::green, 64);
	char temp[32]; stringbuilder sb(temp);
	sb.add(format, v1, b1);
	draw::text(x, y, sb);
	fore = p_fore;
	return textw(sb);
}

static int fiela(int x, int y, int w, const char* name, int v1, int b1, const char* f1 = "%1i", int v2 = 0, int b2 = 0, const char* f2 = "%1i") {
	header(x, y, name); x += w;
	x += fielw(x, y, v1, b1, f1);
	if(v2) {
		text(x, y, " и ");
		x += textw(" и ");
		fielw(x, y, v2, b2, f2);
	}
	return draw::texth();
}

static int fielp(int x, int y, int w, const char* name, int p1, int b1, int p2, int b2) {
	header(x, y, name);
	char temp[128]; stringbuilder sb(temp);
	sb.add("%1i%%", p1);
	if(p2)
		sb.adds("и %1i%%", p2);
	draw::text(x + w, y, sb);
	return draw::texth();
}

static int fielr(int x, int y, int w, const char* name, int p1, int p2) {
	header(x, y, name);
	char temp[128]; stringbuilder sb(temp);
	sb.add("%1i%%", p1);
	if(p2)
		sb.add(" и %1i", p2);
	draw::text(x + w, y, temp);
	return draw::texth();
}

static int field(int x, int y, int w, const char* name, int value) {
	header(x, y, name);
	char temp[128]; stringbuilder sb(temp); sb.add("%1i", value);
	draw::text(x + w, y, sb);
	return draw::texth();
}

static int field(int x, int y, int w, const char* name, int v1, int v2) {
	header(x, y, name);
	char temp[128]; stringbuilder sb(temp); sb.add("%1i/%2i", v1, v2);
	draw::text(x + w, y, sb);
	return draw::texth();
}

static int texth(int x, int y, const char* text, char level) {
	static color color_level[] = {colors::text, colors::yellow, colors::red};
	draw::state push;
	if(level > 0)
		draw::fore = color_level[level];
	draw::text(x, y, text);
	return draw::textw(text) + 4;
}

static void render_info(const creature& e) {
	if(!show_gui_panel)
		return;
	char temp[512]; string sb(temp);
	const int tw = 26;
	const int dx = 52;
	const int width = 560;
	const int height = draw::texth() * 4 + 2;
	auto x = (draw::getwidth() - width) / 2;
	auto y = draw::getheight() - height - gui_padding * 3;
	auto y2 = y;
	draw::state push;
	fore = colors::white;
	window({x, y, x + width, y + height}, false, 0);
	sb.clear(); e.getfullname(sb);
	textf(x, y, width, sb);
	auto loc = e.getsite();
	if(loc) {
		sb.clear(); loc->getname(sb);
		draw::text(x + width - draw::textw(sb), y, sb);
	}
	y += draw::texth() + 2;
	int y1 = y;
	int x1 = x;
	y += fiela(x, y, tw, "СЛ", e.get(Strenght), e.getboost(Strenght));
	y += fiela(x, y, tw, "ИН", e.get(Intellegence), e.getboost(Intellegence));
	x += dx;
	y = y1;
	y += fiela(x, y, tw, "ЛВ", e.get(Dexterity), e.getboost(Dexterity));
	y += fiela(x, y, tw, "МД", e.get(Wisdow), e.getboost(Wisdow));
	x += dx;
	y = y1;
	y += fiela(x, y, tw, "ТЛ", e.get(Constitution), e.getboost(Constitution));
	y += fiela(x, y, tw, "ХР", e.get(Charisma), e.getboost(Charisma));
	x += dx + 6;
	y = y1;
	y += fiela(x, y, tw, "АТ", e.getattack(Melee).attack, e.getboost(Attack), "%1i%%", e.getattack(Ranged).attack, e.getboost(Attack), "%1i%%");
	y += fiela(x, y, tw, "БР", e.get(Protection), e.getboost(Protection), "%1i%%", e.get(Armor), e.getboost(Armor));
	x += dx + 50;
	y = y1;
	y += field(x, y, 40, "Хиты", e.gethits(), e.get(LifePoints));
	y += field(x, y, 40, "Мана", e.getmana(), e.get(ManaPoints));
	x += dx + 40 + 20 - tw;
	y = y1;
	y += field(x, y, 52, "Опыт", e.getexperience());
	y += field(x, y, 52, "Деньги", e.getmoney());
	x += dx + 58;
	y = y1;
	y += field(x, y, 52, "Время", game.getrounds());
	y += fiela(x, y, 52, "ПБ", e.get(Deflect), e.getboost(Deflect), "%1i%%");
	x = x1;
	y = y1 + draw::texth() * 2;
	// Draw encumbrance
	switch(e.getencumbrance()) {
	case Encumbered: x += texth(x, y, "Нагружен", 0); break;
	case HeavilyEncumbered: x += texth(x, y, "Нагружен", 1); break;
	}
	// Draw status
	for(auto i = Anger; i <= LastState; i = (state_s)(i + 1)) {
		if(!e.is(i))
			continue;
		auto pt = getstr(i);
		draw::text(x, y, pt);
		x += draw::textw(pt) + 4;
	}
}

static void render_message(const char* press_key = 0) {
	if(!message_text[0])
		return;
	windowf(message_text, press_key);
	sb.clear();
}

static void render_indoor() {
	picture effects[2] = {};
	if(current_index != Blocked)
		effects[0].setcursor(current_index, 1);
	loc.indoor(camera, true, effects);
	auto player = creature::getactive();
	if(player)
		render_info(*player);
}

static void controls() {
	current_index = translate(current_index);
}

void location::editor() {
	read("overland.map");
	setbackground(render_editor);
	while(ismodal()) {
		current_background();
		domodal();
		switch(hot.key) {
		case KeyEscape:
			breakmodal(0);
			break;
		default:
			current_index = translate(current_index);
			break;
		}
	}
}

static void breakparam() {
	breakmodal(hot.param);
}

static int mget(int ox, int oy, int mx, int my) {
	int x = mx - ox;
	int y = my - oy;
	if(x < 0 || y < 0 || x >= scrx || y >= scry)
		return Blocked;
	return y * scrx + x;
}

static int getorder(item_s type) {
	switch(type) {
	case Sling: return 2;
	default: return 0;
	}
}

static int getaccindex(item_s type) {
	switch(type) {
	case BowLong: return 0;
	case BowShort: return 1;
	case Sling: return 2;
	case CrossbowLight: case CrossbowHeavy: return 3;
	default: return -1;
	}
}

static void avatar(int x, int y, const creature& player, short unsigned flags, unsigned char alpha) {
	const int FirstWeapon = AxeBattle;
	int i1 = 0;
	int i2 = 1;
	const auto& ci = player.getclass();
	auto race = player.getrace();
	auto gender = player.getgender();
	auto melee = player.get(Melee);
	auto offhand = player.get(OffHand);
	auto ranged = player.get(Ranged);
	auto body = player.get(Torso);
	auto back = player.get(TorsoBack);
	if(melee.is(TwoHanded)) {
		i1 = 2;
		i2 = 3 + melee.getkind() - FirstWeapon;
	} else {
		if(melee.getkind())
			i1 = 3 + melee.getkind() - FirstWeapon;
		if(offhand.getkind() == Shield)
			i2 = 3 + (SwordTwoHanded - FirstWeapon + 1) + (SwordTwoHanded - FirstWeapon + 1);
		else if(offhand.getkind())
			i2 = 3 + (SwordTwoHanded - FirstWeapon + 1) + (offhand.getkind() - FirstWeapon);
	}
	auto at = 2;
	switch(body.getkind()) {
	case NoItem:
		at = ci.naked_avatar;
		break;
	case LeatherArmor:
	case StuddedLeatherArmor:
		at = 1;
		break;
	}
	// Cloack
	if(back) {
		switch(race) {
		case Dwarf:
		case Halfling:
			image(x, y, gres(ResPCmac), 5, flags, alpha);
			break;
		default:
			image(x, y, gres(ResPCmac), 4, flags, alpha);
			break;
		}
	}
	// Weapon on back
	if(ranged && getorder(ranged.getkind()) == 0) {
		auto index = getaccindex(ranged.getkind());
		if(index != -1)
			image(x, y, gres(ResPCmac), index, flags, alpha);
	}
	// Character body
	image(x, y, gres(ResPCmar), i1, flags, alpha);
	image(x, y, gres(ResPCmbd), (race - Human) * 6 + (gender - Male) * 3 + at, flags, alpha);
	// Weapon on belt
	if(ranged && getorder(ranged.getkind()) == 2) {
		auto index = getaccindex(ranged.getkind());
		if(index != -1)
			image(x, y, gres(ResPCmac), index, flags, alpha);
	}
	// Hand
	image(x, y, gres(ResPCmar), i2, flags, alpha);
}

bool location::xget(short unsigned i, direction_s direction) const {
	auto i1 = to(i, direction);
	if(i1 == Blocked)
		return false;
	return !is(i1, Explored);
}

bool location::wget(short unsigned i, direction_s direction, tile_s value) const {
	auto i1 = to(i, direction);
	if(i1 == Blocked)
		return true;
	return gettile(i1) == value;
}

bool location::wget(short unsigned i, direction_s direction, tile_s value, bool default_result) const {
	auto i1 = to(i, direction);
	if(i1 == Blocked)
		return default_result;
	return gettile(i1) == value;
}

void location::indoor(point camera, bool show_fow, const picture* effects) {
	creature* units[scrx*scry];
	auto night_percent = 0;
	auto is_dungeon = isdungeon();
	const sprite *floor, *walls;
	if(is_dungeon) {
		floor = gres(ResDungeon);
		walls = gres(ResDungeonW);
	} else {
		night_percent = (128 * 0) / 100;
		floor = gres(ResGrass);
		walls = gres(ResGrassW);
	}
	auto shadow = gres(ResShadow);
	auto doors = gres(ResDoors);
	viewport.x = draw::getwidth();
	viewport.y = draw::getheight();
	auto emmx = mmx * elx;
	auto emmy = mmy * ely;
	if(camera.x < 0)
		camera.x = 0;
	if(camera.y < 0)
		camera.y = 0;
	if(camera.x > emmx - viewport.x)
		camera.x = emmx - viewport.x;
	if(camera.y > emmy - viewport.y)
		camera.y = emmy - viewport.y;
	camera.x -= elx / 2;
	camera.y -= ely / 2;
	rect rc;
	rc.x1 = camera.x / elx;
	rc.y1 = camera.y / ely;
	rc.x2 = rc.x1 + viewport.x / elx + 2;
	rc.y2 = rc.y1 + viewport.y / ely + 3;
	int x0 = 0;
	int y0 = 0;
	// Инициализируем видимые юниты
	memset(units, 0, sizeof(units));
	for(auto& e : bsmeta<creature>()) {
		if(!e)
			continue;
		auto i0 = e.getposition();
		auto i = mget(rc.x1, rc.y1, getx(i0), gety(i0));
		if(i == Blocked)
			continue;
		units[i] = &e;
	}
	// Нижний уровень
	for(auto my = rc.y1; my <= rc.y2; my++) {
		if(my >= mmy)
			break;
		for(auto mx = rc.x1; mx <= rc.x2; mx++) {
			if(mx >= mmx)
				continue;
			auto x = x0 + mx * elx - camera.x;
			auto y = y0 + my * ely - camera.y;
			auto i = get(mx, my);
			auto t = gettile(i);
			auto o = getobject(i);
			auto r = getrand(i) % 4;
			// Тайлы
			switch(t) {
			case Hill:
				image(x, y, floor, r, 0);
				image(x, y, gres(ResFeature), 2, 0);
				break;
			case Swamp:
				image(x, y, floor, r, 0);
				image(x, y, gres(ResFeature), 3, 0);
				break;
			case Floor:
				if(is_dungeon)
					image(x, y, floor, r, 0);
				else
					image(x, y, floor, 4, 0);
				break;
			case Plain:
				image(x, y, floor, r, 0);
				break;
			case Wall:
				image(x, y, walls, 0, 0);
				if(wget(i, Down, Wall)) {
					if(wget(i, Right, Wall)) {
						if(!wget(i, RightDown, Wall))
							image(x, y, walls, 10, 0);
					} else if(wget(i, RightDown, Wall))
						image(x, y, walls, 12, 0);
					else
						image(x, y, walls, 2, 0);
					if(wget(i, Left, Wall)) {
						if(!wget(i, LeftDown, Wall))
							image(x, y, walls, 9, 0);
					} else if(wget(i, LeftDown, Wall))
						image(x, y, walls, 11, 0);
					else
						image(x, y, walls, 4, 0);
				} else {
					if(is_dungeon) {
						switch(i % 3) {
						case 0: image(x, y, walls, 13, 0); break;
						case 1: image(x, y, walls, 14, 0); break;
						default: image(x, y, walls, 1, 0); break;
						}
					} else
						image(x, y, walls, 1, 0);
					if(!wget(i, Left, Wall))
						image(x, y, walls, 8, 0);
					if(!wget(i, Right, Wall))
						image(x, y, walls, 7, 0);
				}
				break;
			case Water:
				image(x, y, gres(ResWater), getindex(i, Water), 0);
				break;
			case Road:
				image(x, y, gres(ResRoad), getindex(i, Road), 0);
				break;
			default:
				rectf({x - elx / 2, y - ely / 2, x + elx / 2, y + ely / 2}, colors::black);
				break;
			}
			// Объекты на земле
			switch(o) {
			case Trap:
				if(!is(i, Hidden))
					image(x, y, gres(ResFeature), 59 + gettrap(i) - TrapAnimal, 0);
				break;
			case Plants:
				image(x, y, gres(ResFeature), 36 + (getrand(i) / 60) % 3, 0);
				break;
			}
			// Кровь
			if(is(i, Blooded))
				image(x, y, gres(ResBlood), 12 + r, 0, 192);
		}
	}
	// Предметы на земле
	for(auto& e : bsmeta<itemground>()) {
		if(!e)
			continue;
		point pt;
		pt.x = getx(e.index);
		pt.y = gety(e.index);
		if(!pt.in(rc))
			continue;
		auto x = x0 + pt.x * elx - camera.x;
		auto y = y0 + pt.y * ely - camera.y;
		draw::image(x, y - 8, gres(ResItems), e.getkind(), 0);
	}
	// Нижний уровень эффектов
	if(effects) {
		for(auto p = effects; *p; p++)
			p->render(-camera.x, -camera.y);
	}
	// Тени
	for(auto my = rc.y1; my <= rc.y2; my++) {
		if(my >= mmy)
			break;
		for(auto mx = rc.x1; mx <= rc.x2; mx++) {
			if(mx >= mmx)
				continue;
			auto x = x0 + mx * elx - camera.x;
			auto y = y0 + my * ely - camera.y;
			auto i = get(mx, my);
			auto t = gettile(i);
			if(t != Wall) {
				bool uw = gettile(to(i, Up)) == Wall;
				bool rw = gettile(to(i, Right)) == Wall;
				bool dw = gettile(to(i, Down)) == Wall;
				bool lw = gettile(to(i, Left)) == Wall;
				if(uw)
					image(x, y, shadow, 0, 0);
				if(lw)
					image(x, y, shadow, 1, 0);
				if(dw)
					image(x, y, shadow, 2, 0);
				if(rw)
					image(x, y, shadow, 3, 0);
				if(!uw && !rw && gettile(to(i, RightUp)) == Wall)
					image(x, y, shadow, 4, 0);
				if(!uw && !lw && gettile(to(i, LeftUp)) == Wall)
					image(x, y, shadow, 5, 0);
				if(!dw && !rw && gettile(to(i, RightDown)) == Wall)
					image(x, y, shadow, 6, 0);
				if(!dw && !lw && gettile(to(i, LeftDown)) == Wall)
					image(x, y, shadow, 7, 0);
			}
			// Паутина
			if(is(i, Webbed))
				image(x, y, gres(ResFeature), 74 + getrand(i) % 3, 0);
		}
	}
	// Верхний уровень
	for(auto my = rc.y1; my <= rc.y2; my++) {
		if(my >= mmy)
			break;
		for(auto mx = rc.x1; mx <= rc.x2; mx++) {
			if(mx >= mmx)
				continue;
			auto x = x0 + mx * elx - camera.x;
			auto y = y0 + my * ely - camera.y;
			auto i = get(mx, my);
			auto t = getobject(i);
			auto r = getrand(i);
			switch(t) {
			case Door:
				r = is(i, Opened) ? 0 : 1;
				if(gettile(to(i, Left)) == Wall && gettile(to(i, Right)) == Wall)
					draw::image(x, y + 16, doors, r, 0);
				else if(gettile(to(i, Up)) == Wall && gettile(to(i, Down)) == Wall)
					draw::image(x, y, doors, 2 + r, 0);
				break;
			default:
				if(bsmeta<map_objecti>::elements[t].start) {
					auto c = bsmeta<map_objecti>::elements[t].count;
					if(c)
						image(x, y, gres(ResFeature), bsmeta<map_objecti>::elements[t].start + getrand(i) % c, 0);
					else
						image(x, y, gres(ResFeature), bsmeta<map_objecti>::elements[t].start, 0);
				}
				break;
			}
			// Вывод юнитов
			int i1 = mget(rc.x1, rc.y1, mx, my);
			if(i1 != -1) {
				auto pc = units[i1];
				if(pc && (loc.is(i, Visible) || pc->isactive())) {
					if(!pc->is(Invisible) || pc->is(Friendly) || pc->isactive()) {
						unsigned flags;
						switch(pc->getdirection()) {
						case Left:
						case LeftUp:
						case LeftDown:
							flags = ImageMirrorH;
							break;
						default:
							flags = 0;
							break;
						}
						// Показываем юниты только когда они видимы игроку
						unsigned char alpha = 0xFF;
						if(pc->is(Invisible))
							alpha = 0x80;
						if(pc->ischaracter())
							avatar(x, y, *pc, flags, alpha);
						else
							image(x, y, gres(ResMonsters), pc->getrole(), flags, alpha);
					}
				}
			}
			// Прозрачные стены
			if(gettile(i) != Wall) {
				// У границы внизу должны рисоваться только в подземелье
				if(wget(i, Down, Wall, is_dungeon)) {
					draw::image(x, y, walls, 3, 0);
					if(!wget(i, Left, Wall) && !wget(i, LeftDown, Wall))
						draw::image(x, y, walls, 6, 0);
					if(!wget(i, Right, Wall) && !wget(i, RightDown, Wall))
						draw::image(x, y, walls, 5, 0);
				}
			}
		}
	}
	// Ночной режим
	if(night_percent)
		rectf({0, 0, draw::getwidth(), draw::getheight()}, color::create(0, 0, 64), night_percent);
	// Show fog of war
	if(show_fow) {
		for(auto my = rc.y1; my <= rc.y2; my++) {
			if(my >= mmy)
				break;
			for(auto mx = rc.x1; mx <= rc.x2; mx++) {
				if(mx >= mmx)
					continue;
				auto x = x0 + mx * elx - camera.x;
				auto y = y0 + my * ely - camera.y - ely / 2;
				auto i = get(mx, my);
				if(!is(i, Explored))
					draw::rectf({x - elx / 2, y - ely / 2, x + elx / 2, y + ely / 2}, colors::fow);
				else {
					bool dw = xget(i, Down);
					bool up = xget(i, Up);
					bool rg = xget(i, Right);
					bool lf = xget(i, Left);
					if(dw)
						image(x, y, gres(ResFog), 2, 0);
					if(up)
						image(x, y, gres(ResFog), 2, ImageMirrorV);
					if(lf)
						image(x, y, gres(ResFog), 1, 0);
					if(rg)
						image(x, y, gres(ResFog), 1, ImageMirrorH);
					if(!lf && !up && xget(i, LeftUp))
						draw::image(x, y, gres(ResFog), 0, ImageMirrorV);
					if(!lf && !dw && xget(i, LeftDown))
						draw::image(x, y, gres(ResFog), 0, 0);
					if(!rg && !up && xget(i, RightUp))
						draw::image(x, y, gres(ResFog), 0, ImageMirrorV | ImageMirrorH);
					if(!rg && !dw && xget(i, RightDown))
						draw::image(x, y, gres(ResFog), 0, ImageMirrorH);
				}
			}
		}
	}
}

int	answeri::paint(int x, int y, int width, int i, int& maximum_width) const {
	auto k = Alpha + '1' + i;
	auto z = 22;
	char temp[2] = {(char)(k - Alpha), 0};
	buttonr(x, y, z - 6, k);
	auto& e = elements[i];
	auto h = texth();
	if(e.text) {
		auto h1 = textf(x + z, y, width - z, e.text, &maximum_width);
		if(h1 > h)
			h = h1;
	}
	if(hot.key == k)
		execute(breakparam, elements[i].param);
	return h;
}

int	answeri::paint(int x, int y, int width, const char* format, int& maximum_width) const {
	auto i = 0;
	auto z = y;
	if(format)
		y += textf(x, y, width, format, &maximum_width) + 4;
	for(auto& e : elements)
		y += paint(x, y, width, i++, maximum_width) + 2;
	return y - z;
}

int	answeri::choosev(bool interactive, bool clear_text, bool return_single, const char* format) const {
	auto w = 400;
	auto h = 0;
	if(true) {
		auto mw = 0;
		draw::state push;
		setclip({0, 0, 0, 0});
		h = paint(0, 0, 400, format, mw);
	}
	while(ismodal()) {
		current_background();
		rect rc = {(getwidth() - w) / 2, 32, (getwidth() + w) / 2, 32 + h};
		window(rc, false, 0);
		auto maximum_width = 0;
		paint(rc.x1, rc.y1, rc.width(), format, maximum_width);
		domodal();
	}
	return getresult() != 0;
}

int	answeri::dialogv(bool allow_cancel, const char* title, const char* format) const {
	int x, y;
	const int width = 600;
	while(ismodal()) {
		current_background();
		dialogw(x, y, width, 440, title);
		auto maximum_width = 0;
		paint(x, y, width, format, maximum_width);
		domodal();
		if(allow_cancel && hot.key == KeyEscape)
			breakmodal(0);
	}
	return getresult();
}

static void render_item(int x, int y, int width, const item& e) {
	if(!e)
		return;
	auto ps_fore = fore;
	if(e.is(KnownMagic)) {
		switch(e.getmagic()) {
		case Cursed: fore = colors::red; break;
		case Blessed: fore = colors::green; break;
		case Artifact: fore = colors::yellow; break;
		}
	}
	char temp[260]; stringbuilder sb(temp);
	e.getname(sb, false);
	szupper(temp, 1);
	text(x, y, temp);
	if(e.is(KnownStats)) {
		sb.clear();
		e.getstatistic(sb);
		text(x + width - 160, y, temp);
	}
	fore = ps_fore;
	//line(x + width, y, x + width, y + 10, colors::red);
}

static void render_weight(int x, int y, int width, const item& e) {
	if(!e)
		return;
	char temp[64]; stringbuilder sb(temp);
	auto v = e.getweight();
	sb.add("%1i.%2i кг", v / 100, (v / 10) % 10);
	auto w = textw(temp);
	text(x + width - w, y, temp);
}

static void render(int& x, int y, int width, const item& e, slot_mode_s mode) {
	if(mode == NoSlotName)
		return;
	auto p_fore = fore;
	fore = colors::h3.mix(colors::text);// colors::text.mix(colors::special, 212);
	auto slot = e.getwearerslot();
	const char* name = 0;
	if(slot >= Head && slot <= Amunitions) {
		if(mode == SlotWhere)
			name = bsmeta<sloti>::elements[slot].name_where;
		else
			name = bsmeta<sloti>::elements[slot].name;
	}
	if(name)
		text(x, y, name);
	if(mode != NoSlotName)
		x += width;
	fore = p_fore;
}

static int render(int x, int y, int width, itema& e) {
	char temp[512]; stringbuilder sb(temp);
	e.footer(sb);
	if(sb)
		return textf(x, y, width, sb);
	return 0;
}

static int text(int x, int y, int width, const char* value) {
	text(x, y, value);
	return width + 8;
}

static int text(int x, int y, int width, int value, const char* format = "%1i") {
	char temp[32]; stringbuilder sb(temp);
	sb.add(format, value);
	text(x + width - textw(temp), y, temp);
	return width + 8;
}

static int text(int x, int y, int width, dicei v, const char* format = "+ %1i-%2i") {
	char temp[32]; stringbuilder sb(temp);
	sb.add(format, v.min, v.max);
	text(x + width - textw(temp), y, temp);
	return width + 8;
}

skill_s skillu::choose(bool interactive, const char* title, bool* cancel_result) const {
	int x, y, y1;
	const int width = 400;
	while(ismodal()) {
		current_background();
		dialogw(x, y, width, 300, title, &y1);
		auto x1 = x;
		auto x2 = x + width;
		if(cancel_result)
			button(x1, y1, "Отмена", KeyEscape, breakparam, 0);
		auto index = 0;
		for(auto e : *this) {
			auto x0 = x;
			if(button(x0, y, 0, Alpha + answeri::getkey(index), 0))
				execute(breakparam, (int)e);
			x0 += 22;
			if((index + 1) % 2)
				rectf({x, y, x + width, y + texth() + 1}, colors::white, 4);
			text(x0, y, getstr(e)); x0 += 220;
			x0 += text(x0, y, 36, player->get(e), "%1i%%");
			if(cancel_result)
				x0 += text(x0, y, 100, player->getlevelname(e));
			else {
				x0 += text(x0, y, 36, player->getraise(e));
				x0 += text(x0, y, 64, getcap(e), " макс. %1i%%");
			}
			index++;
			y += texth() + 4;
		}
		y += texth();
		domodal();
	}
	return (skill_s)getresult();
}

item* itema::choose(const char* interactive, const char* title, const char* format, slot_mode_s mode, bool show_always) {
	if(!count) {
		if(interactive) {
			if(!show_always) {
				sb.add(interactive);
				return 0;
			}
		} else
			return 0;
	}
	if(!interactive)
		return data[rand() % count];
	const int width = 600;
	while(ismodal()) {
		current_background();
		int x, y, y1;
		dialogw(x, y, width, 440, title, &y1);
		auto x1 = x;
		auto x2 = x + width;
		if(format)
			y += textf(x, y, width, format);
		button(x1, y1, "Отмена", KeyEscape, breakparam, 0);
		if(count > 0) {
			auto index = 0;
			for(auto e : *this) {
				auto x0 = x;
				if(button(x0, y, 0, Alpha + answeri::getkey(index), 0))
					execute(breakparam, (int)e);
				x0 += 22;
				if((index + 1) % 2)
					rectf({x, y, x + width, y + texth() + 1}, colors::white, 4);
				render(x0, y, 110, *e, mode);
				render_item(x0, y, x2 - x0 - 72, *e);
				render_weight(x0, y, x2 - x0, *e);
				index++;
				y += texth() + 4;
			}
			y += texth();
		} else if(interactive)
			y += textf(x, y, width, interactive) + texth() + 4;
		render(x, y, width, *this);
		domodal();
	}
	return (item*)getresult();
}

int indexa::choose(bool interactive, const char* format) {
	if(!count)
		return -1;
	if(!interactive)
		return rand() % count;
	auto index = 0;
	while(ismodal()) {
		if(current_index != data[index]) {
			current_index = data[index];
			loc.setcamera(current_index);
		}
		current_background();
		if(true) {
			char temp[512]; string sb(temp);
			loc.addinfo(current_index, sb);
			sb.adds(format);
			if(sb)
				windowf(sb, 0);
		}
		domodal();
		switch(hot.key) {
		case KeyEnter:
		case KeySpace:
			breakmodal(index);
			break;
		case KeyLeft:
			if(index > 0)
				index--;
			break;
		case KeyRight:
			if(index < getcount() - 1)
				index++;
			break;
		}
	}
	current_index = Blocked;
	return getresult();
}

indext location::choose(bool allow_cancel) {
	current_index = gets2i(camera);
	setbackground(render_indoor);
	while(ismodal()) {
		current_background();
		if(true) {
			char temp[512]; string sb(temp);
			addinfo(current_index, sb);
			if(sb)
				windowf(sb, 0);
		}
		domodal();
		switch(hot.key) {
		case KeyEscape:
			if(allow_cancel)
				breakmodal(Blocked);
			break;
		case KeyEnter:
		case KeySpace:
			breakmodal(current_index);
			break;
		default:
			current_index = translate(current_index);
			break;
		}
	}
	return getresult();
}

spell_s spella::choose(const char* interactive, const char* title, bool* cancel_result, const creature* player) const {
	if(!getcount()) {
		if(interactive)
			sb.add(interactive);
		if(cancel_result)
			*cancel_result = true;
		return (spell_s)0;
	}
	int x, y, y1;
	const int width = 400;
	while(ismodal()) {
		current_background();
		dialogw(x, y, width, 300, title, &y1);
		auto x1 = x;
		auto x2 = x + width;
		if(cancel_result)
			button(x1, y1, "Отмена", KeyEscape, breakparam, -1);
		auto index = 0;
		for(auto e : *this) {
			auto x0 = x;
			if(button(x0, y, 0, Alpha + answeri::getkey(index), 0))
				execute(breakparam, (int)e);
			x0 += 22;
			if((index + 1) % 2)
				rectf({x, y, x + width, y + texth() + 1}, colors::white, 4);
			text(x0, y, getstr(e)); x0 += 220;
			if(player)
				x0 += text(x0, y, 46, player->get(e), "%1i ранг");
			x0 += text(x0, y, 100, bsmeta<spelli>::elements[e].mp, "%1i маны");
			index++;
			y += texth() + 4;
		}
		y += texth();
		domodal();
	}
	auto r = getresult();
	if(r == -1) {
		if(cancel_result)
			*cancel_result = true;
	}
	return (spell_s)r;
}

static void pixel(int x, int y, color c1) {
	color* p = (color*)draw::ptr(x, y);
	for(int j = 0; j < mmaps; j++) {
		for(int i = 0; i < mmaps; i++)
			p[i] = c1;
		p += draw::canvas->scanline / sizeof(color);
	}
}

void location::minimap(int x, int y, point camera) const {
	if(x < 8)
		x = 8;
	if(y < 64)
		y = 64;
	int y3 = y;
	auto is_dungeon = isdungeon();
	color floor = colors::gray;
	if(!is_dungeon)
		floor = color::create(51, 140, 29);
	color floor1 = floor.darken();
	color object = colors::gray;
	if(!is_dungeon)
		object = floor1.darken();
	color wall = colors::black.lighten();
	color door = colors::red;
	color stairs = colors::red.darken().darken();
	color water = colors::blue;
	color road = color::create(94, 70, 51).mix(floor, 192);
	color border = colors::white;
	for(int y1 = 0; y1 < mmy; y1++) {
		int x3 = x;
		for(int x1 = 0; x1 < mmx; x1++, x3 += mmaps) {
			auto i = get(x1, y1);
			if(!is(i, Explored))
				continue;
			switch(gettile(i)) {
			case Hill:
			case Foothills:
			case Swamp:
				pixel(x3, y3, object);
				break;
			case Plain:
				pixel(x3, y3, floor);
				break;
			case Floor:
				pixel(x3, y3, floor1);
				break;
			case Water:
			case Sea:
				pixel(x3, y3, water);
				break;
			case Wall:
			case CloudPeaks:
				pixel(x3, y3, wall);
				break;
			case Road:
				pixel(x3, y3, road);
				break;
			}
			switch(getobject(i)) {
			case Door:
				pixel(x3, y3, door);
				break;
			case Tree:
				pixel(x3, y3, object);
				break;
			case StairsDown:
			case StairsUp:
				pixel(x3, y3, stairs);
				break;
			}
		}
		y3 += mmaps;
	}
	// Camera rectangle
	int mmx1 = mmx * mmaps;
	int mmy1 = mmy * mmaps;
	int scx = mmx * elx;
	int scy = mmy * ely;
	int xs1 = camera.x;
	int ys1 = camera.y;
	if(xs1 < 0)
		xs1 = 0;
	if(ys1 < 0)
		ys1 = 0;
	if(xs1 + viewport.x > scx)
		xs1 = scx - viewport.x;
	if(ys1 + viewport.y > scy)
		ys1 = scy - viewport.y;
	rect cm;
	cm.x1 = x + xs1 * mmx1 / scx;
	cm.y1 = y + ys1 * mmy1 / scy;
	cm.x2 = x + (xs1 + viewport.x)*mmx1 / scx;
	cm.y2 = y + (ys1 + viewport.y)*mmy1 / scy;
	draw::rectb(cm, border);
	//view_legends(x, y, map::size.x*mmaps + metrics::padding);
}

void location::minimap(indext index) const {
	char temp[128]; stringbuilder sb(temp);
	int w = mmx * mmaps + 280;
	int h = mmy * mmaps;
	point camera = {getx(index)*elx - viewport.x / 2, gety(index)*ely - viewport.y / 2};
	while(ismodal()) {
		draw::rectf({0, 0, draw::getwidth(), draw::getheight()}, colors::form);
		if(loc.level)
			sb.add("Уровень %1i", loc.level);
		//view_dialog(bsgets(Minimap, Name), temp, 1);
		loc.minimap((draw::getwidth() - w) / 2, (draw::getheight() - h) / 2, camera);
		domodal();
		switch(hot.key) {
		case KeyEscape:
		case KeySpace:
			breakmodal(0);
			break;
		}
	}
}

void location::setcamera(short x, short y) {
	camera.x = x*elx - getwidth() / 2 + elx / 2;
	camera.y = y*ely - getheight() / 2 + ely / 2;
	correct(camera);
}

static void change_player() {
	auto n = hot.param;
	auto pn = creature::getactive(n);
	auto po = creature::getactive();
	if(pn != po && pn) {
		po->wait();
		pn->activate();
	}
}

static hotkey adventure_keys[] = {{F1, "Выбрать первого героя", change_player, 0},
{F2, "Выбрать второго героя", change_player, 1},
{F3, "Выбрать третьего героя", change_player, 2},
{F4, "Выбрать четвертого героя", change_player, 3},
{Ctrl + Alpha + 'M', "Открыть мануал", gamei::help},
{Alpha + 'I', "Открыть инвентарь", &creature::inventory},
{Alpha + 'A', "Выбрать навык", &creature::useskills},
{Alpha + 'D', "Положить пердмет", &creature::dropdown},
{Alpha + 'P', "Поднять пердмет", &creature::pickup},
{Alpha + 'Q', "Стрелять по врагу", &creature::shoot},
{Alpha + 'V', "Рюкзак", &creature::backpack},
{Alpha + 'M', "Карта местности", &creature::minimap},
{Alpha + 'S', "Создать заклинание", &creature::usespells},
{Ctrl + Alpha + 'D', "Выпить что-то", &creature::drink},
{Ctrl + Alpha + 'E', "Чъесть что-то", &creature::eat},
{Ctrl + Alpha + 'B', "Поработить для отладки", &creature::enslave},
{Ctrl + Alpha + 'W', "Тестировать оружие", &creature::testweapons},
{}};

static bool translate_move(creature* player) {
	for(auto& e : move_keys) {
		if(e.key == hot.key) {
			auto ni = location::to(player->getposition(), e.direction);
			player->move(ni);
			breakmodal(0);
			return true;
		}
	}
	return false;
}

static bool translate_commands(creature* player) {
	for(auto& e : adventure_keys) {
		if(hot.key == e.key) {
			hot.key = 0;
			hot.param = e.param;
			if(player && e.proc.pcre)
				(player->*e.proc.pcre)();
			else
				e.proc.pinp();
			breakmodal(0);
			return true;
		}
	}
	return false;
}

void creature::playui() {
	current_index = Blocked;
	setbackground(render_indoor);
	while(ismodal()) {
		current_background();
		render_message();
		domodal();
		if(translate_move(this))
			continue;
		if(translate_commands(this))
			continue;
	}
}

void creature::pause() {
	current_index = Blocked;
	while(ismodal()) {
		current_background();
		render_message("Пробел");
		domodal();
		switch(hot.key) {
		case KeySpace:
		case KeyEscape:
			breakmodal(1);
			hot.key = 0;
			break;
		}
	}
}

void location::show(rooma& rooms) {
	while(ismodal()) {
		rectf({0, 0, getwidth(), getheight()}, colors::form);
		auto x0 = mmaps, y0 = mmaps;
		for(auto& e : rooms) {
			rect rc;
			rc.x1 = x0 + e.x1*mmaps;
			rc.x2 = x0 + e.x2*mmaps;
			rc.y1 = x0 + e.y1*mmaps;
			rc.y2 = x0 + e.y2*mmaps;
			rectb(rc, colors::white);
		}
		domodal();
		switch(hot.key) {
		case KeySpace:
		case KeyEscape:
			breakmodal(1);
			hot.key = 0;
			break;
		}
	}
}