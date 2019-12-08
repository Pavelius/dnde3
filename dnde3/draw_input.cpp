#include "main.h"
#include "draw.h"

using namespace draw;

static bool		break_modal;
static int		break_result;
eventproc		draw::domodal;
const int		gui_border = 8;
const int		gui_padding = 4;
const int		elx = 64; // Map tile element width
const int		ely = 48; // Map tile element height
static point	viewport;
static point	camera;

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
	draw::create(-1, -1, 800, 600, WFResize | WFMinmax, 32);
}

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
			draw::image(x, y, gres(ResPCmac), 5, flags, alpha);
			break;
		default:
			draw::image(x, y, gres(ResPCmac), 4, flags, alpha);
			break;
		}
	}
	// Weapon on back
	if(ranged && getorder(ranged.gettype()) == 0) {
		auto index = getaccindex(ranged.gettype());
		if(index != -1)
			draw::image(x, y, gres(ResPCmac), index, flags, alpha);
	}
	// Character body
	draw::image(x, y, gres(ResPCmar), i1, flags, alpha);
	draw::image(x, y, gres(ResPCmbd), (race - Human) * 6 + (gender - Male) * 3 + at, flags, alpha);
	// Weapon on belt
	if(ranged && getorder(ranged.gettype()) == 2) {
		auto index = getaccindex(ranged.gettype());
		if(index != -1)
			draw::image(x, y, gres(ResPCmac), index, flags, alpha);
	}
	// Hand
	draw::image(x, y, gres(ResPCmar), i2, flags, alpha);
}

void location::worldmap(point camera, bool show_fow) const {
	auto plain = gres(ResPlains);
	auto hills = gres(ResFoothills);
	auto mount = gres(ResMountains);
	auto tmount = gres(ResCloudPeaks);
	auto sea = gres(ResSea);
	auto decals = gres(ResDecals);
	viewport.x = draw::getwidth();
	viewport.y = draw::getheight();
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
	camera.x -= elx / 2;
	camera.y -= ely / 2;
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
				draw::image(x, y, plain, r, 0);
				break;
			case Sea:
				draw::image(x, y, sea, getindex(i, Sea), 0);
				break;
			case Foothills:
				draw::image(x, y, plain, r, 0);
				draw::image(x, y, hills, getindex(i, Foothills), 0);
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
				draw::image(x, y, mount, getindex(i, Mountains), 0);
				break;
			case CloudPeaks:
				draw::image(x, y, tmount, getindex(i, CloudPeaks), 0);
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

void location::choose() const {
	while(ismodal()) {
		worldmap(camera, true);
		domodal();
	}
}