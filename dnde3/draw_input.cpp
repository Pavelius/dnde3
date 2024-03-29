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
color				fow = color::create(20, 21, 20);
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
static point		viewport = {800, 600};
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
imgi bsdata<imgi>::elements[] = {
	{""},
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
	{"traps", "art"},
	{"sea", "art"},
	{"plain", "art"},
	{"hills", "art"},
	{"forest", "art"},
	{"mount", "art"},
	{"tmount", "art"},
	{"outdoor", "art"},
	{"swamp", "art"},
	{"trail", "art"},
	{"ui", "art"},
	{"pcmar", "art"},
	{"pcmbd", "art"},
	{"pcmac", "art"},
};
assert_enum(imgi, ResPCmac);

static hotkeym move_keys[] = {{KeyLeft, Left},
	{KeyHome, LeftUp},
	{KeyUp, Up},
	{KeyPageUp, RightUp},
	{KeyRight, Right},
	{KeyPageDown, RightDown},
	{KeyDown, Down},
	{KeyEnd, LeftDown},
};
static picture trail_images[] = {
	{}, {ResTrail, 6, ImageMirrorV}, {ResTrail, 6}, {ResTrail, 4},
	{ResTrail, 3, ImageMirrorH}, {ResTrail, 0, ImageMirrorH | ImageMirrorV}, {ResTrail, 0, ImageMirrorH}, {ResTrail, 5, ImageMirrorH},
	{ResTrail, 3}, {ResTrail, 0, ImageMirrorV}, {ResTrail, 0}, {ResTrail, 5},
	{ResTrail, 2}, {ResTrail, 7, ImageMirrorV}, {ResTrail, 7}, {ResTrail, 1},
};

static const sprite* gres(img_s i) {
	auto& e = bsdata<imgi>::elements[i];
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
	return bsdata<imgi>::elements[i].data;
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

void gamei::setnobackground() {
	current_background = no_background;
}

void gamei::intialize() {
	draw::initialize();
	setnobackground();
	draw::create(-1, -1, 800, 600, WFResize | WFMinmax, 32);
	draw::setcaption("Dungeon adventures");
}

static indext gets2i(const point& camera) {
	return location::get(camera.x / elx, camera.y / ely);
}

static void window(rect rc, bool disabled, int border) {
	if(border == 0)
		border = gui_border;
	rc.offset(-border, -border);
	color c = colors::form;
	auto op = 192;
	if(disabled)
		op = op / 2;
	draw::rectf(rc, c, op);
	draw::rectb(rc, colors::form);
}

static int buttonr(int x, int y, int w1, const char* name) {
	auto w = textw(name);
	if(w1 == -1)
		w1 = w;
	rect rc = {x - 2, y - 1, x + w1 + 2, y + texth()};
	rectf(rc, colors::button);
	rectb(rc, colors::form);
	text(x + (w1 - w) / 2, y, name);
	return w1 + 5;
}

static void windowf(const char* string, const char* press_key) {
	rect rc = {0, 0, 400, 0};
	draw::textf(rc, string);
	auto w = rc.width();
	auto h = rc.height();
	rc.x1 = (getwidth() - w) / 2;
	rc.y1 = gui_padding * 3;
	rc.x2 = rc.x1 + w;
	rc.y2 = rc.y1 + h;
	window(rc, false, 0);
	draw::textf(rc.x1, rc.y1, w, string);
	if(press_key)
		buttonr(rc.x1, rc.y2 + gui_border + 1, -1, press_key);
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
	auto mx = mmx * elx - elx / 2;
	auto my = mmy * ely - ely / 2;
	if(camera.x < 0)
		camera.x = 0;
	if(camera.y < 0)
		camera.y = 0;
	if(camera.x > mx - viewport.x)
		camera.x = mx - viewport.x;
	if(camera.y > my - viewport.y)
		camera.y = my - viewport.y;
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
		camera.x = (x - viewport.x / elx) * elx;
	if(y < y1)
		camera.y = y * ely;
	if(y > y2 - 1)
		camera.y = (y - viewport.y / ely) * ely;
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
	default: return Blocked;
	}
	if(i1 == Blocked)
		return Blocked;
	hot.key = 0;
	correct(camera, i1);
	return i1;
}

void picture::set(short x, short y) {
	pos.x = x * elx;
	pos.y = y * ely;
}

