#include "Player.hpp"
#include "main_helper.hpp"
#include "worldObjects.hpp"

constexpr int NPC_NUM = 12;
Player::Player(std::string playerTexturePath, SDL_Renderer *renderer, int player_x, int player_y){
    // Load several textures
    playerTexture = loadFiles(playerTexturePath, renderer);
    SDL_QueryTexture(playerTexture, NULL, NULL, &cropPNG.w, &cropPNG.h);
    // These need to find a new home asap
    interactTexture = loadFiles("Art/Messages/interact.png", renderer);
    churchMessageTexture = loadFiles("Art/Messages/churchMessage.png", renderer);
    exitMessageTexture = loadFiles("Art/Messages/exitMessage.png", renderer);

    //Position of player sprite
    positionPNG.x = player_x;
    positionPNG.y = player_y;

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

// Misc stuff to collide with
SDL_Rect ponds[17] = {{1510, 564, 336, 124},{2028, 564, 337, 124},{2290, 1056, 337, 125},{1256, 1946, 460, 126},
                    {1498, 732, 93, 76}, {1085, 1929, 95, 74}, {1090, 935, 468, 32}, {998, 1068, 563, 28},
                    {873, 1840, 379, 31},{1572, 1815, 99, 31}, {1609, 1295, 288, 35}, {1999, 1295, 288, 34},
                    {1809, 1534, 289, 35},{1813, 1683, 283, 30},{1811, 1814, 287, 31}, {1812, 1943, 286, 30},
                    {2307, 1977, 657, 33}};

void Player::move(float change, const Uint8 *keyState, bool farnan, NPC *npcs, CyanBuilding cBuilding,
    BlueBuilding bBuilding, GreenBuilding gBuilding, YellowBuilding yBuilding, RedBuilding rBuilding){
    //True by default, animation is stopped if not running
    // Should probably revisit this code
    isRunning = true;
    xvel = yvel = 0;
    direction = 0;
    SDL_Rect collide = {0,0,0,0};
    int hasCollided = 0;
    if(!farnan){
        isRunning = false;
        //Determine which directional keypad arrow is clicked and apply appropriate movement / image
        if(keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) {
            yvel -= static_cast<int>(playerSpeed * change);
            direction |= UP;
            cropPNG.y = frameHeight * 3;
            isRunning = true;
        }
        if(keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) {
            yvel += static_cast<int>(playerSpeed * change);
            direction |= DOWN;
            cropPNG.y = 0;
            isRunning = true;
        }
        if(keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) {
            xvel -= static_cast<int>(playerSpeed * change);
            direction |= LEFT;
            cropPNG.y = frameHeight;
            isRunning = true;
        }
        if(keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) {
            xvel += static_cast<int>(playerSpeed * change);
            direction |= RIGHT;
            cropPNG.y = frameHeight * 2;
            isRunning = true;
        }
        // alter velocities to account for diagonal movement
        if (xvel != 0 && yvel != 0){
            xvel = static_cast<int>(xvel * 0.8);
            yvel = static_cast<int>(yvel * 0.8);
        }
        positionPNG.x += xvel;
        // Check NPC Collisions
        int i = 0;
          for(i = 0; i < NPC_NUM; i++){
              if (npcs[i].NPCCollider.checkCollision(&(positionPNG), &collide)){
                  hasCollided = 2;
              }
          }
          for(i = 0; i < 17; i++){
            if(SDL_HasIntersection(&ponds[i], &positionPNG)){
                SDL_IntersectRect(&ponds[i], &positionPNG, &collide);
                hasCollided = 4;
                break;
            }
          }
          //Check building collisions
          if (cBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (bBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (gBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (yBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (rBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if(hasCollided){
              alterPositionHoriz(&collide);
          }
          hasCollided = 0;
        positionPNG.y += yvel;
        // Check NPC Collisions
          for(i = 0; i < NPC_NUM; i++){
              if (npcs[i].NPCCollider.checkCollision(&(positionPNG), &collide)){
                  hasCollided = 2;
              }
          }
          for(i = 0; i < 17; i++){
            if(SDL_HasIntersection(&ponds[i], &positionPNG)){
                SDL_IntersectRect(&ponds[i], &positionPNG, &collide);
                hasCollided = 4;
                break;
            }
          }
          //Check building collisions
          if (cBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (bBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (gBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (yBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (rBuilding.checkCollision(&(positionPNG), &collide)) {
              hasCollided = 3;
          }
          if(hasCollided){
            alterPositionVert(&collide);
          }
          hasCollided = 0;
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
    if(isRunning && (xvel != 0.0 || yvel != 0.0)) {
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

    
    lastX = positionPNG.x;
    lastY = positionPNG.y;
}

void Player::render(SDL_Renderer *rendererPointer, SDL_Rect *cam) {
    SDL_Rect screenPos;
    screenPos.x = positionPNG.x - (*cam).x;
    screenPos.y = positionPNG.y - (*cam).y;
    screenPos.w = cropPNG.w;
    screenPos.h = cropPNG.h;
    SDL_RenderCopy(rendererPointer, playerTexture, &cropPNG, &screenPos);
}

// I'll come back to this later
bool Player::collision(SDL_Renderer *rendererPointer, const Uint8 *keyState){
    return false;
}

// void Player::alterPosition(SDL_Rect *collide) {
//     if (direction == 'L') {
//         positionPNG.x += (*collide).w;
//     }
//     if (direction == 'R') {
//         positionPNG.x -= (*collide).w;
//     }
//     if (direction == 'U') {
//         positionPNG.y += (*collide).h;
//     }
//     if (direction == 'D') {
//         positionPNG.y -= (*collide).h;
//     }
//     lastX = positionPNG.x;
//     lastY = positionPNG.y;
// }
void Player::alterPositionHoriz(SDL_Rect *collide){
    if( direction & LEFT ){
        positionPNG.x += collide->w+1;
    } else if (direction & RIGHT){
        positionPNG.x -= collide->w+1;
    }
}

void Player::alterPositionVert(SDL_Rect *collide){
    if( direction & UP){
        positionPNG.y += collide->h+1;
    } else if ( direction & DOWN){
        positionPNG.y -= collide->h+1;
    }
}

void Player::alterPosition(SDL_Rect *collide) {
    // collision from moving left
    switch (direction){
        case ( UP ):{
            positionPNG.y += (*collide).h;
            break;
        }
        case ( DOWN ):{
            positionPNG.y -= (*collide).h;
            break;
        }
        case ( LEFT ):{
            positionPNG.x += (*collide).w;
            break;
        }
        case ( RIGHT ):{
            positionPNG.x -= (*collide).w;
            break;
        }
        case ( UP|RIGHT ):{
            if(collide->w > collide->h){
                positionPNG.y += collide->h;
            } else {
                positionPNG.x -= collide->w;
            }
            break;
        }
        case ( UP|LEFT ):{
            if(collide->w > collide->h){
                positionPNG.y += collide->h;
            } else {
                positionPNG.x += collide->w;
            }
            break;
        }
        case ( DOWN|RIGHT ):{
            if(collide->w > collide->h){
                positionPNG.y -= collide->h;
            } else {
                positionPNG.x -= collide->w;
            }
            break;
        }
        case ( DOWN|LEFT ):{
            if(collide->w > collide->h){
                positionPNG.y -= collide->h;
            } else {
                positionPNG.x += collide->w;
            }
            break;
        }
    }

    lastX = positionPNG.x;
    lastY = positionPNG.y;
}

Player::~Player() {
    SDL_DestroyTexture(playerTexture);
}
