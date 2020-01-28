#include "main.h"

template<> spritei bsmeta<spritei>::elements[] = {{ResDecals, "forest_1", "���"},
{ResDecals, "forest_2", "���"},
{ResDecals, "forest_3", "���"},
{ResDecals, "swamp_1", "���"},
{ResDecals, "swamp_2", "���"},
{ResDecals, "swamp_3", "���"},
{ResDecals, "clearing", "�������"},
{ResDecals, "town", "���������"},
{ResDecals, "terinyo", "�������"},
{ResDecals, "borderland_entry", "�������"},
{ResDecals, "hmv", "����"},
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