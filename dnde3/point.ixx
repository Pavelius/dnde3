export module point;
import rect;

export struct point {
	short int x, y;
	constexpr explicit operator bool() const {
		return x || y;
	}
	constexpr bool operator!=(const point pt) const {
		return pt.x != x || pt.y != y;
	}
	constexpr bool operator==(const point pt) const {
		return pt.x == x && pt.y == y;
	}
	constexpr point operator-(const point pt) const {
		return{x - pt.x, y - pt.y};
	}
	constexpr point	operator+(const point pt) const {
		return{x + pt.x, y + pt.y};
	}
	void clear() {
		x = y = 0;
	}
	constexpr bool in(const rect& rc) const {
		return x >= rc.x1 && x <= rc.x2 && y >= rc.y1 && y <= rc.y2;
	}
	bool in(const point p1, const point p2, const point p3) const {
		int a = (p1.x - x) * (p2.y - p1.y) - (p2.x - p1.x) * (p1.y - y);
		int b = (p2.x - x) * (p3.y - p2.y) - (p3.x - p2.x) * (p2.y - y);
		int c = (p3.x - x) * (p1.y - p3.y) - (p1.x - p3.x) * (p3.y - y);
		return (a >= 0 && b >= 0 && c >= 0)
			|| (a < 0 && b < 0 && c < 0);
	}
	void set(int px, int py) {
		x = (short)px;
		y = (short)py;
	}
};