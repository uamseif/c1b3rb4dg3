#include "base.h"
#include "wifi_creds.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

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

#define GAME_ENDED 4

struct CustomResponse {
    bool error;
    String body;
};


void printCentered(const char *text, short y, short color=WHITE, short size=1){
    int16_t x1, y1;
    uint16_t w, h;
    gfx->setTextColor(color);
    gfx->setTextSize(size);
    gfx->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    gfx->setCursor(SCREEN_WIDTH/2 - w/2, y);
    gfx->print(text);
}

void printCentered(String text, short y, short color=WHITE, short size=1){
    printCentered(text.c_str(), y, color, size);
}

class APIHandler {
    public:
        APIHandler(){
            pin = -1;
            questionNumber = -1;
            gameEnded = false;
        }

        CustomResponse sendPin(int selectedPin){
            sendRequestLeds();
            http.begin(API_PARTICIPANT);
            http.addHeader("Content-Type", "application/json");
            String args = "{\"game\": " + String(selectedPin) + "}";
            http.POST(args);
            String response = http.getString();
            http.end();

            deserializeJson(doc, response);

            for (JsonPair kv : doc.as<JsonObject>()) {
                if (kv.key() == "error") {
                    return CustomResponse{true, kv.value().as<String>()};
                } else if (kv.key() == "alias") {
                    alias = kv.value().as<String>();
                } else if (kv.key() == "uuidP") {
                    uuidP = kv.value().as<String>();
                }
            }

            pin = selectedPin;
            return CustomResponse{false, alias};
        }

        bool gameHasStarted(){
            http.begin(API_GAMES+String(pin)+"/");
            http.GET();
            String response = http.getString();
            http.end();

            deserializeJson(doc, response);

            bool hasStarted = false;
            short questionNo;
            for (JsonPair kv : doc.as<JsonObject>()) {
                hasStarted |= kv.key() == "state" && kv.value() > 1;
                questionNo = kv.key() == "questionNo" ? kv.value().as<short>() : questionNo;
            }

            if (hasStarted) questionNumber = questionNo;
            return hasStarted;
        }

        CustomResponse sendAnswer(short answer){
            sendRequestLeds();
            http.begin(API_GUESS);
            http.addHeader("Content-Type", "application/json");
            String args = "{\"game\": " + String(pin) + ", \"uuidp\": \"" + uuidP + "\", \"answer\": " + String(answer) + "}";
            int responseCode = http.POST(args);
            String response = http.getString();
            http.end();

            deserializeJson(doc, response);

            for (JsonPair kv : doc.as<JsonObject>()) {
                if (kv.key() == "error") {
                    return CustomResponse{true, "Demasiado tarde!"};
                }
            }

            return {responseCode < 200 && responseCode >= 300, "Error interno"};
        }

        bool gameIsNextQuestion(){
            http.begin(API_GAMES+String(pin)+"/");
            http.GET();
            String response = http.getString();
            http.end();

            deserializeJson(doc, response);

            bool nextQuestion = false;
            short questionNo;
            for (JsonPair kv : doc.as<JsonObject>()) {
                nextQuestion |= kv.key() == "questionNo" && kv.value() > questionNumber;
                questionNo = kv.key() == "questionNo" ? kv.value().as<short>() : questionNo;
                gameEnded |= kv.key() == "state" && kv.value().as<short>() == GAME_ENDED;
            }

            if (nextQuestion) questionNumber = questionNo;
            return nextQuestion;
        }

        bool gameHasEnded(){
            return gameEnded;
        }

    private:
        HTTPClient http;
        short pin;
        short questionNumber;
        bool gameEnded;
        String alias;
        String uuidP;
        StaticJsonDocument<256> doc;

