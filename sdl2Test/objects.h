#pragma once
#include "included.h"
#include "misc.h"
class Ball
{
public:
	static const int SCREEN_WIDTH = 1440;
	static const int SCREEN_HEIGHT = 880;

	int mass;
	int ID;
	Vecf pos;
	Vecf v;
	Vecf a;
	Vecf cpos;
	Vecf cv;
	Vecf ca;
	bool hit_handled = 0;
	std::set<Ball*> hits;
	float r;
	float elapsed_time;
	Ball(int id,Vecf pos, float r, SDL_Renderer* renderer,float elapsed_time);
	void draw();
	Derirative next_derirative(Derirative d, std::vector<Ball*>& balls, float dt);

	void move( std::vector<Ball*>& balls);
	void apply();
	void bounce_off_wall();
private:
	SDL_Renderer* renderer;
	Vecf take_acceleration(float ID, Vecf pos, std::vector < Ball* >& balls);
	void draw_circle(Vecf pos, float r);


	
};

