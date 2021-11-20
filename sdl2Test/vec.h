#pragma once
class Vecf {
public:
	float x;
	float y;
	Vecf(float x, float y);
	Vecf(int x, int y);
	Vecf();

	Vecf operator+(Vecf v);
	float operator*(Vecf v);
	Vecf operator*(float a);
	Vecf operator-();
	Vecf operator/(float a);
	Vecf operator-(Vecf v);
	Vecf& operator+=(Vecf v);
	
	Vecf& operator*=(float a);
	Vecf& operator-=(Vecf v);
	float size();
	void norm();
	
};