        void sendRequestLeds(){
            for (auto i=7; i>=0; i--){
                ledState[i] = true;
                drawLeds();
                ledState[i] = false;
                delay(15+2*i);
            }
            drawLeds();
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

        void drawButtons(bool off=false){
            int selection = getSelection();
            for (auto i=0; i<4; i++){
                drawButton(i, i == selection, off);
            }
        }

        void moveSelection(short dir){
            int prev = getSelection();
            if (dir == HORIZONTAL){
                selection[1] = (selection[1] + 1) % 2;
            } else if (dir == VERTICAL){
                selection[0] = (selection[0] + 1) % 2;
            }
            auto i = getSelection();
            drawButton(prev);
            drawButton(i, true);
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

        void drawButton(short i, bool selected=false, bool off=false){
            if (selected){
                gfx->fillRoundRect(COORDS[i].x, COORDS[i].y, BUTTON_WIDTH, BUTTON_HEIGHT, ROUND, COLORS[i]);
                gfx->drawRoundRect(COORDS[i].x, COORDS[i].y, BUTTON_WIDTH, BUTTON_HEIGHT, ROUND, WHITE);
            } else {
                gfx->fillRoundRect(COORDS[i].x, COORDS[i].y, BUTTON_WIDTH, BUTTON_HEIGHT, ROUND, off ? COLORS_OFF[i] : COLORS_DIM[i]);
            }
        }
};


void answerSelectionLoop(){
    do {
        ButtonManager buttonManager;
        clearLeds();
        for (auto j = 0; j<10; j++){
            for (auto i=0; i<8; i++){
                ledState[i] = !ledState[i];
            }
            drawLeds();
            delay(50);
        }
        buttonManager.drawButtons();
        short answer = -1;
        while (answer == -1){
            buttonsLoop();
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
        
        buttonManager.drawButtons(true);
        CustomResponse response = apiHandler.sendAnswer(answer);
        gfx->fillRoundRect(0, SCREEN_HEIGHT/2 - 20, SCREEN_WIDTH, 40, 5, BLACK);
        gfx->drawRoundRect(0, SCREEN_HEIGHT/2 - 20, SCREEN_WIDTH, 40, 5, WHITE);
        printCentered(response.error ? response.body : "Respuesta enviada!", SCREEN_HEIGHT/2 - 4, response.error ? KH_RED : KH_GREEN);
        delay(500);

        short i = 0;
        do {
            waitingLeds(i);
            i = (i+1) % 8;
        } while (!apiHandler.gameIsNextQuestion() && !apiHandler.gameHasEnded());
        clearLeds();
        gfx->fillScreen(BLACK);
    } while (!apiHandler.gameHasEnded());
}

String pinSelection(){
    const short positions[3] = {SCREEN_WIDTH/2 -20-5-10, SCREEN_WIDTH/2-10, SCREEN_WIDTH/2 +10+5};
    CustomResponse response;
    short digits[3] = {0,0,0};
    short i = 0;
    
    do {
        printCentered("Introduce el    ", 14 + 9);
        printCentered("             pin", 14 + 9, KH_YELLOW);
        printCentered("que se muestra en", 14 + 18);
        printCentered("pantalla y pulsa A", 14 + 27);       

        short pin = -1;
        bool changed = true;
        do {
            buttonsLoop();
            
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

        response = apiHandler.sendPin(pin);
        if (response.error){
            gfx->fillScreen(BLACK);
            printCentered(response.body, 60, KH_RED);
        }
    } while (response.error);

    gfx->fillScreen(BLACK);
    return response.body;
}

/*
 * Sets ledState to the corresponding values, such that the leds light up
 * according to the binary representation of the ascii code of character c
 * @param c: character to be represented
 */
void assignLeds(char c){
    for (auto i=7, j=1; i>=0, j<=256; i--, j*=2){
        ledState[i] = (j & c) != 0;
    }
}

void waitingLeds(short i){
    ledState[i] = true;
    drawLeds();
    delay(950); // almost 1 second, to account for the response time of the server
    ledState[i] = false;
}

void waitingScreen(String alias){
    printCentered("Todo listo!", SCREEN_HEIGHT/6, KH_GREEN);
    printCentered("Comenzaremos en", SCREEN_HEIGHT/4 + 10);
    printCentered("breves momentos", SCREEN_HEIGHT/4 + 10 + 9);

    printCentered("Tu alias es:", SCREEN_HEIGHT/2 + 10);
    printCentered(alias, 2*SCREEN_HEIGHT/3 + 10, KH_YELLOW, 2);

    short i = 0;
    do {
        waitingLeds(i);
        i = (i+1) % 8;
    } while (!apiHandler.gameHasStarted());
    
    clearLeds();
    gfx->fillScreen(BLACK);
}

void finalScreen(String alias){
    printCentered("Fin del juego!", SCREEN_HEIGHT/6, KH_BLUE);
    printCentered("Si has ganado, no", SCREEN_HEIGHT/4 + 10);
    printCentered("apages la pantalla.", SCREEN_HEIGHT/4 + 10 + 9);

    printCentered("Tu alias es:", SCREEN_HEIGHT/2 + 10);
    printCentered(alias, 2*SCREEN_HEIGHT/3 + 10, KH_YELLOW, 2);
}

void connectWiFi(){
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
    setupBadge();
    connectWiFi();
    String alias = pinSelection();
    waitingScreen(alias);
    answerSelectionLoop();
    finalScreen(alias);
}

void loop(){}