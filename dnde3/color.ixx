export module color;
import collections;

export struct color {
	unsigned char b, g, r, a;
	constexpr bool operator==(const color& e) const {
		return b == e.b && g == e.g && r == e.r && a == e.a;
	}
	constexpr bool operator!=(const color& e) const {
		return b != e.b || g != e.g || r != e.r || a != e.a;
	}
	void clear() {
		*((int*)this) = 0;
	}
	static void	convert(void* output, int width, int height, int output_bpp, const void* output_pallette, const void* input, int input_bpp, const void* input_pallette = 0, int input_scanline = 0) {
		int isc = (input_scanline == 0) ? scanline(width, input_bpp) : input_scanline;
		int osc = scanline(width, output_bpp);
		unsigned char* ip = (unsigned char*)input;
		unsigned char* op = (unsigned char*)output;
		color e;
		while(height-- > 0) {
			for(int x = 0; x < width; x++) {
				e.read(ip, x, input_bpp, input_pallette);
				e.write(op, x, output_bpp, output_pallette);
			}
			op += osc;
			ip += isc;
		}
	}
	static color create(unsigned char r, unsigned char g, unsigned char b) {
		return{b, g, r};
	}
	color darken() const {
		color result;
		result.r = r - (r >> 3);
		result.g = g - (g >> 3);
		result.b = b - (b >> 3);
		result.a = a;
		return result;
	}
	int	find(const void* pallette, int count) const {
		color* p = (color*)pallette;
		int result = 0;
		int mcr = 1000;
		int mcg = 1000;
		int mcb = 1000;
		for(int i = 0; i < count; i++) {
			color& m = p[i];
			int xcr = iabs(m.r - r);
			int xcg = iabs(m.g - g);
			int xcb = iabs(m.b - b);
			if(xcr <= mcr && xcg <= mcg && xcb <= mcb) {
				result = i;
				mcr = xcr;
				mcg = xcg;
				mcb = xcb;
			}
		}
		return result;
	}
	static void	flipv(unsigned char* bits, unsigned scanline, int height) {
		if(height < 2)
			return;
		auto p1 = bits;
		auto p2 = bits + scanline * (height - 1);
		while(p1 < p2) {
			for(unsigned i = 0; i < scanline; i++)
				iswap(p1[i], p2[i]);
			p1 += scanline;
			p2 -= scanline;
		}
	}
	color gray() const {
		unsigned char level = (r + g + b) / 3;
		return{level, level, level, a};
	}
	color lighten() const {
		color result;
		int n = r + (r >> 2);
		if(n > 255)
			result.r = 255;
		else
			result.r = n;
		n = g + (g >> 2);
		if(n > 255)
			result.g = 255;
		else
			result.g = n;
		n = b + (b >> 2);
		if(n > 255)
			result.b = 255;
		else
			result.b = n;
		result.a = a;
		return result;
	}
	color mix(const color c1, unsigned char s = 128) const {
		color result;
		result.r = (((int)r * a) + (c1.r * (255 - a))) >> 8;
		result.g = (((int)g * a) + (c1.g * (255 - a))) >> 8;
		result.b = (((int)b * a) + (c1.b * (255 - a))) >> 8;
		result.a = a;
		return result;
	}
	color negative() const {
		color result;
		result.r = ~r;
		result.g = ~g;
		result.b = ~b;
		result.a = ~a;
		return result;
	}
	void read(const void* scanline, int x, int bpp, const void* pallette = 0) {
		unsigned char* p;
		switch(bpp) {
		case 1:
			a = 0;
			break;
		case 2:
			a = 0;
			break;
		case 4:
			a = 0;
			break;
		case 8:
			p = (unsigned char*)scanline + x;
			p = (unsigned char*)&((unsigned char*)pallette)[(*p) * 4];
			r = p[0];
			g = p[1];
			b = p[2];
			a = 255;
			break;
		case 32:
			p = (unsigned char*)scanline + x * 4;
			r = p[2];
			g = p[1];
			b = p[0];
			a = p[3];
			break;
		case -32:
			p = (unsigned char*)scanline + x * 4;
			r = p[0];
			g = p[1];
			b = p[2];
			a = p[3];
			break;
		case 24:
			p = (unsigned char*)scanline + x * 3;
			r = p[2];
			g = p[1];
			b = p[0];
			a = 255;
			break;
		case -24:
			p = (unsigned char*)scanline + x * 3;
			r = p[0];
			g = p[1];
			b = p[2];
			a = 255;
			break;
		}
	}
	static void rgb2bgr(color* source, int count) {
		for(int i = 0; i < count; i++)
			iswap(source[i].r, source[i].b);
	}
	static int scanline(int width, int bpp) {
		switch(iabs(bpp)) {
		case 1:
			return ((width + 31) / 32) * 4;
		case 2:
			return ((width + 15) / 16) * 4;
		case 4:
			return ((width + 7) / 8) * 4;
		case 8:
			return ((width + 3) / 4) * 4;
		case 24:
			return ((width * 3 + 3) / 4) * 4;
		case 32:
			return width * 4;
		default:
			return 0;
		}
	}
	void write(void* scanline, int x, int bpp, const void* pallette = 0, int color_count = 0) const {
		unsigned char* p;
		switch(bpp) {
		case 1:
			break;
		case 2:
			break;
		case 4:
			if(color_count == -1)
				color_count = 16;
			break;
		case 8:
			if(color_count == -1)
				color_count = 256;
			*((unsigned char*)scanline + x) = find(pallette, color_count);
			break;
		case 32:
			((color*)scanline)[x] = *this;
			break;
		case -32:
			((color*)scanline)[x].r = b;
			((color*)scanline)[x].g = g;
			((color*)scanline)[x].b = r;
			((color*)scanline)[x].a = a;
			break;
		case 24:
			p = (unsigned char*)scanline + x * 3;
			p[0] = b;
			p[1] = g;
			p[2] = r;
			break;
		case -24:
			p = (unsigned char*)scanline + x * 3;
			p[0] = r;
			p[1] = g;
			p[2] = b;
			break;
		}
	}
};
export namespace colors {
extern color black = color::create(0, 0, 0);
extern color blue = color::create(0, 0, 255);
extern color gray = color::create(128, 128, 128);
extern color green = color::create(0, 255, 0);
extern color red = color::create(255, 0, 0);
extern color yellow = color::create(255, 244, 32);
extern color white = color::create(255, 255, 255);
}