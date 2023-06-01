#include "base.h"
#include "sprites.h"

#define SPRITE_WIDTH 13
#define SPRITE_HEIGHT 21
#define STANDING 0
#define WALKING 1
#define PUNCHING 2
#define JUMPING 3

#define TIME_BETWEEN_FRAMES 8000
#define FLOOR_Y 100

#define RGB565_DARKDARKGREY RGB565(50, 51, 50)
#define DARKDARKGREY RGB565_DARKDARKGREY

struct coord {
  int x;
  int y;
};

int animation_timer = 0;

//class to represent the stickman fighter
class Stickman{
    public:
        Stickman(bool isPlayer1 = true){
            if (isPlayer1){
                standingSprites = standingSpritesP1;
                walkingSprites = walkingSpritesP1;
                punchingSprites = punchingSpritesP1;
                jumpingSprite = jumpingP1;
            } else {
                standingSprites = standingSpritesP2;
                walkingSprites = walkingSpritesP2;
                punchingSprites = punchingSpritesP2;
                jumpingSprite = jumpingP2;
            }
        }

        void update(){
            buttonsLoop();
            prevPos = pos;

            if (state == JUMPING){
                if(animation_timer > TIME_BETWEEN_FRAMES){
                    if (pos.y >= FLOOR_Y && jumpVelocity > 0){
                        state = STANDING;
                    } else {
                        pos.y = min(pos.y + jumpVelocity, FLOOR_Y);
                        jumpVelocity += 2.5;
                    }
                    if (buttonRight.getState() == LOW) pos.x += 4;
                    else if (buttonLeft.getState() == LOW) pos.x -= 4;
                }
                return;
            }

            if (state == PUNCHING){
                if (animation_timer > TIME_BETWEEN_FRAMES && stopPunching){
                    state = STANDING;
                    stopPunching = false;
                }
                else if (animation_timer > TIME_BETWEEN_FRAMES){
                    stopPunching = true;
                }
                return;
            }

            if (buttonRight.getState() == LOW && animation_timer > TIME_BETWEEN_FRAMES){
                pos.x += 5;
                state = WALKING;
            } else if (buttonRight.isReleased()){
                state = STANDING;
            } else if (buttonLeft.getState() == LOW && animation_timer > TIME_BETWEEN_FRAMES){
                pos.x -= 5;
                state = WALKING;
            } else if (buttonLeft.isReleased()){
                state = STANDING;
            }
            
            if (buttonB.isPressed() && state != JUMPING){
                state = JUMPING;
                jumpVelocity = -10;
            }

            if (buttonA.isPressed() && state != JUMPING){
                state = PUNCHING;
            }
        }

        void render(){
            if (animation_timer > TIME_BETWEEN_FRAMES){
                animation_timer = 0;
                gfx->fillRect(prevPos.x, prevPos.y-SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT, BLACK);
                gfx->drawLine(prevPos.x, prevPos.y-SPRITE_HEIGHT-4, prevPos.x+SPRITE_WIDTH-2, prevPos.y-SPRITE_HEIGHT-4, BLACK);
                gfx->draw16bitRGBBitmap(pos.x, pos.y-SPRITE_HEIGHT, getSprite(), SPRITE_WIDTH, SPRITE_HEIGHT);
                gfx->drawLine(pos.x, pos.y-SPRITE_HEIGHT-4, pos.x+SPRITE_WIDTH-2, pos.y-SPRITE_HEIGHT-4, DARKDARKGREY);
                gfx->drawLine(pos.x, pos.y-SPRITE_HEIGHT-4, pos.x+SPRITE_WIDTH/2, pos.y-SPRITE_HEIGHT-4, GREEN);
            }
        }

    private:
        coord pos = {20, FLOOR_Y};
        coord prevPos = {20, FLOOR_Y};
        short state = STANDING;
        const unsigned char ** standingSprites;
        const unsigned char ** walkingSprites;
        const unsigned char ** punchingSprites;
        const unsigned char * jumpingSprite;
        short standingCounter = 0;
        short walkingCounter = 0;
        short punchingCounter = 0;
        short jumpVelocity = 0;
        bool stopPunching = false;

        const uint16_t * getSprite(){
            if (state == STANDING){
                standingCounter = (standingCounter + 1) % 2;
                return (const uint16_t *) standingSprites[standingCounter];
            } else if (state == WALKING){
                walkingCounter = (walkingCounter + 1) % 4;
                return (const uint16_t *) walkingSprites[walkingCounter];
            } else if (state == PUNCHING){
                punchingCounter = (punchingCounter + 1) % 2;
                return (const uint16_t *) punchingSprites[punchingCounter];
            } else if (state == JUMPING){
                return (const uint16_t *) jumpingSprite;
            }
        }
};




Stickman stickman;

void setup() {
    setupBadge();

    stickman = Stickman();
    gfx->drawLine(0, FLOOR_Y, 128, FLOOR_Y, DARKDARKGREY);
}

void loop() {
    if (animation_timer > TIME_BETWEEN_FRAMES){
        gfx->fillRoundRect(100, FLOOR_Y-20, 8, 20, 2, RED);
        gfx->fillRoundRect(98, FLOOR_Y-4, 12, 4, 2, DARKGREY);
    }
    stickman.update();
    stickman.render();
    animation_timer++;
}