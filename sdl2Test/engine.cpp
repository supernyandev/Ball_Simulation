#include "included.h"
#include "vec.h"
#include "misc.h"
#include <random>
#include <stack>
Engine::Engine(SDL_Renderer* renderer,float t) {
	this->TIME_PER_TICK = t;
	this->renderer = renderer;

}
Ball* Engine::add_ball(Vecf pos,float r) {
	//std::srand(std::time(nullptr));
	int id = rand();
	Ball* b = new Ball(id , pos, r, renderer,TIME_PER_TICK);
	for (int i = 0; i < balls.size(); ++i) {
		if (balls[i] == NULL) {
			balls[i] = b;
			return b;
		}
	}
	
	balls.push_back(b);
	std::cout << id << std::endl;
	return b;
}
void Engine::change_mass(int id) {
	if (id == -1) {
		for (auto ball : balls) {
			if (ball != NULL) {
				ball->mass = -ball->mass;
				break;
			}
		}
		return;

	}
	for (auto ball : balls) {
		if (ball != NULL&&ball->ID==id) {
			ball->mass = -ball->mass;
			break;
		}
	}
}
void Engine::drawall() {
	for (auto ball : balls) {
		if (ball == NULL) {
			continue;
		}
		ball->draw();

	}
}
void Engine::moveall() {
	for (auto ball : balls) {
		if (ball == NULL) {
			continue;
		}
		ball->move(balls);

	}
}
void dfs(Ball* ball) {
	if (ball == NULL || ball->hit_handled) return;
		ball->hit_handled = 1;
		auto curr_ball = ball;
		curr_ball->hit_handled = 1;
		curr_ball->bounce_off_wall();
		bool support = 0;
		if (curr_ball->hits.size() >= 3) {
			support = 1;
		}

		for (auto v_ball : curr_ball->hits) {
			dfs(v_ball);
			v_ball->bounce_off_wall();
			Vecf t = v_ball->pos - curr_ball->pos;
			float dist = point2_distance(v_ball->pos, curr_ball->pos);

			if (isnan(dist)) {
				dist = 0;
			}


			float d = v_ball->r + curr_ball->r;
			if (t.size() <= 0.000001f) {
				t = 10 * t;
			}

			t.norm();

			Vecf dposv(0, 0);
			Vecf dposc(0, 0);
			Vecf dav(0, 0);
			Vecf dac(0, 0);
			if (dist < 0.001) {
				dist = 0.001;
			}
			dav += 2000 * t / dist / v_ball->mass;

			dac += -1 * 2000 * t / dist / v_ball->mass;
			dposv += t * (d - dist) * 0.5;
			dposc += -1 * t * (d - dist) * 0.5;

			
			v_ball->pos += dposv;
			v_ball->ca += dav;
				
			

			curr_ball->ca += dac;
			if (!support) {
				curr_ball->pos += dposc;
			}

			//elastic bounce
			dist = point2_distance(v_ball->pos, curr_ball->pos);

			if (isnan(dist)) {
				dist = 1;
			}
			if (dist <= 0.0001f) {
				dist = 1;
			}
			curr_ball->bounce_off_wall();
			Vecf v1 = curr_ball->v;
			Vecf v2 = v_ball->v;

			Vecf pos1 = curr_ball->pos;
			Vecf pos2 = v_ball->pos;
			float m1 = curr_ball->mass;
			float m2 = v_ball->mass;
			Vecf nv1 = v1 - ((2 * m2) / (m1 + m2) * ((v1 - v2) * (pos1 - pos2)) / (dist * dist)) * (pos1 - pos2);
			Vecf nv2 = v2 - (2 * m1) / (m1 + m2) * ((v1 - v2) * (pos1 - pos2)) / (dist * dist) * (pos2 - pos1);

			curr_ball->cv = 0.96 * nv1;
			v_ball->cv = 0.96 * nv2;

		
	}
}
void Engine::handle_hits() {
	for (auto ball : balls) {
		//dfs
		if (ball == NULL || ball->hit_handled) continue;
		dfs(ball);

		}

	}