void picture::render(int x, int y) const {
	image(pos.x + x, pos.y + y, gres(img), frame, flags, alpha ? alpha : 0xFF);
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
	if(key & Ctrl) {
		sb.add("Ctrl+");
		key &= ~Ctrl;
	}
	if(key & Alt) {
		sb.add("Alt+");
		key &= ~Alt;
	}
	if(key & Shift) {
		sb.add("Shift+");
		key &= ~Shift;
	}
	char temp[2];
	switch(key) {
	case KeyEscape: sb.add("Esc"); break;
	case KeySpace: sb.add("Space"); break;
	case KeyDown: sb.add("����"); break;
	case KeyUp: sb.add("�����"); break;
	case KeyLeft: sb.add("����"); break;
	case KeyRight: sb.add("�����"); break;
	default:
		temp[0] = key;
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

static void render_editor() {
	loc.worldmap(camera, false);
	picture effects[1]; effects[0].setcursor(current_index, 1);
	render(effects);
}

static void header(int x, int y, const char* name) {
	char temp[128]; stringbuilder sb(temp);
	draw::state push;
	sb.add("%1:", name);
	draw::text(x, y, sb);
}

static int fielw(int x, int y, int v1, int b1, const char* format = "%1i") {
	auto p_fore = fore;
	if(v1 > b1)
		fore = colors::text.mix(colors::green, 96);
	else if(v1 < b1)
		fore = colors::text.mix(colors::red, 64);
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
		text(x, y, " � ");
		x += textw(" � ");
		fielw(x, y, v2, b2, f2);
	}
	return draw::texth();
}

static int fielp(int x, int y, int w, const char* name, int p1, int b1, int p2, int b2) {
	header(x, y, name);
	char temp[128]; stringbuilder sb(temp);
	sb.add("%1i%%", p1);
	if(p2)
		sb.adds("� %1i%%", p2);
	draw::text(x + w, y, sb);
	return draw::texth();
}

static int fielr(int x, int y, int w, const char* name, int p1, int p2) {
	header(x, y, name);
	char temp[128]; stringbuilder sb(temp);
	sb.add("%1i%%", p1);
	if(p2)
		sb.add(" � %1i", p2);
	draw::text(x + w, y, temp);
	return draw::texth();
}

static int fielt(int x, int y, int w, const char* name, int value) {
	//header(x, y, name);
	char temp[128]; stringbuilder sb(temp);
	sb.add("����: %1i, ���: %2i", value / (24 * 60), (value % (24 * 60)) / 60);
	draw::text(x, y, sb);
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
	auto attm = e.getattack(Melee);
	auto attr = e.getattack(Ranged);
	int y1 = y;
	int x1 = x;
	y += fiela(x, y, tw, "��", e.get(Strenght), e.getbase(Strenght));
	y += fiela(x, y, tw, "��", e.get(Intellegence), e.getbase(Intellegence));
	x += dx;
	y = y1;
	y += fiela(x, y, tw, "��", e.get(Dexterity), e.getbase(Dexterity));
	y += fiela(x, y, tw, "��", e.get(Wisdow), e.getbase(Wisdow));
	x += dx;
	y = y1;
	y += fiela(x, y, tw, "��", e.get(Constitution), e.getbase(Constitution));
	y += fiela(x, y, tw, "��", e.get(Charisma), e.getbase(Charisma));
	x += dx + 6;
	y = y1;
	y += fiela(x, y, tw, "��", attm.attack, attm.attack, "%1i%%", attr.attack, attr.attack, "%1i%%");
	y += fiela(x, y, tw, "��", e.get(Protection), e.get(Protection), "%1i%%", e.get(Armor), e.getbase(Armor));
	x += dx + 50;
	y = y1;
	y += field(x, y, 40, "����", e.gethits(), e.get(LifePoints));
	y += field(x, y, 40, "����", e.getmana(), e.get(ManaPoints));
	x += dx + 40 + 20 - tw;
	y = y1;
	y += field(x, y, 52, "����", e.getexperience());
	y += field(x, y, 52, "������", e.getmoney());
	x += dx + 58;
	y = y1;
	y += fielt(x, y, 52, "�����", game.getrounds());
	y += fiela(x, y, 52, "��", e.get(Deflect), e.get(Deflect), "%1i%%");
	x = x1;
	y = y1 + draw::texth() * 2;
	// Draw encumbrance
	switch(e.getencumbred()) {
	case Encumbered: x += texth(x, y, "��������", 0); break;
	case HeavilyEncumbered: x += texth(x, y, "��������", 1); break;
	}
	// Draw status
	for(auto i = Darkvision; i <= LastState; i = (state_s)(i + 1)) {
		if(!e.is(i))
			continue;
		auto& ei = bsdata<statei>::elements[i];
		if(ei.flags.is(Friendly))
			continue;
		x += texth(x, y, ei.name, ei.flags.is(Hostile) ? 2 : 0);
	}
}

static void render_message(const char* press_key = 0) {
	if(!message_text[0])
		return;
	windowf(message_text, press_key);
	if(!press_key)
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
	auto i = translate(current_index);
	if(i != Blocked)
		current_index = i;
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

void location::viewfow(int x0, int y0, const rect& rc) const {
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
				bool dw = loc.xget(i, Down);
				bool up = loc.xget(i, Up);
				bool rg = loc.xget(i, Right);
				bool lf = loc.xget(i, Left);
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

void location::indoor(point camera, bool show_fow, const picture* effects) {
	creature* units[scrx * scry];
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
	// �������������� ������� �����
	memset(units, 0, sizeof(units));
	for(auto& e : bsdata<creature>()) {
		if(!e)
			continue;
		auto i0 = e.getposition();
		auto i = mget(rc.x1, rc.y1, getx(i0), gety(i0));
		if(i == Blocked)
			continue;
		units[i] = &e;
	}
	// ������ �������
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
			// �����
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
			// ������� �� �����
			switch(o) {
			case Trap:
				if(!is(i, Hidden))
					image(x, y, gres(ResTraps), gettrap(i), 0);
				break;
			case Pool:
				if(!is(i, Hidden))
					image(x, y, gres(ResTraps), bsdata<map_objecti>::elements[Pool].start + getrand(i) % 15, 0);
				break;
			case Plants:
				image(x, y, gres(ResFeature), 36 + (getrand(i) / 60) % 3, 0);
				break;
			}
			// �����
			if(is(i, Blooded))
				image(x, y, gres(ResBlood), 12 + r, 0, 192);
		}
	}
	// �������� �� �����
	for(auto& e : bsdata<itemground>()) {
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
	// ������ ������� ��������
	if(effects) {
		for(auto p = effects; *p; p++)
			p->render(-camera.x, -camera.y);
	}
	// ����
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
			// �������
			if(is(i, Webbed))
				image(x, y, gres(ResFeature), 74 + getrand(i) % 3, 0);
		}
	}
	// ������� �������
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
				if(gettile(i) == Wall)
					break; // Hidden door
				r = is(i, Opened) ? 0 : 1;
				if(gettile(to(i, Left)) == Wall && gettile(to(i, Right)) == Wall)
					draw::image(x, y + 16, doors, r, 0);
				else if(gettile(to(i, Up)) == Wall && gettile(to(i, Down)) == Wall)
					draw::image(x, y, doors, 2 + r, 0);
				break;
			default:
				if(bsdata<map_objecti>::elements[t].start) {
					auto c = bsdata<map_objecti>::elements[t].count;
					if(c)
						image(x, y, gres(ResFeature), bsdata<map_objecti>::elements[t].start + getrand(i) % c, 0);
					else
						image(x, y, gres(ResFeature), bsdata<map_objecti>::elements[t].start, 0);
				}
				break;
			}
			// ����� ������
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
						// ���������� ����� ������ ����� ��� ������ ������
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
			// ���������� �����
			if(gettile(i) != Wall) {
				// � ������� ����� ������ ���������� ������ � ����������
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
	// ������ �����
	if(night_percent)
		rectf({0, 0, draw::getwidth(), draw::getheight()}, color::create(0, 0, 64), night_percent);
	// Show fog of war
	if(show_fow)
		viewfow(x0 + elx / 2, y0 + ely / 2, rc);
}

