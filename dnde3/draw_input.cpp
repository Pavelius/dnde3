#include "main.h"
#include "draw.h"

using namespace draw;

namespace draw {
struct hotkey {
	int				key;
	const char*		name;
	void(*proc)();
	int				param;
	explicit operator bool() const { return proc != 0; }
};
}

namespace colors {
color				fow;
}
const int			gui_border = 8;
const int			gui_padding = 4;
static eventproc	current_background;
static eventproc	current_layer;
static location*	current_location;
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

struct imgi {
	const char*		name;
	const char*		folder;
	const sprite*	data;
	bool			notfound;
};
imgi bsmeta<imgi>::elements[] = {{""},
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
		if(current_location)
			current_location->write("overland.map");
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

static int windowf(int x, int y, int width, const char* string) {
	rect rc = {x, y, x + width, y};
	draw::state push;
	draw::font = metrics::font;
	auto height = draw::textf(rc, string);
	rc.x2 = rc.x1 + width;
	window(rc, false, 0);
	draw::textf(x, y, rc.width(), string);
	return height + gui_border * 2 + gui_padding;
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
		camera.x = x*elx;
	if(x > x2 - 1)
		camera.x = (x - viewport.x / elx)*elx;
	if(y < y1)
		camera.y = y*ely;
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
	correct(camera, i1);
	return i1;
}

static bool shortcuts(const hotkey* ph) {
	for(auto p = ph; p->proc; p++) {
		if(hot.key != p->key || hot.pressed)
			continue;
		execute(p->proc, p->param);
		return true;
	}
	return false;
}

void picture::set(short x, short y) {
	this->x = x*elx;
	this->y = y*ely;
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

static int detaih(int x, int y, int width, const hotkey* pk) {
	auto x0 = x;
	for(auto p = pk; *p; p++) {
		auto w = 0;
		if(button(x, y, p->name, p->key, &w))
			execute(p->proc, p->param);
		x += w;
	}
	return x - x0;
}

static void setbackground(eventproc proc) {
	current_background = proc;
}

void gamei::setnextlayer(void(*proc)()) {
	current_layer = proc;
}

void gamei::layer() {
	while(current_layer) {
		auto proc = current_layer;
		current_layer = 0;
		proc();
	}
}

int	widget(eventproc before, eventproc after) {
	while(ismodal()) {
		current_background();
		if(before)
			before();
		domodal();
		if(after)
			after();
	}
	return getresult() != 0;
}

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

static hotkey hotkeys[] = {{Alpha + '1', getstr(Plain), choose_tile_1},
{Alpha + '2', getstr(Sea), choose_tile_2},
{Alpha + '3', getstr(Foothills), choose_tile_3},
{Alpha + '4', getstr(Mountains), choose_tile_4},
{Alpha + '5', getstr(CloudPeaks), choose_tile_5},
{Alpha + '6', getstr(Forest), choose_tile_6},
{}};

static int render_keys(int x, int y, int width) {
	auto x0 = x;
	char temp[260]; stringbuilder sb(temp);
	sb.add("Вывести [%1]", getstr(current_tile));
	//x += button(x, y, temp, KeySpace, put_tile);
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
	picture effects[1]; effects[0].setcursor(current_index, 1);
	current_location->worldmap(camera, true);
	render(effects);
	render_bottom();
}

static void render_indoor() {
	current_location->indoor(camera, false, 0);
	current_index = translate(current_index);
}

static void controls() {
	current_index = translate(current_index);
}

void location::editor() {
	current_location = this;
	if(current_location)
		current_location->read("overland.map");
	setbackground(render_editor);
	widget(0, controls);
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
	auto type = player.getclass();
	auto race = player.getrace();
	auto gender = player.getgender();
	auto melee = player.get(Melee);
	auto offhand = player.get(OffHand);
	auto ranged = player.get(Ranged);
	auto body = player.get(Torso);
	auto back = player.get(TorsoBack);
	if(melee.istwohanded()) {
		i1 = 2;
		i2 = 3 + melee.gettype() - FirstWeapon;
	} else {
		if(melee.gettype())
			i1 = 3 + melee.gettype() - FirstWeapon;
		if(offhand.gettype() == Shield)
			i2 = 3 + (SwordTwoHanded - FirstWeapon + 1) + (SwordTwoHanded - FirstWeapon + 1);
		else if(offhand.gettype())
			i2 = 3 + (SwordTwoHanded - FirstWeapon + 1) + (offhand.gettype() - FirstWeapon);
	}
	auto at = 2;
	switch(body.gettype()) {
	case NoItem:
		if(type == Fighter || type == Theif)
			at = 1;
		else
			at = 0;
		break;
	case LeatherArmour:
	case StuddedLeatherArmour:
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
	if(ranged && getorder(ranged.gettype()) == 0) {
		auto index = getaccindex(ranged.gettype());
		if(index != -1)
			image(x, y, gres(ResPCmac), index, flags, alpha);
	}
	// Character body
	image(x, y, gres(ResPCmar), i1, flags, alpha);
	image(x, y, gres(ResPCmbd), (race - Human) * 6 + (gender - Male) * 3 + at, flags, alpha);
	// Weapon on belt
	if(ranged && getorder(ranged.gettype()) == 2) {
		auto index = getaccindex(ranged.gettype());
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
	item_s stuff[scrx*scry];
	auto night_percent = 0;
	auto is_dungeon = false;
	const sprite *floor, *walls;
	if(is_dungeon) {
		floor = gres(ResDungeon);
		walls = gres(ResDungeonW);
	} else {
		night_percent = (128 * /*game::getnight()*/0) / 100;
		floor = gres(ResGrass);
		walls = gres(ResGrassW);
	}
	auto shadow = gres(ResShadow);
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
	// Инициализируем видимые предметы
	memset(stuff, 0, sizeof(stuff));
	//for(auto& e : grounditems) {
	//	if(!e)
	//		continue;
	//	auto i = mget(rc.x1, rc.y1, game::getx(e.index), game::gety(e.index));
	//	if(i == -1)
	//		continue;
	//	if(stuff[i] == NoItem)
	//		stuff[i] = e.gettype();
	//	else
	//		stuff[i] = ManyItems;
	//}
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
			auto can_be_shadow = false;
			// Тайлы
			switch(t) {
			case Hill:
				image(x, y, floor, r, 0);
				image(x, y, gres(ResFeature), 2, 0);
				can_be_shadow = true;
				break;
			case Swamp:
				image(x, y, floor, r, 0);
				image(x, y, gres(ResFeature), 3, 0);
				can_be_shadow = true;
				break;
			case Floor:
				if(is_dungeon)
					image(x, y, floor, r, 0);
				else
					image(x, y, floor, 4, 0);
				can_be_shadow = true;
				break;
			case Plain:
				image(x, y, floor, r, 0);
				can_be_shadow = true;
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
				can_be_shadow = true;
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
			}
			// Предметы на земле
			auto pi = mget(rc.x1, rc.y1, mx, my);
			if(pi != -1) {
				switch(stuff[pi]) {
				case NoItem:
					break;
				case ManyItems:
					draw::image(x, y - 8, gres(ResItems), 0, 0);
					break;
				default:
					draw::image(x, y - 8, gres(ResItems), stuff[pi], 0);
					break;
				}
			}
			// Тени
			if(can_be_shadow) {
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
		}
	}
	// Нижний уровень эффектов
	if(effects) {
		for(auto p = effects; *p; p++)
			p->render(camera.x, camera.y);
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
			case Tree:
				image(x, y, gres(ResFeature), 4 + r % 3, 0);
				break;
			case StairsDown:
				image(x, y, gres(ResFeature), 56, 0);
				break;
			case StairsUp:
				image(x, y, gres(ResFeature), 57, 0);
				break;
			case Door:
				r = is(i, Opened) ? 0 : 1;
				if(gettile(to(i, Left)) == Wall && gettile(to(i, Right)) == Wall)
					draw::image(x, y + 16, gres(ResDoors), r, 0);
				else if(gettile(to(i, Up)) == Wall && gettile(to(i, Down)) == Wall)
					draw::image(x, y, gres(ResDoors), 2 + r, 0);
				break;
			}
			// Вывод юнитов
			int i1 = mget(rc.x1, rc.y1, mx, my);
			if(i1 != -1) {
				auto pc = units[i1];
				if(pc) {
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
		auto h1 = textf(x + z, y, width - z, e.text, 0);
		if(h1 > h)
			h = h1;
	}
	if(hot.key == k)
		execute(breakparam, i);
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

void location::adventure() {
	current_location = this;
	setbackground(render_indoor);
	widget(0, controls);
}