#include "base.h"

#define HORIZONTAL 1
#define VERTICAL 2

#define KH_RED RGB565(226, 27, 60)
#define KH_BLUE RGB565(19, 104, 206)
#define KH_YELLOW RGB565(255, 166, 2)
#define KH_GREEN RGB565(38, 137, 12)

#define KH_RED_DIM RGB565(170, 20, 45)
#define KH_BLUE_DIM RGB565(14, 78, 154)
#define KH_YELLOW_DIM RGB565(191, 124, 2)
#define KH_GREEN_DIM RGB565(29, 102, 9)

#define KH_RED_OFF RGB565(85, 10, 22)
#define KH_BLUE_OFF RGB565(7, 39, 77)
#define KH_YELLOW_OFF RGB565(95, 62, 1)
#define KH_GREEN_OFF RGB565(15, 51, 4)

#define UP_ARROW 0x1E
#define DOWN_ARROW 0x1F

struct coord {
   int x;
   int y;
};

class ButtonManager {
    public:
        short getSelection(){
            return 2*selection[0] + selection[1];
        }

        void drawButtons(){
            int selection = getSelection();
            for (int i=0; i<4; i++){
                drawButton(i, i == selection);
            }
        }

        void moveSelection(short dir){
            int prev = getSelection();
            if (dir == HORIZONTAL){
                selection[1] = (selection[1] + 1) % 2;
            } else if (dir == VERTICAL){
                selection[0] = (selection[0] + 1) % 2;
            }
            int i = getSelection();
            drawButton(prev);
            drawButton(i, true);
        }

        void selectAnswer(short answer){
            for (int i=0; i<4; i++){
                drawButton(i, i == answer, true);
            }
            // TODO: send answer to server
        }

    private:
        bool selection[2] = {false, false};
        const short COLORS[4] = {KH_RED, KH_BLUE, KH_YELLOW, KH_GREEN};
        const short COLORS_DIM[4] = {KH_RED_DIM, KH_BLUE_DIM, KH_YELLOW_DIM, KH_GREEN_DIM};
        const short COLORS_OFF[4] = {KH_RED_OFF, KH_BLUE_OFF, KH_YELLOW_OFF, KH_GREEN_OFF};
        const short MARGIN_OUT = 5;
        const short MARGIN_IN = 3;
        const short ROUND = 7;
        const short BUTTON_WIDTH = SCREEN_WIDTH/2 - MARGIN_OUT - MARGIN_IN;
        const short BUTTON_HEIGHT = SCREEN_HEIGHT/2 - MARGIN_OUT - MARGIN_IN;
        const coord COORDS[4] = {
            {MARGIN_OUT, MARGIN_OUT},
            {SCREEN_WIDTH/2 + MARGIN_IN, MARGIN_OUT},
            {MARGIN_OUT, SCREEN_HEIGHT/2 + MARGIN_IN},
            {SCREEN_WIDTH/2 + MARGIN_IN, SCREEN_HEIGHT/2 + MARGIN_IN}
        };

        void drawButton(int i, bool selected=false, bool off=false){
            if (selected){
                gfx->fillRoundRect(COORDS[i].x, COORDS[i].y, BUTTON_WIDTH, BUTTON_HEIGHT, ROUND, COLORS[i]);
                gfx->drawRoundRect(COORDS[i].x, COORDS[i].y, BUTTON_WIDTH, BUTTON_HEIGHT, ROUND, WHITE);
            } else {
                gfx->fillRoundRect(COORDS[i].x, COORDS[i].y, BUTTON_WIDTH, BUTTON_HEIGHT, ROUND, off ? COLORS_OFF[i] : COLORS_DIM[i]);
            }
        }
};

ButtonManager buttonManager;

void answer_selection_loop(){
    buttonManager.drawButtons();
    short answer = -1;
    while (answer == -1){
        buttons_loop();
        if (buttonA.isPressed()){
            answer = buttonManager.getSelection();
        } else {
            short dir = 0;
            if (buttonUp.isPressed() || buttonDown.isPressed()){
                dir = VERTICAL;
            } else if (buttonRight.isPressed() || buttonLeft.isPressed()){
                dir = HORIZONTAL;
            }
            if (dir != 0) buttonManager.moveSelection(dir);
        }
    }
    buttonManager.selectAnswer(answer);
}

void pin_selection(){
    short digits[3] = {0,0,0};
    const short positions[3] = {SCREEN_WIDTH/2 -20-5-10, SCREEN_WIDTH/2-10, SCREEN_WIDTH/2 +10+5};
    short pin = 0;
    short i = 0;

    gfx->fillScreen(BLACK);
    gfx->setTextColor(WHITE);
    gfx->setTextSize(1);
    gfx->setCursor(0, 0);
    gfx->println("Introduce el pin de la pantalla. Pulsa A para continuar"); // TODO: print prettier
    bool changed = true;

    do {
        buttons_loop();
        
        if(buttonUp.isPressed()){
            digits[i] = (digits[i] + 1) % 10;
            changed = true;
        } else if (buttonDown.isPressed()){
            digits[i] = (digits[i] +9) % 10;
            changed = true;
        } else if (buttonRight.isPressed()){
            i = (i + 1) % 3;
            changed = true;
        } else if (buttonLeft.isPressed()){
            i = (i + 2) % 3;
            changed = true;
        }

        if (changed){
            gfx->fillRect(positions[i], SCREEN_HEIGHT/2 - 14, 20, 28, BLACK);
            for (int j=0; j<3; j++){
                gfx->setTextSize(2);
                gfx->drawChar(positions[j]+5, SCREEN_HEIGHT/2 - 14 - 30+7, UP_ARROW, j==i ? LIGHTGREY : BLACK, BLACK);
                gfx->drawChar(positions[j]+5, SCREEN_HEIGHT/2 - 14 + 30+7, DOWN_ARROW, j==i ? LIGHTGREY : BLACK, BLACK);

                gfx->setTextSize(4);
                gfx->setTextColor(j==i ? WHITE : DARKGREY);
                gfx->setCursor(positions[j], SCREEN_HEIGHT/2 - 14);
                gfx->print(digits[j]);
                gfx->setTextColor(j==i ? WHITE : BLACK);
            }
            
        }        

        if(buttonA.isPressed()){
            pin = digits[0]*100 + digits[1]*10 + digits[2];
        }
        changed = false;
    } while (!pin);

    // TODO: send pin to server
    // TODO: dont exit the while loop if the pin is wrong

    gfx->fillScreen(BLACK);
}

void setup(){
    setup_badge();
    pin_selection();
    // TODO: waiting screen
    answer_selection_loop();
}

void loop(){

}