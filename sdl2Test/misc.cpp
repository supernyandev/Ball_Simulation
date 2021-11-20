#include "included.h"
float point2_distance(Vecf pos1,Vecf pos2) {
	return (pos1-pos2).size();
}


Vecf operator*(float a,Vecf v) {
	return v * a;
}

