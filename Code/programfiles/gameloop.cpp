#include "gameloop.hpp"
#include "main_helper.hpp"
#include "Player.hpp"

// Add some vars to be used below
std::string mapImgPath = "Art/Tiles/Map.png";
std::string playerImgPath = "Art/Player/player_sprite_sheet.png";

void gameloop(SDL_Event e, bool quit, int curr_time, int last_time, float time_change, const Uint8 *keyPressed, SDL_Renderer* renderer, SDL_Rect cam){

    SDL_Texture* bg = loadFiles(mapImgPath, renderer);
	  SDL_Rect cropPNG;
    SDL_Rect positionPNG;

    Player *player = new Player(playerImgPath, &cropPNG, &positionPNG, renderer);

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
        player->move(time_change, keyPressed, &cropPNG, &positionPNG);

        setCamera(cam, positionPNG);

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

        printf("Running player.render()\n");

        //Render Player
        player->render(&cropPNG, &positionPNG, renderer, &cam);

        //Update Screen
        SDL_RenderPresent(renderer);
    }
}
