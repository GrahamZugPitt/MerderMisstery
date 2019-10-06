#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

constexpr int CAM_WIDTH = 1280;
constexpr int CAM_HEIGHT = 720;
constexpr int BG_WIDTH = 6000;
constexpr int BG_HEIGHT = 3937;
constexpr int TILE_SIZE = 100;

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gBackground;
SDL_Texture* gBirdSheet;

bool init() {	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}
	
	gWindow = SDL_CreateWindow("Tiling", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, CAM_WIDTH, CAM_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Adding VSync to avoid absurd framerates
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == nullptr) {	
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	int imgFlags = IMG_INIT_PNG;
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags) {
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			
	return true;
}

SDL_Texture* loadImage(std::string fname) {
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}


void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	SDL_DestroyTexture(gBackground);
	gBackground = nullptr;

	SDL_DestroyTexture(gBirdSheet);
	gBirdSheet = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int main() {
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	
	gBackground = loadImage("images/town.png");
	gBirdSheet = loadImage("images/costume1.png");

	// Moving box
	SDL_Rect moveBox = {BG_WIDTH/2 - TILE_SIZE/2, BG_HEIGHT/2 - TILE_SIZE/2, TILE_SIZE, TILE_SIZE};	
	int x_vel = 0;
	int y_vel = 0;
	int x_deltav = 0;
	int y_deltav = 0;

	SDL_Rect camera = {BG_WIDTH/2 - CAM_WIDTH/2, BG_HEIGHT/2 - CAM_HEIGHT, CAM_WIDTH, CAM_HEIGHT};
	SDL_Rect birdcam = {CAM_WIDTH/2, CAM_HEIGHT/2, TILE_SIZE, TILE_SIZE};

	SDL_Event e;
	bool gameon = true;
	while(gameon) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				gameon = false;
			}
		}
		
		x_deltav = 0;
		y_deltav = 0;
		const Uint8* keystate = SDL_GetKeyboardState(nullptr);
		if (keystate[SDL_SCANCODE_W])
			y_deltav -= 1;
		if (keystate[SDL_SCANCODE_A])
			x_deltav -= 1;
		if (keystate[SDL_SCANCODE_S])
			y_deltav += 1;
		if (keystate[SDL_SCANCODE_D])
			x_deltav += 1;

		if (x_deltav == 0) {
			if (x_vel > 0)
				x_deltav = -1;
			else if (x_vel < 0)
				x_deltav = 1;
		}
		if (y_deltav == 0) {
			if (y_vel > 0)
				y_deltav = -1;
			else if (y_vel < 0)
				y_deltav = 1;
		}
		
		x_vel += x_deltav;
		y_vel += y_deltav;

		if (x_vel < -5)
			x_vel = -5;
		else if (x_vel > 5)
			x_vel = 5;

		if (y_vel < -5)
			y_vel = -5;
		else if (y_vel > 5)
			y_vel = 5;

		// Move bird
		moveBox.y += y_vel;
		if (moveBox.y < 0 || (moveBox.y + TILE_SIZE > BG_HEIGHT))
			moveBox.y -= y_vel;
		moveBox.x += x_vel;
		if (moveBox.x < 0 || (moveBox.x + TILE_SIZE > BG_WIDTH))
			moveBox.x -= x_vel;
			
		// Make sure bird is centered in camera
		camera.x = (moveBox.x + moveBox.w/2) - CAM_WIDTH/2;
		if (camera.x < 0)
			camera.x = 0;
		else if (camera.x + CAM_WIDTH > BG_WIDTH)
			camera.x = BG_WIDTH - CAM_WIDTH;

		camera.y = (moveBox.y + moveBox.h/2) - CAM_HEIGHT/2;
		if (camera.y < 0)
			camera.y = 0;
		else if (camera.y + CAM_HEIGHT > BG_HEIGHT)
			camera.y = BG_HEIGHT - CAM_HEIGHT;

		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);
		
		// Draw the portion of the background currently highlighted by the camera
		SDL_RenderCopy(gRenderer, gBackground, &camera, nullptr);

		// Draw the bird
		birdcam.x = moveBox.x - camera.x;
		birdcam.y = moveBox.y - camera.y;
		SDL_RenderCopy(gRenderer, gBirdSheet, NULL, &birdcam);

		SDL_RenderPresent(gRenderer);
	}

	close();
}
