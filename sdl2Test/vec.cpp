#include "vec.h"
#include "included.h"
Vecf::Vecf() {
	x = 0;
	y = 0;

}
Vecf::Vecf(float x, float y) {
	this->x = x;
	this->y = y;
}
Vecf::Vecf(int x, int y) {
	this->x = x;
	this->y = y;
}

Vecf Vecf::operator+(Vecf v1)
{
	return Vecf(v1.x+this->x,v1.y+this->y);

}

float Vecf::operator*(Vecf v)
{
	return v.x*x+v.y*y;

}

Vecf Vecf::operator*(float a)
{
	return  Vecf(x*a,y*a);
}
Vecf Vecf::operator-()
{
	return  Vecf(-this->x,-this->y);
}
Vecf Vecf::operator/(float a)
{
	return Vecf(x*(1/a),y*(1/a));
}

Vecf Vecf::operator-(Vecf v)
{
	return  Vecf(x-v.x,y - v.y);

}

Vecf& Vecf::operator+=(Vecf v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}


Vecf& Vecf::operator*=(float a)
{
	this->x *= a;
	this->y *= a;
	return *this;
}

Vecf& Vecf::operator-=(Vecf v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

float Vecf::size()
{
	
	return sqrt(x*x+y*y);
}

void Vecf::norm()
{
	if (this->size() == 0) {
		
		this->x = rand();
		this->y = rand();
		this->norm();
	}
	else {
		this->x /= this->size();
		this->y /= this->size();
	}
}
