#include "gameloop.hpp"
#include "main_helper.hpp"
#include "Player.hpp"

void gameloop(SDL_Event e, bool quit, int curr_time, int last_time, float time_change, const Uint8 *keyPressed, SDL_Renderer* renderer, SDL_Rect cam){

    SDL_Texture* bg = loadFiles("Art/Tiles/Map.png");
    SDL_Texture* playerTexture = loadFiles("Art/Player/PlayerSpriteSheet.png");
	SDL_Rect cropPNG;
	SDL_Rect positionPNG;


    Player *player = new Player(playerTexture, cropPNG);

	//Enter Game Loop
    while(!quit) {
        //SDL time and delta value
        last_time = curr_time;
        curr_time = SDL_GetTicks();
        time_change = (curr_time - last_time) / 500.0f;

        //Quit application
        while(SDL_PollEvent(&e) != 0)
        {
            //Getting event
            if(e.type == SDL_QUIT)
                quit = true;
        }

        //Get current state of keyboard
        keyPressed = SDL_GetKeyboardState(NULL);

        //Move Player
        player->move(time_change, keyPressed);

        setCamera();

        //Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        //Grass image
        SDL_Rect tempRect;
        tempRect.x = 0;
        tempRect.y = 0;
        tempRect.w = cam.w;
        tempRect.h = cam.h;
        SDL_RenderCopy(renderer, bg, &cam, &tempRect);

        //Render Player
        player->render();

        //Update Screen
        SDL_RenderPresent(renderer);
    }
}
