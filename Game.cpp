#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include <cstdlib>

Map* map;
Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

//Entities
auto& playerHand(manager.addEntity());
auto& enemyHand(manager.addEntity());
auto& gunting(manager.addEntity());
auto& batu(manager.addEntity());
auto& kertas(manager.addEntity());
auto& result(manager.addEntity());

Game::Game() {
}

Game::~Game() {
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {

	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialised!" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window Created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created!" << std::endl;
		}

		isRunning = true;
	}
	map = new Map();

	//Init Entities
	gunting.addComponent<TransformComponent>(640.0f-128.0f, 600.0f, 64, 64, 1);
	gunting.addComponent<SpriteComponent>("assets/Q.png");
	
	kertas.addComponent<TransformComponent>(640.0f, 600.0f, 64, 64, 1);
	kertas.addComponent<SpriteComponent>("assets/W.png");
	
	batu.addComponent<TransformComponent>(640.0f + 128.0f, 600.0f, 64, 64, 1);
	batu.addComponent<SpriteComponent>("assets/E.png");

	playerHand.addComponent<TransformComponent>(640.0f-32.0f, 400.0f, 64, 64, 2);
	playerHand.addComponent<SpriteComponent>("assets/default.png");

	enemyHand.addComponent<TransformComponent>(640.0f - 32.0f, 100.0f, 64, 64, 2);
	enemyHand.addComponent<SpriteComponent>("assets/default.png");

	result.addComponent<TransformComponent>(640.0f - 32.0f, 250.0f, 64, 64, 2);
	result.addComponent<SpriteComponent>("assets/kopongan.png");


}

void Game::handleEvents() {
	

	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
}

bool keyPressed = false;
int playerDecision = 0;

void Game::update() {
	manager.refresh();
	manager.update();
	
	if (Game::event.type == SDL_KEYDOWN && !keyPressed) {
		switch (Game::event.key.keysym.sym) {
		case SDLK_q:
			playerHand.getComponent<SpriteComponent>().setTex("assets/gunting.png");
			Result(1, EnemyDecision());
			keyPressed = true;
			break;
		case SDLK_w:
			playerHand.getComponent<SpriteComponent>().setTex("assets/kertas.png");
			Result(3, EnemyDecision());
			keyPressed = true;
			break;
		case SDLK_e:
			playerHand.getComponent<SpriteComponent>().setTex("assets/batu.png");
			Result(2, EnemyDecision());
			keyPressed = true;
			break;
		default:
			break;
		}
	}

	if (Game::event.type == SDL_KEYUP) {
		switch (Game::event.key.keysym.sym) {
		case SDLK_q:
			keyPressed = false;
			break;
		case SDLK_w:
			keyPressed = false;
			break;
		case SDLK_e:
			keyPressed = false;
			break;
		default:
			break;
		}
	}
}


int Game::EnemyDecision() {
	int enemyDecision = 1 + (rand() % 3);
	switch (enemyDecision) {
	case 1:
		enemyHand.getComponent<SpriteComponent>().setTex("assets/gunting.png");
		break;
	case 2:
		enemyHand.getComponent<SpriteComponent>().setTex("assets/batu.png");
		break;
	case 3:
		enemyHand.getComponent<SpriteComponent>().setTex("assets/kertas.png");
		break;
	default:
		break;
	}

	return enemyDecision;
}

void Game::Result(int player, int enemy) {
	

	int resultID = 0;
	//0 draw, 1 win, 2 lose
	
	if (player == 1) {
		if (enemy == 1) {
			std::cout << "Draw!" << std::endl;
			resultID = 0;
		}
		else if (enemy == 2) {
			std::cout << "Enemy Win!" << std::endl;
			resultID = 2;
		}
		else if (enemy == 3) {
			std::cout << "Player Win!" << std::endl;
			resultID = 1;
		}
	}

	else if (player == 2) {
		if (enemy == 1) {
			std::cout << "Player Win!" << std::endl;
			resultID = 1;
		}
		else if (enemy == 2) {
			std::cout << "Draw!" << std::endl;
			resultID = 0;
		}
		else if (enemy == 3) {
			std::cout << "Enemy Win!" << std::endl;
			resultID = 2;
		}
	}

	else if (player == 3) {
		if (enemy == 1) {
			std::cout << "Enemy Win!" << std::endl;
			resultID = 2;
		}
		else if (enemy == 2) {
			std::cout << "Player Win!" << std::endl;
			resultID = 1;
		}
		else if (enemy == 3) {
			std::cout << "Draw!" << std::endl;
			resultID = 0;
		}
	}

	switch (resultID) {
	case 0:
		result.getComponent<SpriteComponent>().setTex("assets/draw.png");
		break;
	case 1:
		result.getComponent<SpriteComponent>().setTex("assets/win.png");
		break;
	case 2:
		result.getComponent<SpriteComponent>().setTex("assets/lose.png");
		break;
	default:
		break;
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	map->DrawMap();

	manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;

}
