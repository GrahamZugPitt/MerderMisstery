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

    xvel = yvel = 0.0;
    //Setting last positions for use in collisions
    lastX = positionPNG.x;
    lastY = positionPNG.y;

    //Set textureWidth to current crop width
    textureWidth = cropPNG.w;

    //Crop player sprite surface based on number of image types (in this case 12; 3x4)
    cropPNG.w = cropPNG.w / X_FRAMES;
    cropPNG.h = cropPNG.h / Y_FRAMES;
    cropPNG.x = 64;
    cropPNG.y = 0;
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
    xvel = yvel = 0;
    if(!farnan){
        isRunning = false;
        //Determine which directional keypad arrow is clicked and apply appropriate movement / image
        if(keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) {
            yvel -= static_cast<int>(playerSpeed * change);
            //positionPNG.y -= static_cast<int>(playerSpeed * change);
            cropPNG.y = frameHeight * 3;
            isRunning = true;
        }
        if(keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) {
            yvel += static_cast<int>(playerSpeed * change);
            //positionPNG.y += static_cast<int>(playerSpeed * change);            //yvel += playerSpeed * change;
            cropPNG.y = 0;
            isRunning = true;
        }
        if(keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) {
            xvel -= static_cast<int>(playerSpeed * change);
            //positionPNG.x -= static_cast<int>(playerSpeed * change);
            cropPNG.y = frameHeight;
            isRunning = true;
        }
        if(keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) {
            xvel += static_cast<int>(playerSpeed * change);
            //positionPNG.x += static_cast<int>(playerSpeed * change);
            cropPNG.y = frameHeight * 2;
            isRunning = true;
        }
        // try collision detection here
        if (xvel != 0 && yvel != 0){
            xvel = static_cast<int>(xvel * 0.707);
            yvel = static_cast<int>(yvel * 0.707);
        }
        positionPNG.x += xvel;
        positionPNG.y += yvel;
        std::cout << "running " << positionPNG.x << " " << positionPNG.y <<std::endl;
    } else {
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

    if (lastY > positionPNG.y) {
        direction = 'U';
        overWriteY = positionPNG.y;
    } else if (lastY < positionPNG.y) {
        direction = 'D';
    } else if (lastX < positionPNG.x) {
        direction = 'R';
    } else if (lastX > positionPNG.x) {
        direction = 'L';
    } else {
        direction = 'a';
    }
    lastX = positionPNG.x;
    lastY = positionPNG.y;
}

void Player::render(SDL_Renderer *rendererPointer, SDL_Rect *cam) {
    SDL_Rect screenPos;
    screenPos.x = positionPNG.x - (*cam).x;
    screenPos.y = positionPNG.y - (*cam).y;
    screenPos.w = cropPNG.w;
    screenPos.h = cropPNG.h;
    //std::cout << screenPos.x << " " << screenPos.y <<std::endl;
    SDL_RenderCopy(rendererPointer, playerTexture, &cropPNG, &screenPos);
}

// I'll come back to this later
bool Player::collision(SDL_Renderer *rendererPointer, const Uint8 *keyState){
    return false;
}

void Player::alterPosition(SDL_Rect *collide) {
    if (direction == 'U') {
        positionPNG.y += (*collide).h;
    }
    if (direction == 'D') {
        positionPNG.y -= (*collide).h;
    }
    if (direction == 'L') {
        positionPNG.x += (*collide).w;
    }
    if (direction == 'R') {
        positionPNG.x -= (*collide).w;
    }
    lastX = positionPNG.x;
    lastY = positionPNG.y;
}

Player::~Player() {
    SDL_DestroyTexture(playerTexture);
}