int	answeri::paint(int x, int y, int width, int i, int& maximum_width, const column* columns) const {
	char k = '1' + i;
	if(i >= 9)
		k = 'A' + (i - 9);
	auto z = 22;
	char temp[2] = {k, 0};
	buttonr(x, y, z - 6, k);
	auto& e = elements[i];
	auto h = texth();
	if(e.text) {
		auto mw = 0;
		auto h1 = textf(x + z, y, width - z, e.text, &mw);
		if(mw + z > maximum_width)
			maximum_width = mw + z;
		if(h1 > h)
			h = h1;
	}
	if(hot.key == k)
		execute(breakparam, elements[i].param);
	if(columns) {
		auto x2 = x + width;
		for(auto* p = columns; *p; p++) {
			char temp[260]; stringbuilder sb(temp);
			auto x1 = x2 - p->width;
			p->proc((void*)elements[i].param, sb);
			textf(x1, y, x2 - x1, temp);
		}
	}
	return h;
}

static void paint_header(int x, int& y, int width, const answeri::column* columns) {
	auto push_fore = fore;
	fore = colors::yellow.mix(colors::text);
	auto x2 = x + width;
	y += metrics::padding;
	for(auto* p = columns; *p; p++) {
		auto x1 = x2 - p->width;
		textc(x1, y, p->width, p->id);
	}
	text(x + 22, y, "������������");
	fore = push_fore;
	y += texth() + metrics::padding;
}

int	answeri::paint(int x, int y, int width, const char* format, int& maximum_width, int y2, const answeri::column* columns, int columns_count) const {
	auto i = 0;
	auto z = y;
	if(format)
		y += textf(x, y, width, format, &maximum_width) + 4;
	auto w = width / columns_count;
	auto t = y;
	if(columns) {
		maximum_width = width;
		paint_header(x, y, width, columns);
	}
	for(auto& e : elements) {
		if(y2 != -1 && (y + texth() + 4) >= y2) {
			x += w;
			y = t;
		}
		if(columns)
			y += paint(x, y, width, i++, maximum_width, columns) + 2;
		else
			y += paint(x, y, w, i++, maximum_width, columns) + 2;
	}
	return y - z;
}

int	answeri::choosev(bool interactive, bool clear_text, bool return_single, const char* format) const {
	auto w = 400;
	auto h = 0;
	if(format) {
		auto mw = 0;
		draw::state push;
		setclip({0, 0, 0, 0});
		h = paint(0, 0, w, format, mw);
		w = mw;
	}
	while(ismodal()) {
		current_background();
		rect rc = {(getwidth() - w) / 2, gui_padding * 3, (getwidth() + w) / 2, gui_padding * 3 + h};
		window(rc, false, 0);
		auto maximum_width = 0;
		paint(rc.x1, rc.y1, rc.width(), format, maximum_width);
		domodal();
	}
	if(clear_text)
		sb.clear();
	return getresult();
}

