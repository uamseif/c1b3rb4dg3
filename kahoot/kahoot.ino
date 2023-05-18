#include "base.h"
#include "wifi_creds.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

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

#define UP_ARROW 0x18
#define DOWN_ARROW 0x19

#define API_URL "http://chatarritant.sytes.net:8000/api/"
#define API_PARTICIPANT API_URL "participant/"
#define API_GAMES API_URL "games/"
#define API_GUESS API_URL "guess/"


const char* funny_text = "Hola mundo!";

struct CustomResponse {
    bool error;
    String body;
};

class APIHandler {
    public:
        CustomResponse send_pin(int selectedPin){
            send_request_leds();
            http.begin(API_PARTICIPANT);
            http.addHeader("Content-Type", "application/json");
            String args = "{\"game\": " + String(selectedPin) + "}";
            http.POST(args);
            String response = http.getString();
            http.end();

            JSONVar participant = JSON.parse(response);
            if (JSON.typeof(participant) == "undefined") {
                return CustomResponse{true, "Error interno"};
            }

            JSONVar keys = participant.keys();
            bool error = false;
            for (int i = 0; i < keys.length() && !error; i++) {
                error |= JSON.stringify(keys[i]) == "\"error\"";
            }

            if (error){
                return CustomResponse{true, JSONHelper(participant["error"])};
            } 

            pin = selectedPin;
            alias = JSONHelper(participant["alias"]);
            uuidP = JSONHelper(participant["uuidP"]);
            return CustomResponse{false, alias};
        }

        bool game_has_started(){
            http.begin(API_GAMES+String(pin)+"/");
            http.GET();
            String response = http.getString();
            http.end();

            JSONVar game = JSON.parse(response);
            if (JSON.typeof(game) == "undefined") {
                return false;
            }

            JSONVar keys = game.keys();
            bool hasStarted = false;
            for (int i = 0; i < keys.length() && !hasStarted; i++) {
                if (JSON.stringify(keys[i]) == "\"state\""){
                    hasStarted = int(game[keys[i]]) > 1;
                }
            }
            return hasStarted;
        }

    private:
        HTTPClient http;
        int pin;
        String alias;
        String uuidP;
        
        void send_request_leds(){
            for (int i=7; i>=0; i--){
                led_state[i] = true;
                draw_leds();
                led_state[i] = false;
                delay(15+2*i);
            }
            draw_leds();
        }   

        String JSONHelper(JSONVar json){
            String str = JSON.stringify(json);
            return str.substring(1, str.length()-1); // remove quotes 
        }
};

APIHandler apiHandler;


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

void print_centered(const char *text, short y, short color=WHITE, short size=1){
    int16_t x1, y1;
    uint16_t w, h;
    gfx->setTextColor(color);
    gfx->setTextSize(size);
    gfx->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    gfx->setCursor(SCREEN_WIDTH/2 - w/2, y);
    gfx->print(text);
}

void print_centered(String text, short y, short color=WHITE, short size=1){
    print_centered(text.c_str(), y, color, size);
}

String pin_selection(){
    const short positions[3] = {SCREEN_WIDTH/2 -20-5-10, SCREEN_WIDTH/2-10, SCREEN_WIDTH/2 +10+5};
    CustomResponse response;
    short digits[3] = {0,0,0};
    short i = 0;
    
    do {
        print_centered("Introduce el    ", 14 + 9);
        print_centered("             pin", 14 + 9, KH_YELLOW);
        print_centered("que se muestra en", 14 + 18);
        print_centered("pantalla y pulsa A", 14 + 27);       

        short pin = -1;
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
                gfx->fillRect(positions[i], SCREEN_HEIGHT/2 + 15, 20, 28, BLACK);
                for (int j=0; j<3; j++){
                    gfx->setTextSize(2);
                    gfx->drawChar(positions[j]+5, SCREEN_HEIGHT/2 + 15 - 25+7, UP_ARROW, j==i ? KH_YELLOW : BLACK, BLACK);
                    gfx->drawChar(positions[j]+5, SCREEN_HEIGHT/2 + 15 - 25+7 + 2, UP_ARROW, BLACK, BLACK);
                    gfx->drawChar(positions[j]+5, SCREEN_HEIGHT/2 + 15 + 24+7, DOWN_ARROW, j==i ? KH_YELLOW : BLACK, BLACK);
                    gfx->drawChar(positions[j]+5, SCREEN_HEIGHT/2 + 15 + 24+7 - 2, DOWN_ARROW, BLACK, BLACK);

                    gfx->setTextSize(4);
                    gfx->setTextColor(j==i ? WHITE : DARKGREY);
                    gfx->setCursor(positions[j], SCREEN_HEIGHT/2 + 15);
                    gfx->print(digits[j]);
                    gfx->setTextColor(j==i ? WHITE : BLACK);
                }
                changed = false;
            }

            if(buttonA.isPressed()){
                pin = digits[0]*100 + digits[1]*10 + digits[2];
            }
        } while (pin == -1);

        response = apiHandler.send_pin(pin);
        if (response.error){
            gfx->fillScreen(BLACK);
            print_centered(response.body.c_str(), 60, KH_RED);
        }
    } while (response.error);

    gfx->fillScreen(BLACK);
    return response.body;
}

/*
 * Sets led_state to the corresponding values, such that the leds light up
 * according to the binary representation of the ascii code of character c
 * @param c: character to be represented
 */
void assign_leds(char c){
    for (int i=7, j=1; i>=0, j<=256; i--, j*=2){
        led_state[i] = (j & c) != 0;
    }
}

void waiting_screen(String alias){
    gfx->fillScreen(BLACK);
    print_centered("Todo listo!", SCREEN_HEIGHT/6, KH_GREEN);
    print_centered("Comenzaremos en", SCREEN_HEIGHT/4);
    print_centered("breves momentos", SCREEN_HEIGHT/4 + 9);

    print_centered("Tu alias es:", SCREEN_HEIGHT/2);
    print_centered(alias, 2*SCREEN_HEIGHT/3, KH_YELLOW, 2);

    int i = 0;
    do {
        led_state[i] = true;
        draw_leds();
        delay(1000);
        led_state[i] = false;
        i = (i+1) % 8;
    } while (!apiHandler.game_has_started());
    
    clear_leds();
    gfx->fillScreen(BLACK);
}

void connect_wifi(){
    gfx->fillScreen(BLACK);
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWD);
    gfx->print("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        gfx->print(".");
    }
    gfx->println("");
    gfx->print("Connected. IP Address: ");
    gfx->println(WiFi.localIP());
    delay(2000);
    gfx->fillScreen(BLACK);
}

void setup(){
    setup_badge();
    connect_wifi();
    String alias = pin_selection();
    waiting_screen(alias);
    answer_selection_loop();
}

void loop(){

}
