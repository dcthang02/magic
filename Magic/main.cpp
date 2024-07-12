#include <SDL.h>
#include <iostream>
#include <SDL_image.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	gWindow = SDL_CreateWindow("Magic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr) {
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = nullptr;
	gWindow = nullptr;
	SDL_Quit();
}

SDL_Texture* loadTexture(const std::string& path) {
	// Load ảnh từ file
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == nullptr) {
		std::cerr << "Không thể tải hình ảnh từ " << path << ": " << IMG_GetError() << std::endl;
		return nullptr;
	}

	// Tạo texture từ surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);

	if (texture == nullptr) {
		std::cerr << "Không thể tạo texture từ surface: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	return texture;
}

int main(int argc, char* args[]) {
	if (!init()) {
		std::cerr << "Failed to initialize!" << std::endl;
		return -1;
	}

	SDL_Texture* backgroundTexture = loadTexture("assets/bg.jpg");
	if (backgroundTexture == nullptr) {
		close();
		return 1;
	}

	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, backgroundTexture, nullptr, nullptr);
		SDL_RenderPresent(gRenderer);
	}

	close();
	return 0;
}