int	answeri::choosev(const char* interactive, const char* format, bool allow_cancel, const answeri::column* columns) const {
	int x, y;
	const int width = 600;
	while(ismodal()) {
		current_background();
		dialogw(x, y, width, 440, interactive);
		auto maximum_width = 0;
		paint(x, y, width, format, maximum_width, 480, columns);
		domodal();
		if(allow_cancel && hot.key == KeyEscape)
			breakmodal(0);
	}
	return getresult();
}

int	answeri::dialogv(bool allow_cancel, const char* title, const char* format) const {
	int x, y;
	const int width = 600;
	while(ismodal()) {
		current_background();
		dialogw(x, y, width, 400, title);
		auto maximum_width = 0;
		paint(x, y, width, format, maximum_width, 440, 0, 2);
		domodal();
		if(allow_cancel && hot.key == KeyEscape)
			breakmodal(0);
	}
	return getresult();
}

int	answeri::menuv(bool allow_cancel, const char* format) const {
	const int width = 200;
	while(ismodal()) {
		current_background();
		auto x = getwidth() - width - metrics::padding * 4;
		auto y = metrics::padding * 5;
		auto index = 0;
		for(auto& e : elements) {
			rect rc = {x, y, x + width, y + texth()};
			window(rc, false, 0);
			char k = '1' + index;
			char z = 22;
			char temp[2] = {k, 0};
			buttonr(x, y, z - 6, k);
			rc.x1 += 22;
			//rectb(rc, colors::red);
			text(rc, e.text, AlignCenterCenter);
			y += texth() + metrics::padding * 4 + 2;
			if(hot.key == k)
				execute(breakparam, e.param);
			index++;
		}
		domodal();
		if(allow_cancel && hot.key == KeyEscape)
			buttoncancel();
	}
	return getresult();
}

static void render_item(int x, int y, int width, const item& e, bool show_damage = false, bool show_empthy = false) {
	if(!show_empthy && !e)
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
	if(e.is(KnownStats) || !e) {
		sb.clear();
		e.getstatistic(sb);
		text(x + width - 160, y, temp);
	}
	if(show_damage) {
		auto p = e.getdamagetext();
		if(p && p[0])
			text(x + width - 170 - textw(p), y, p);
	}
	fore = ps_fore;
	//line(x + width, y, x + width, y + 10, colors::red);
}

