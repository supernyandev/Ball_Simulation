#pragma once
#include "included.h"
#include "vec.h"
#include "objects.h"
float point2_distance(Vecf pos1, Vecf pos2);
	
std::pair<float, float> normalize(float x, float y);
float dot_product(float x1, float y1, float x2, float y2);

struct Derirative {
	Vecf v;
	Vecf a;
	Derirative(Vecf v, Vecf a) {
		this->v = v;
		this->a = a;
	}

};
Vecf operator*(float a, Vecf v);
