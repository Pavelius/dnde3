#include "main.h"

template<> spritei bsmeta<spritei>::elements[] = {{ResDecals, "forest_1", "лес"},
{ResDecals, "forest_2", "лес"},
{ResDecals, "forest_3", "лес"},
{ResDecals, "swamp_1", "лес"},
{ResDecals, "swamp_2", "лес"},
{ResDecals, "swamp_3", "лес"},
{ResDecals, "clearing", "хуторок"},
{ResDecals, "town", "поселение"},
{ResDecals, "terinyo", "деревня"},
{ResDecals, "borderland_entry", "поселок"},
{ResDecals, "hmv", "форт"},
};
DECLFULL(spritei);

static void initialize_resource(img_s i) {
	auto index = 0;
	for(auto& e : bsmeta<spritei>()) {
		if(e.image != i)
			continue;
		e.frame = index++;
	}
}

void spritei::initialize() {
	initialize_resource(ResDecals);
}