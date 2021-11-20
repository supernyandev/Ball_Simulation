#pragma once
#include "included.h"
#include "objects.h";
class Engine {
	public:
		
		static const int SCREEN_WIDTH = 1440;
		static const int SCREEN_HEIGHT = 880;
		Engine(SDL_Renderer* renderer,float t);
		Ball* add_ball(Vecf pos,float r);
		void change_mass(int id);
		void setv_ball(int id, Vecf v);
		void tick();
		void handle_mouse_event(SDL_Event& e);

		void calculate_gravity();

		
private:
	float TIME_PER_TICK;
	std::vector < Ball* > balls;
	SDL_Renderer* renderer;
	void handle_left_mouse_click(SDL_Event& e);
	void handle_right_mouse_click(SDL_Event& e);
	void handle_middle_mouse_click(SDL_Event& e);
	void drawall();
	void moveall();
	void handle_hits();
};