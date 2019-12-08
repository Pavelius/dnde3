#include "main.h"
#include "draw.h"

int widget::choose() {
	while(draw::ismodal()) {
		rect rc = {0, 0, draw::getwidth(), draw::getheight()};
		draw::rectf(rc, colors::window);
		rc.offset(metrics::padding, metrics::padding);
		render(rc.x1, rc.y1, rc.width());
		draw::domodal();
	}
	return draw::getresult();
}