#include "Player.hpp"
#include "main_helper.hpp"
#include "worldObjects.hpp"

Player::Player(std::string playerTexturePath, SDL_Renderer *renderer){
			// Load several textures
			playerTexture = loadFiles(playerTexturePath, renderer);
			SDL_QueryTexture(playerTexture, NULL, NULL, &cropPNG.w, &cropPNG.h);
			// These need to find a new home asap
    	interactTexture = loadFiles("Art/Messages/interact.png", renderer);
    	churchMessageTexture = loadFiles("Art/Messages/churchMessage.png", renderer);
    	exitMessageTexture = loadFiles("Art/Messages/exitMessage.png", renderer);

    	//Position of player sprite
    	positionPNG.x = (MAP_WIDTH / 2) - (PLAYER_WIDTH / 2);
    	positionPNG.y = (MAP_HEIGHT / 2) - (PLAYER_HEIGHT / 2);

    	//Set textureWidth to current crop width
    	textureWidth = cropPNG.w;

    	//Crop player sprite surface based on number of image types (in this case 12; 3x4)
    	cropPNG.w = cropPNG.w / X_FRAMES;
    	cropPNG.h = cropPNG.h / Y_FRAMES;

    	//Constant width of our frame
    	positionPNG.w = cropPNG.w;
    	positionPNG.h = cropPNG.h;
    	frameWidth = positionPNG.w;
    	frameHeight = positionPNG.h;

    	//Running flag
    	isRunning = false;
}

void Player::move(float change, const Uint8 *keyState, bool farnan){
    //True by default, animation is stopped if not running
    // Should probably revisit this code
    isRunning = true;

		if(!farnan){
    		//Determine which directional keypad arrow is clicked and apply appropriate movement / image
    		if(keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) {
        		positionPNG.y -= playerSpeed * change;
        		cropPNG.y = frameHeight * 3;
    		}
    		else if(keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) {
        		positionPNG.y += playerSpeed * change;
        		cropPNG.y = 0;
    		}
    		else if(keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) {
        		positionPNG.x -= playerSpeed * change;
        		cropPNG.y = frameHeight;
    		}
    		else if(keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) {
        		positionPNG.x += playerSpeed * change;
        		cropPNG.y = frameHeight * 2;
    		}
				else {
		        isRunning = false;
		    }
		}
		else{
				//Determine which directional keypad arrow is clicked and apply appropriate movement / image
    		if(keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_M]) {
        		positionPNG.y -= playerSpeed * change;
        		cropPNG.y = frameHeight * 3;
    		}
    		else if(keyState[SDL_SCANCODE_DOWN] && keyState[SDL_SCANCODE_S] && keyState[SDL_SCANCODE_M]) {
        		positionPNG.y += playerSpeed * change;
        		cropPNG.y = 0;
    		}
    		else if(keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_M]) {
        		positionPNG.x -= playerSpeed * change;
        		cropPNG.y = frameHeight;
    		}
    		else if(keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_M]) {
        		positionPNG.x += playerSpeed * change;
        		cropPNG.y = frameHeight * 2;
    		}
				else {
		        isRunning = false;
		    }
			}
    //User is not clicking key, so player is no longer running


    //Animate the player sprite by flashing frames
    if(isRunning) {
        counter += change;
        if(counter >= 0.08f) {
            counter = 0;
            cropPNG.x += frameWidth;
            if(cropPNG.x >= textureWidth)
                cropPNG.x = 0;
        }
    }
    else {
        //Reset to original position since inactive
        counter = 0;
        cropPNG.x = frameWidth;
    }

    if (positionPNG.x < 0) {
        positionPNG.x += playerSpeed * change;
    }
    if (positionPNG.x + PLAYER_WIDTH > MAP_WIDTH) {
        positionPNG.x -= playerSpeed * change;
    }
    if (positionPNG.y < 0) {
        positionPNG.y += playerSpeed * change;
    }
    if (positionPNG.y + PLAYER_HEIGHT > MAP_HEIGHT) {
        positionPNG.y -= playerSpeed * change;
    }
}

void Player::render(SDL_Renderer *rendererPointer, SDL_Rect *cam) {
    SDL_Rect screenPos;

    screenPos.x = positionPNG.x - (*cam).x;
    screenPos.y = positionPNG.y - (*cam).y;
    screenPos.w = cropPNG.w;
    screenPos.h = cropPNG.h;
    SDL_RenderCopy(rendererPointer, playerTexture, &cropPNG, &screenPos);
}

bool Player::collision(SDL_Renderer *rendererPointer, const Uint8 *keyState){
		SDL_Rect castlePosition = getCastle();
		SDL_Rect messageDestination;

	  // If we're not colliding, return false;
    if(positionPNG.x + positionPNG.w <= castlePosition.x || positionPNG.x >= castlePosition.x + castlePosition.w ||
        positionPNG.y + positionPNG.h <= castlePosition.y || positionPNG.y >= castlePosition.y + castlePosition.h)
    {
        colorKeyed = 0;
        SDL_SetTextureColorMod(playerTexture, 255, 255, 255);
        return false;
    }

    SDL_SetTextureColorMod(playerTexture, 255, 0, 0);

    if (keyState[SDL_SCANCODE_X] || colorKeyed == 1){
        colorKeyed = 1;

        //Display messages
        messageDestination.x = (SCREEN_WIDTH / 2) - 450;
        messageDestination.y = 600;
        messageDestination.w = 900;
        messageDestination.h = 100;
        SDL_RenderCopy(rendererPointer, churchMessageTexture, NULL, &messageDestination);
        messageDestination.x = 0;
        messageDestination.y = 0;
        messageDestination.w = 300;
        messageDestination.h = 100;
        SDL_RenderCopy(rendererPointer, exitMessageTexture, NULL, &messageDestination);
        SDL_SetTextureColorMod(playerTexture, 255, 0, 0);
    }

    if(keyState[SDL_SCANCODE_Z] || colorKeyed == 0) {
        colorKeyed = 0;
        //Display interaction message and highlight character red when collision occurs
        messageDestination.x = 0;
        messageDestination.y = 0;
        messageDestination.w = 300;
        messageDestination.h = 100;
        SDL_RenderCopy(rendererPointer, interactTexture, NULL, &messageDestination);
    }

    return true;
}

Player::~Player() {
    SDL_DestroyTexture(playerTexture);
}
