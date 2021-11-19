#include "included.h"
#include "vec.h"
#include "misc.h"
#include <random>
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
void Engine::handle_hits(std::vector<std::pair<Ball*, Ball*>>& hits) {
	for (auto balls2 : hits) {
		Vecf v1 = balls2.first->v;
		Vecf v2 = balls2.second->v;
		Vecf pos1 = balls2.first->pos;
		Vecf pos2 = balls2.second->pos;
		float m1 = balls2.first->mass;
		float m2 = balls2.second->mass;
		Vecf nv1 = v1 - ((2 * m2) / (m1 + m2) * ((v1-v2)*(pos1-pos2)) / (point2_distance(pos1, pos2) * point2_distance(pos1,pos2))) * (pos1-pos2);
		Vecf nv2 = v2 - (2 * m1) / (m1 + m2) * ((v1-v2)*(pos1-pos2)) / (point2_distance(pos1, pos2) * point2_distance(pos1,pos2)) * (pos2-pos1);
		balls2.first->v = nv1;
		balls2.second->v = nv2;
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
					float nx, ny,t1,t2;
					Vecf t = ball2->pos - ball1->pos;
					if (t.size() <= 0.000001f) {
						t = 10 * t;
					}
					t.norm();

					ball2->pos += t * (d - point2_distance(ball1->pos, ball2->pos))*0.5;
					ball2->pos += -1*t * (d - point2_distance(ball1->pos, ball2->pos)) * 0.5;

					
					hits.push_back({ ball1,ball2 });
				}
			}
		}
	}
	
	handle_hits(hits);
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
				ball->v.x = 0;
				ball->v.y = 0;
				
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
		if (point2_distance(Vecf(x, y), ball->cpos) <= ball->r) {
			if_inside = 1;
			bool quit = 0;
			while (!quit) {
				SDL_PollEvent(&e);
				int xcurr, ycurr;
				SDL_GetMouseState(&xcurr, &ycurr);
				if (e.type == SDL_MOUSEBUTTONUP) {
					quit = 1;
					ball->v = Vecf(xcurr, ycurr) -  Vecf(x, y);
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