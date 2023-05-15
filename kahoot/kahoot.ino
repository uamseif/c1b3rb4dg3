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

struct coord {
   int x;
   int y;
};

class ButtonManager {
    public:
        ButtonManager() {}

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

void setup(){
    setup_badge();
    answer_selection_loop();
}

void loop(){

}