void Engine::tick() {
	std::vector<std::pair<Ball*, Ball*>> hits;
	
	for (int i = 0; i < balls.size();++i) {
		if (balls[i] == NULL) {
			continue;
		}
		auto ball1 = balls[i];
		for (int j = i+1; j < balls.size(); ++j) {
			if (balls[j] == NULL) {
				continue;
			}
			auto ball2 = balls[j];
			if (ball1->ID != ball2->ID) {
				float d = ball1->r + ball2->r;
				if (point2_distance(ball1->pos,ball2->pos) - ball1->r - ball2->r<=-0.0000001f) {
					
					ball1->hits.insert(ball2);
					ball2->hits.insert(ball1);
					
				}
			}
		}
	}
	
	handle_hits();
	this->moveall();
	for (auto ball : balls) {
		if (ball == NULL) {
			continue;
		}
		ball->apply();
	}
	this->drawall();
	
}
void Engine::setv_ball(int id,Vecf v) {
	if (id == -1) {
		if (balls[0] != NULL) {
			balls[0]->v = v;
			return;
		}
	}
	for (auto ball : balls) {
		if (ball == NULL) {
			continue;
		}
		if (ball->ID == id) {
			ball->v = v;
			break;
		}

	}
}
void Engine::handle_mouse_event(SDL_Event& e) {
	if (e.button.button == SDL_BUTTON_LEFT) {
		handle_left_mouse_click(e);
		
		return;
	}
	if (e.button.button == SDL_BUTTON_RIGHT) {
		handle_right_mouse_click(e);
	}
	if (e.button.button == SDL_BUTTON_MIDDLE) {
		handle_middle_mouse_click(e);
		
	}
	
	
}

void Engine::handle_left_mouse_click(SDL_Event& e) {
	int x, y;
	x = e.button.x;
	y = e.button.y;
	int if_inside = 0;
	for (auto ball : balls) {
		if (ball == NULL) {
			continue;
		}
		if (point2_distance(Vecf(x,y), ball->pos) <= ball->r) {
			if_inside = 1;
			bool quit = 1;
			while (quit) {
				SDL_PollEvent(&e);
				if (e.type == SDL_MOUSEBUTTONUP) {
					quit = 0;
				}
				SDL_GetMouseState(&x, &y);
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(renderer);
				this->tick();
				ball->pos.x = x;
				ball->pos.y = y;
				ball->cv.x = 0;
				ball->cv.y = 0;
				
				SDL_RenderPresent(renderer);
				SDL_Delay(10);
			}

		}
	}
	if (if_inside == 0) {
		bool quit = 0;
		while (!quit) {
			SDL_PollEvent(&e);
			int xcurr, ycurr;
			SDL_GetMouseState(&xcurr, &ycurr);
			if (e.type == SDL_MOUSEBUTTONUP) {
				quit = 1;
				this->add_ball(Vecf(x, y), point2_distance(Vecf(x, y),  Vecf(xcurr, ycurr)));
			}
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			lineRGBA(renderer, x, y, xcurr, ycurr, 100, 100, 100, 255);
			this->drawall();

			SDL_RenderPresent(renderer);
			SDL_Delay(10);

		}
	}
}
void Engine::handle_right_mouse_click(SDL_Event& e) {
	int x, y;
	x = e.button.x;
	y = e.button.y;
	int if_inside = 0;
	for (auto ball : balls) {
		if (ball == NULL) {
			continue;
		}
		if (point2_distance(Vecf(x, y), ball->pos) <= ball->r) {
			if_inside = 1;
			bool quit = 0;
			while (!quit) {
				SDL_PollEvent(&e);
				int xcurr, ycurr;
				SDL_GetMouseState(&xcurr, &ycurr);
				if (e.type == SDL_MOUSEBUTTONUP) {
					quit = 1;
					ball->v = Vecf(xcurr, ycurr) -  Vecf(x, y);
					ball->cv = Vecf(xcurr, ycurr) - Vecf(x, y);
				}
				
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(renderer);

				lineRGBA(renderer, x, y, xcurr, ycurr, 200, 50, 100, 255);
				
				this->drawall();
				SDL_RenderPresent(renderer);
				SDL_Delay(5);
			}

		}
	}
}
void Engine::handle_middle_mouse_click(SDL_Event& e) {
	int x, y;
	x = e.button.x;
	y = e.button.y;
	for (int i = 0; i < balls.size();++i) {
		if (balls[i] == NULL) {
			continue;
		}
		if (point2_distance(Vecf(x, y), balls[i]->pos) <= balls[i]->r) {
			balls[i] = NULL;
			break;
			}

		}
	
}