static void render_weight(int x, int y, int width, const item& e) {
	if(!e)
		return;
	char temp[64]; stringbuilder sb(temp);
	auto v = e.getweight();
	if(v < 10)
		sb.add("%1i ��", v * 10);
	else
		sb.add("%1i.%2i ��", v / 100, (v / 10) % 10);
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
	if(slot >= Head && slot <= LastWear) {
		if(mode == SlotWhere)
			name = bsdata<sloti>::elements[slot].name_where;
		else
			name = bsdata<sloti>::elements[slot].name;
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
	text(x, y, temp);
	return width + 8;
}

skill_s skillu::choose(statable* player, bool interactive, const char* title, bool* cancel_result) const {
	int x, y, y1;
	const int width = 400;
	if(cancel_result)
		*cancel_result = false;
	while(ismodal()) {
		current_background();
		dialogw(x, y, width, 300, title, &y1);
		auto x1 = x;
		auto x2 = x + width;
		if(cancel_result)
			button(x1, y1, "������", KeyEscape, breakparam, -1);
		auto index = 0;
		for(auto e : *this) {
			auto x0 = x;
			auto sv = player->get(e);
			auto cv = getcap(e);
			if(button(x0, y, 0, answeri::getkey(index), 0)) {
				auto need_execute = true;
				if(!cancel_result && sv >= cv)
					need_execute = false;
				if(need_execute)
					execute(breakparam, (int)e);
			}
			x0 += 22;
			if((index + 1) % 2)
				rectf({x, y, x + width, y + texth() + 1}, colors::white, 4);
			text(x0, y, getstr(e)); x0 += 220;
			x0 += text(x0, y, 36, player->get(e), "%1i%%");
			if(cancel_result || sv >= cv)
				x0 += text(x0, y, 100, creature::getrangname(sv));
			else {
				x0 += text(x0, y, 36, player->getraise(e));
				x0 += text(x0, y, 64, cv, " ����. %1i%%");
			}
			index++;
			y += texth() + 4;
		}
		y += texth();
		draw::domodal();
	}
	auto v = getresult();
	if(cancel_result && v == -1)
		*cancel_result = true;
	return (skill_s)v;
}

item* itema::choose(const char* interactive, const char* format, slot_mode_s mode, bool show_always, bool cancel) {
	if(!count) {
		if(interactive) {
			if(!show_always) {
				sb.adds(interactive);
				sb.adds("� ��� ������ ����.");
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
		dialogw(x, y, width, 440, interactive, &y1);
		auto x1 = x;
		auto x2 = x + width;
		if(format)
			y += textf(x, y, width, format);
		if(cancel)
			button(x1, y1, "������", KeyEscape, breakparam, 0);
		if(count > 0) {
			auto index = 0;
			for(auto e : *this) {
				auto x0 = x;
				if(button(x0, y, 0, answeri::getkey(index), 0))
					execute(breakparam, (int)e);
				x0 += 22;
				render(x0, y, 110, *e, mode);
				auto slot = e->getwearerslot();
				render_item(x0, y, x2 - x0 - 72, *e, mode == NoSlotName, slot == Melee);
				render_weight(x0, y, x2 - x0, *e);
				index++;
				y += texth() + 4;
			}
			y += texth();
		} else
			y += textf(x, y, width, "������ ���.") + texth() + 4;
		render(x, y, width, *this);
		domodal();
	}
	return (item*)getresult();
}

int indexa::choose(const char* interactive) {
	if(!count)
		return -1;
	else if(count == 1)
		return 0;
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
			sb.adds(interactive);
			const char* text_answer = 0;
			if(!text_answer) {
				auto p = creature::find(current_index);
				if(p)
					text_answer = p->getname();
			}
			if(!text_answer) {
				auto o = loc.getobject(current_index);
				if(o)
					text_answer = bsdata<map_objecti>::elements[o].name;
			}
			if(!text_answer) {
				auto t = loc.gettile(current_index);
				text_answer = bsdata<tilei>::elements[t].name;
			}
			if(text_answer)
				sb.adds("[%1].", text_answer);
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
			button(x1, y1, "������", KeyEscape, breakparam, -1);
		auto index = 0;
		for(auto e : *this) {
			auto x0 = x;
			if(button(x0, y, 0, answeri::getkey(index), 0))
				execute(breakparam, (int)e);
			x0 += 22;
			if((index + 1) % 2)
				rectf({x, y, x + width, y + texth() + 1}, colors::white, 4);
			text(x0, y, getstr(e)); x0 += 220;
			if(player)
				x0 += text(x0, y, 46, player->get(e), "%1i ����");
			x0 += text(x0, y, 100, bsdata<spelli>::elements[e].mp, "%1i ����");
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

static point getpos(point origin, short x, short y) {
	return {origin.x + x * mmaps + mmaps / 2 + 1, origin.y + y * mmaps + mmaps / 2 + 1};
}

static void view_bullet(point origin, indext index, int number) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("%1i", number);
	auto p_fore = fore;
	auto pm = getpos(origin, loc.getx(index), loc.gety(index));
	circlef(pm.x, pm.y, 8, colors::white);
	fore = colors::black;
	circle(pm.x, pm.y, 8, colors::white);
	fore = colors::black;
	text(pm.x - textw(temp) / 2 - 1, pm.y - texth() / 2, temp);
	fore = p_fore;
}

static bool view_name(int x, int y, const site& e, int number) {
	char temp[260]; temp[0] = 0; stringbuilder sb(temp); e.getname(sb);
	if(!temp[0])
		return false;
	text(x, y, temp);
	sb.clear();
	sb.add("%1i.", number);
	text(x - 26, y, temp);
	return true;
}

static void view_legends(point origin, bool fow) {
	auto number = 1;
	auto x1 = origin.x + mmx * mmaps + 40;
	auto y1 = origin.y;
	for(auto& e : bsdata<site>()) {
		auto index = e.getposition();
		if(index == Blocked)
			continue;
		if(fow && !loc.is(index, Explored))
			continue;
		if(!view_name(x1, y1, e, number))
			continue;
		view_bullet(origin, index, number);
		y1 += texth() + 4;
		number++;
	}
}

void location::minimap(int x, int y, point camera, bool fow) const {
	if(x < 8)
		x = 8;
	int y3 = y;
	color floor, floor1;
	auto is_dungeon = isdungeon();
	if(!is_dungeon) {
		floor = color::create(51, 140, 29);
		floor1 = color::create(95, 77, 62);
	} else {
		floor = colors::gray;
		floor1 = floor.darken();
	}
	color object = colors::white;
	if(!is_dungeon)
		object = floor.darken();
	color wall = colors::black.lighten();
	color door = colors::red;
	color stairs = colors::red.darken().darken();
	color water = colors::blue;
	color road = color::create(94, 70, 51).mix(floor, 192);
	color border = colors::white;
	color tree = color::create(40, 120, 19);
	for(int y1 = 0; y1 < mmy; y1++) {
		int x3 = x;
		for(int x1 = 0; x1 < mmx; x1++, x3 += mmaps) {
			auto i = get(x1, y1);
			if(fow && !is(i, Explored))
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
				pixel(x3, y3, wall);
				continue; // Hidden objects don't shown
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
				pixel(x3, y3, tree);
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
	cm.x2 = x + (xs1 + viewport.x) * mmx1 / scx;
	cm.y2 = y + (ys1 + viewport.y) * mmy1 / scy;
	draw::rectb(cm, border);
	view_legends({(short)x, (short)y}, fow);
}

void location::minimap(indext index, bool fow) const {
	const bool show_title = true;
	char temp[128]; stringbuilder sb(temp);
	sb.add("%+1", getstr(loc.type));
	if(loc.level > 0)
		sb.add(" - ������� %1i", loc.level);
	int w = mmx * mmaps + 280;
	int h = mmy * mmaps;
	point camera = {getx(index) * elx - viewport.x / 2, gety(index) * ely - viewport.y / 2};
	while(ismodal()) {
		draw::rectf({0, 0, draw::getwidth(), draw::getheight()}, colors::form);
		auto y = 4;
		if(show_title) {
			auto pf = font;
			font = metrics::h1;
			text((draw::getwidth() - textw(temp)) / 2, y, temp);
			y += texth() + 8;
			font = pf;
		}
		loc.minimap((draw::getwidth() - w) / 2, y, camera, fow);
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
	camera.x = x * elx - getwidth() / 2 + elx / 2;
	camera.y = y * ely - getheight() / 2 + ely / 2;
	correct(camera);
}

static void change_player() {
	auto po = creature::getactive();
	auto pn = creature::getactive(hot.param);
	if(!pn)
		return;
	if(pn != po && pn) {
		po->wait();
		pn->activate();
	}
}

static bool translate_move(creature* player) {
	for(auto& e : move_keys) {
		if(e.key == hot.key) {
			if(player->leaving(e.direction)) {
				breakmodal(0);
				return true;
			}
			auto ni = location::to(player->getposition(), e.direction);
			player->move(ni);
			breakmodal(0);
			return true;
		}
	}
	return false;
}

void answeri::information(const char* title, fntext fparam, int w1 = 80) const {
	const int window_width = 680;
	while(ismodal()) {
		current_background();
		int x, y, y2;
		dialogw(x, y, window_width, 420, title, &y2);
		auto y1 = y;
		auto dx = window_width / 2 - (x - (draw::getwidth() - 680) / 2) * 2;
		auto w2 = dx - w1;
		auto push_fore = fore;
		for(auto& e : elements) {
			char temp[260]; stringbuilder sb(temp);
			auto t1 = fparam(&e, sb);
			fore = colors::yellow;
			text({x, y, x + w1, y + texth()}, t1, AlignCenter);
			fore = colors::text;
			y += text({x + w1, y, x + dx, y + texth()}, e.text, AlignLeft) + 4;
			if(y >= y2) {
				x += dx;
				y = y1;
			}
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
	sb.clear();
}

const char* answeri::getnmshortcut(const void* object, stringbuilder& sb) {
	auto pe = (element*)object;
	auto key = pe->param;
	if(key & Ctrl)
		sb.add("Ctrl+");
	if(key & Alt)
		sb.add("Alt+");
	if(key & Shift)
		sb.add("Shift+");
	key = key & 0xFFFF;
	switch(key) {
	case KeyDown: sb.add("Down"); break;
	case KeyDelete: sb.add("Del"); break;
	case KeyEnd: sb.add("End"); break;
	case KeyEnter: sb.add("Enter"); break;
	case KeyHome: sb.add("Home"); break;
	case KeyLeft: sb.add("Left"); break;
	case KeyPageDown: sb.add("Page Down"); break;
	case KeyPageUp: sb.add("Page Up"); break;
	case KeyRight: sb.add("Right"); break;
	case KeyUp: sb.add("Up"); break;
	case F1: sb.add("F1"); break;
	case F2: sb.add("F2"); break;
	case F3: sb.add("F3"); break;
	case F4: sb.add("F4"); break;
	case F5: sb.add("F5"); break;
	case F6: sb.add("F6"); break;
	case F7: sb.add("F7"); break;
	case F8: sb.add("F8"); break;
	case F9: sb.add("F9"); break;
	case F10: sb.add("F10"); break;
	case F11: sb.add("F11"); break;
	case F12: sb.add("F12"); break;
	case KeySpace: sb.add("Space"); break;
	default: sb.add(szupper(key)); break;
	}
	return sb;
}

static void show_shortcuts(const hotkey* k1) {
	answeri a;
	for(auto p = k1; *p; p++)
		a.add(p->key, p->name);
	a.sort();
	a.shortcuts("������� �������");
}

static bool translate_commands(creature* player, const hotkey* keys, bool terminate) {
	if(hot.key == KeyEscape) {
		show_shortcuts(keys);
		return true;
	}
	for(auto k = keys; *k; k++) {
		if(hot.key == k->key) {
			hot.key = 0;
			hot.param = k->param;
			if(player && k->proc.pcre)
				(player->*k->proc.pcre)();
			else if(k->proc.ploc)
				(loc.*k->proc.ploc)(current_index);
			else
				k->proc.pinp();
			if(terminate)
				breakmodal(0);
			return true;
		}
	}
	return false;
}

static hotkey indoor_keys[] = {
	{F1, "������� 1-�� �����", change_player, 0},
	{F2, "������� 2-�� �����", change_player, 1},
	{F3, "������� 3-�� �����", change_player, 2},
	{F4, "������� 4-�� �����", change_player, 3},
	{Ctrl + 'M', "������� ������", gamei::help},
	{'I', "������� ���������", &creature::inventory},
	{'A', "������������ �����", &creature::useskills},
	{'D', "�������� �������", &creature::dropdown},
	{'P', "������� �������", &creature::pickup},
	{'Q', "�������� �� �����", &creature::shoot},
	{'V', "������", &creature::backpack},
	{'M', "����� ���������", &creature::minimap},
	{'S', "������� ����������", &creature::usespells},
	{'Z', "������������ ��������� ����", &creature::usewands},
	{'U', "������������ ������", &creature::useobject},
	{'T', "������������ ����������", &creature::usetools},
	{'C', "���������� � ���-��", &creature::chat},
	{'L', "����������� �� ��������", &creature::lookobjects},
	{KeySpace, "��������� 10 �����", &creature::waitturn},
	{Ctrl + 'D', "������ ���-��", &creature::drink},
	{Ctrl + 'E', "������ ���-��", &creature::eat},
	{Ctrl + 'R', "��������� ���-��", &creature::readsomething},
	{Ctrl + 'B', "���������� ��� �������", &creature::enslave},
	{Ctrl + 'W', "����������� ������", &creature::testweapons},
	{Ctrl + 'Q', "��������� � �����", &creature::quitandsave},
	{Ctrl + 'T', "����������� �����", &creature::testpotion},
	{Ctrl + 'S', "�������� ������� ������", &creature::search},
	{Ctrl + 'P', "�������� ���� � ������", &creature::pray},
	{}};

void creature::playui() {
	current_index = Blocked;
	setbackground(render_indoor);
	while(ismodal()) {
		current_background();
		render_message();
		domodal();
		if(translate_move(this))
			continue;
		if(translate_commands(this, indoor_keys, true))
			continue;
	}
}

void creature::pause(bool interactive) {
	if(interactive) {
		current_index = Blocked;
		while(ismodal()) {
			current_background();
			render_message("������");
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
	sb.clear();
}

bool nameable::askyn() {
	current_index = Blocked;
	while(ismodal()) {
		current_background();
		render_message("Y/N");
		domodal();
		switch(hot.key) {
		case KeyEscape:
		case 'N':
			breakmodal(0);
			hot.key = 0;
			break;
		case KeyEnter:
		case 'Y':
			breakmodal(1);
			hot.key = 0;
			break;
		}
	}
	sb.clear();
	return getresult() != 0;
}

void location::show(rooma& rooms) {
	while(ismodal()) {
		rectf({0, 0, getwidth(), getheight()}, colors::form);
		auto x0 = mmaps, y0 = mmaps;
		for(auto& e : rooms) {
			rect rc;
			rc.x1 = x0 + e.x1 * mmaps;
			rc.x2 = x0 + e.x2 * mmaps;
			rc.y1 = x0 + e.y1 * mmaps;
			rc.y2 = x0 + e.y2 * mmaps;
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

static void small_avatar(int x, int y, const creature& player, unsigned flags) {
	const int sx = 128;
	const int sy = 128;
	surface scaler2(sx, sy, 32);
	color c1 = color::create(0, 0, 0);
	c1.a = 0xFF;
	auto x1 = x - (sx / 2) / 2;
	auto y1 = y - (sy - sy / 4) / 2;
	draw::blit(scaler2, 0, 0, sx, sy, 0, *draw::canvas, x1, y1, sx / 2, sy / 2);
	if(true) {
		draw::state push;
		draw::canvas = &scaler2;
		avatar(sx / 2, sy - sy / 4, player, flags, 0xFF);
	}
	draw::blit(*draw::canvas, x1, y1, sx / 2, sy / 2, 0, scaler2, 0, 0, sx, sy);
}

static void render_outdoor() {
	const int x0 = 0;
	const int y0 = 0;
	loc.worldmap(camera, true);
	auto player = creature::getactive();
	if(player) {
		auto index = game.getposition();
		auto x = x0 + loc.getx(index) * elx - camera.x;
		auto y = y0 + loc.gety(index) * ely - camera.y;
		unsigned flags;
		switch(player->getdirection()) {
		case Left:
		case LeftUp:
		case LeftDown:
			flags = ImageMirrorH;
			break;
		default:
			flags = 0;
			break;
		}
		small_avatar(x, y, *player, flags);
		render_info(*player);
	}
}

indext location::choose(indext start, bool fow, bool allow_cancel, bool outdoor) const {
	auto push_value = current_index;
	current_index = start;
	if(outdoor)
		setbackground(render_outdoor);
	else
		setbackground(render_indoor);
	while(ismodal()) {
		current_background();
		if(true) {
			char temp[512]; string sb(temp);
			if(!fow || loc.is(current_index, Explored))
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
			controls();
			break;
		}
	}
	current_index = push_value;
	return getresult();
}

void location::worldmap(point camera, bool show_fow) const {
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
	// ������ �������
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
			case Sea:
				r = getindex3(i, Sea);
				draw::image(x, y, gres(ResSea), r, 0);
				r = getindex2(i, Sea, r);
				if(r != -1)
					draw::image(x, y, gres(ResSea), r, 0);
				break;
			case Foothills:
				draw::image(x, y, gres(ResPlains), r, 0);
				draw::image(x, y, gres(ResFoothills), getindex(i, Foothills), 0);
				break;
			default:
				draw::image(x, y, gres(ResPlains), r, 0);
				break;
			}
			if(is(i, Trailed)) {
				auto r = getindex(i, Trailed);
				if(r)
					trail_images[r].render(x, y);
			}
		}
	}
	// ������� �������
	for(auto my = rc.y1; my <= rc.y2; my++) {
		if(my < 0)
			continue;
		if(my >= mmy)
			break;
		for(auto mx = rc.x2; mx >= rc.x1; mx--) {
			if(mx < 0 || mx >= mmx)
				continue;
			auto x = x0 + mx * elx - camera.x;
			auto y = y0 + my * ely - camera.y;
			auto i = get(mx, my);
			auto t = gettile(i);
			auto r = getrand(i) % 4;
			switch(t) {
			case Mountains: image(x, y, gres(ResMountains), getindex(i, Mountains), 0); break;
			case CloudPeaks: image(x, y, gres(ResCloudPeaks), getindex(i, CloudPeaks), 0); break;
			case Forest: image(x, y, gres(ResForest), getrand(i) % 3, 0); break;
			case Swamp: image(x, y, gres(ResSwamp), getrand(i) % 3, 0); break;
			}
		}
	}
	rect screen;
	adat<picture, 256> pictures;
	auto pb = pictures.begin();
	auto pe = pictures.endof();
	screen.x1 = x0 + camera.x; screen.x2 = screen.x1 + getwidth() - 1;
	screen.y1 = y0 + camera.y; screen.y2 = screen.y1 + getheight() - 1;
	screen.offset(-64, -64);
	for(auto& e : bsdata<outdoori>()) {
		auto i = e.getposition();
		if(i == Blocked)
			continue;
		point pt;
		pt.x = x0 + loc.getx(i) * elx;
		pt.y = y0 + loc.gety(i) * ely;
		if(!pt.in(screen))
			continue;
		pb->clear();
		pb->img = ResOutdoor;
		pb->frame = e.getid();
		pb->pos = pt;
		if(!pb->alpha)
			pb->alpha = 0xFF;
		pb++;
		if(pb >= pe)
			break;
	}
	pictures.count = pb - pictures.data;
	for(auto& e : pictures)
		e.render(x0 - camera.x, y0 - camera.y);
	if(show_fow)
		viewfow(x0, y0, rc);
}

static hotkey overland_keys[] = {{F1, "������� ������� �����", change_player, 0},
	{F2, "������� ������� �����", change_player, 1},
	{F3, "������� �������� �����", change_player, 2},
	{F4, "������� ���������� �����", change_player, 3},
	{Ctrl + 'M', "������� ������", gamei::help},
	{'I', "������� ���������", &creature::inventory},
	{'V', "������", &creature::backpack},
	{'Z', "����� � �������", &creature::zoomon},
	{Ctrl + 'D', "������ ���-��", &creature::drink},
	{Ctrl + 'E', "������ ���-��", &creature::eat},
	{Ctrl + 'R', "��������� ���-��", &creature::readsomething},
	{Ctrl + 'W', "����������� ������", &creature::testweapons},
	{Ctrl + 'Q', "��������� � �����", &creature::quitandsave},
	{}};

void creature::playuioverland() {
	current_index = Blocked;
	setbackground(render_outdoor);
	while(ismodal()) {
		location::setcamera(game.getposition());
		current_background();
		render_message();
		domodal();
		auto i = translate(game.getposition());
		if(i != Blocked) {
			game.move(i);
			breakmodal(0);
		} else
			translate_commands(getactive(), overland_keys, true);
	}
}

static void put_tile() {
	loc.set(current_index, current_tile);
}

static void choose_tile() {
	current_tile = (tile_s)hot.param;
}

static void place_settlement() {
	auto p = outdoori::choose();
	if(!p)
		return;
	p->index = current_index;
}

static void editor_save() {
	loc.write("game/overland.loc", true);
	loc.serialx("export/map.txt", true);
}

static void editor_read() {
	sb.add("�� ������������� ������ ������������ ����� ����������� �����?");
	if(!creature::askyn())
		return;
	loc.read("game/overland.loc", true);
	current_index = loc.get(50, 50);
	correct(camera, current_index);
}

static hotkey editor_keys[] = {{'1', "������� �������", choose_tile, Plain},
	{'2', "������� �����", choose_tile, Sea},
	{'3', "������� ������", choose_tile, Swamp},
	{'4', "������� �����", choose_tile, Foothills},
	{'5', "������� ����", choose_tile, Mountains},
	{'6', "������� ������� ����", choose_tile, CloudPeaks},
	{'7', "������� ���", choose_tile, Forest},
	{'R', "������", &location::trail},
	{KeySpace, "���������� ��������� ����", put_tile},
	{Ctrl + 'S', "��������� �����", editor_save},
	{Ctrl + 'R', "����������� �����", editor_read},
	{}};

void location::editor() {
	setbackground(render_editor);
	while(ismodal()) {
		current_background();
		if(current_index != Blocked) {
			sb.adds("��� %1.", getstr(loc.gettile(current_index)));
			sb.adds("��������� %1i,%2i (������ %3i).",
				loc.getx(current_index), loc.gety(current_index), current_index);
		}
		render_message();
		domodal();
		auto i = translate(current_index);
		if(i != Blocked)
			current_index = i;
		else
			translate_commands(0, editor_keys, false);
	}
}