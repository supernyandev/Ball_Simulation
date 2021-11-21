#include "vec.h"
#include "included.h"
#include "misc.h"

Ball::Ball(int id,Vecf pos, float r, SDL_Renderer* renderer, float elapsed_time)
{
	this->ID = id;
	this->pos = pos;
	this->r = r;
	this->renderer = renderer;
	this->elapsed_time = elapsed_time;
	this->cpos = pos;
	cv = *new Vecf();
	ca = *new Vecf();
	mass = r * r;
}
Vecf Ball::take_acceleration(float ID,Vecf pos, std::vector < Ball* >& balls) {
	Vecf a(this->ca);
	
	
		
	return a;

}
void Ball::draw_circle(Vecf pos, float radius) {
	
	filledCircleRGBA(renderer,pos.x,pos.y, radius,
		std::min(255, std::max(0,(int)log2((v.size()*v.size())*100)*10)),
		255-std::min(255, std::max(0, (int)log2((v.size() * v.size()) * 100) * 10)),

		255,
		150.0f);
	
}
void Ball::draw() {
	draw_circle(pos, r);
}
Derirative  Ball::next_derirative(Derirative d, std::vector<Ball*>& balls,float dt) {
	Derirative new_d(*new Vecf(), *new Vecf());
	
	Vecf new_pos = pos + new_d.v * dt;;
	Vecf new_v = v + d.a * dt;
	
	Vecf as = take_acceleration(ID, new_pos ,balls);
	//std::pair<float, float> as = { 0,0 };
	new_d.a = as;
	new_d.v = new_v;
	return new_d;

}
void Ball::move( std::vector<Ball*>& balls) {
	Vecf as = take_acceleration(ID, pos, balls);
	//std::pair<float, float> as = {0,0};
	Derirative a = Derirative(v, as);
	Derirative b = next_derirative(a, balls,elapsed_time*0.5);
	Derirative c = next_derirative(b, balls, elapsed_time*0.5);
	Derirative d = next_derirative(c, balls, elapsed_time);
	//std::cout <<cvx * elapsed_time- 1.0f / 6.0f * (a.vx + 2.0f * (b.vx + c.vx) + d.vx) * elapsed_time<<' '<<cx << std::endl;
	/*cpos = pos + (1.0f / 6.0f) * (a.v + 2.0f * (b.v + c.v) + d.v) * elapsed_time;
	cv = v + (1.0f / 6.0f) * (a.a + 2.0f * (b.a + c.a) + d.a) * elapsed_time;*/
	if (cnt > 1) {
		int sa = 0;
	}
	cnt = 0;
	pos = pos + cv * elapsed_time;
	cv = cv + as * elapsed_time;

	
	
	 
}
void Ball::apply() {
	Vecf centre(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	bounce_off_wall();
	hit_handled = 0;
	hits = {};

	v = cv;
	Vecf ta = pos - centre;
	ta.norm();
	ta *= 3;
	//ca = -1*ta;
	ca.x = 0;
	ca.y = 5;

	
}
void Ball :: bounce_off_wall(){
	if (pos.x > SCREEN_WIDTH - r) {
		cv.x = -0.5*v.x;
		pos.x = SCREEN_WIDTH-r;
	}
	if (pos.y > SCREEN_HEIGHT-r) {
		cv.y = -0.5*v.y;
		pos.y = SCREEN_HEIGHT-r;
	}
	if (pos.x < r) {
		cv.x = -0.5*v.x;
		pos.x = r;
	}
	if (pos.y < r) {
		cv.y = -0.5*v.y;
		pos.y = r;
	}
}