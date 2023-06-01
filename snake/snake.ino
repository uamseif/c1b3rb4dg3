#include "base.h"

#define GAME_WIDTH              128
#define GAME_HEIGHT             128

#define OFFSET_Y                SCREEN_HEIGHT-GAME_HEIGHT

#define GRID_SIZE               8
#define COLS                    GAME_WIDTH/GRID_SIZE 
#define ROWS                    GAME_HEIGHT/GRID_SIZE 
#define INITIAL_SNAKE_SIZE      5
#define MAX_SNAKE_SIZE          COLS*ROWS
#define INITIAL_X               COLS/2
#define INITIAL_Y               ROWS/2
#define INITIAL_POS             {INITIAL_X, INITIAL_Y}

struct coord {
   int x;
   int y;
};

struct Snake {
  coord head;
  coord body[MAX_SNAKE_SIZE];
  int len;
  coord dir;
  coord prevDir;
};

void printCentered(const char *text, short y, short color=WHITE, short size=1){
    int16_t x1, y1;
    uint16_t w, h;
    gfx->setTextColor(color);
    gfx->setTextSize(size);
    gfx->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    gfx->drawRect(SCREEN_WIDTH/2 - w/2 - 1, y - 1, w + 2, h + 2, BLACK);
    gfx->setCursor(SCREEN_WIDTH/2 - w/2, y);
    gfx->print(text);
}

void printCentered(String text, short y, short color=WHITE, short size=1){
    printCentered(text.c_str(), y, color, size);
}

// Global variables for the game
Snake snake;
coord apple;
float oldTime;
float timer;
float updateRate = 5; //speed
bool restart = true;
bool gameMap[COLS][ROWS];

void setupGame(){
  snake = {
    INITIAL_POS,
    {INITIAL_POS, INITIAL_POS, INITIAL_POS, INITIAL_POS, INITIAL_POS},
    5, 
    {1,0},
    {1,0}
  };
  apple = moveApple();
  oldTime = 0;
  timer = 0;
  restart = false;

  for(int i=0; i<COLS; i++){
    for(int j=0; j<ROWS; j++){
      gameMap[i][j] = false;
    }
  }
  gameMap[snake.head.x][snake.head.y] = true;

  gfx->fillScreen(BLACK);
  printCentered("POINTS: 0", 13);
}

float calculateDeltaTime(){
  float currentTime = millis();
  float dt = currentTime - oldTime;
  oldTime = currentTime;
  return dt;
}

struct coord checkDir(){
  buttonsLoop();
  if (buttonUp.isPressed()){
    return {0,-1};
  } else if (buttonDown.isPressed()){
    return {0,1};
  } else if (buttonRight.isPressed()){
    return {1,0};
  } else if (buttonLeft.isPressed()){
    return {-1,0};
  }
  return {0,0};
}

void removeFirst(){
  gfx->fillRect(snake.body[0].x * GRID_SIZE, snake.body[0].y * GRID_SIZE + OFFSET_Y, GRID_SIZE, GRID_SIZE, BLACK);
  gameMap[snake.body[0].x][snake.body[0].y] = false;
  for(int j=1; j<snake.len; j++){
    snake.body[j-1]= snake.body[j];
  }
  gameMap[snake.head.x][snake.head.y] = true;
}

void updateSnake(){
  coord newHead = {snake.head.x+snake.dir.x, snake.head.y+snake.dir.y};

  // Uncomment to wrap arround the screen
  // if(newHead.x == COLS){
  //   newHead.x = 0;
  // }else if(newHead.x == -1){
  //   newHead.x = COLS-1;
  // }else if(newHead.y == ROWS){
  //   newHead.y = 0;
  // }else if(newHead.y == -1){
  //   newHead.y = ROWS-1;
  // }

  //Check if the snake hits the wall
  if(newHead.x == COLS || newHead.x == -1 || newHead.y == ROWS || newHead.y == -1){
    gameOver();
    return;
  }
  
  //Check If The Snake hits itself
   for(int j=0; j<snake.len; j++){
    if(snake.body[j].x == newHead.x && snake.body[j].y == newHead.y){
      gameOver();
      return;
    }
  }

  //Check if The snake ate the apple
  if(apple.x == newHead.x && apple.y == newHead.y){
    snake.len = snake.len+1;
    apple = moveApple();
    gfx->fillRect(0, 0, SCREEN_WIDTH, OFFSET_Y, BLACK);
    printCentered("POINTS: " + String(snake.len-5), 13);
  } else {
    removeFirst(); //Shifting the array to the left
  }
  
  snake.body[snake.len-1] = newHead;
  snake.head = newHead;
  snake.prevDir = snake.dir;
}

void renderSnake(){
  gfx->fillRect(apple.x * GRID_SIZE, apple.y * GRID_SIZE + OFFSET_Y, GRID_SIZE, GRID_SIZE, RED);
  gfx->drawRect(apple.x * GRID_SIZE, apple.y * GRID_SIZE + OFFSET_Y, GRID_SIZE, GRID_SIZE, BLACK);
  for (int i=0; i<snake.len-1; i++){
    gfx->fillRect(snake.body[i].x * GRID_SIZE, snake.body[i].y * GRID_SIZE + OFFSET_Y, GRID_SIZE, GRID_SIZE, LIGHTGREY);
    gfx->drawRect(snake.body[i].x * GRID_SIZE, snake.body[i].y * GRID_SIZE + OFFSET_Y, GRID_SIZE, GRID_SIZE, BLACK);
  }
  gfx->fillRect(snake.head.x * GRID_SIZE, snake.head.y * GRID_SIZE + OFFSET_Y, GRID_SIZE, GRID_SIZE, DARKGREEN);
  gfx->drawRect(snake.head.x * GRID_SIZE, snake.head.y * GRID_SIZE + OFFSET_Y, GRID_SIZE, GRID_SIZE, BLACK);

  gfx->drawRect(0, OFFSET_Y, GAME_WIDTH, GAME_HEIGHT, WHITE);
}

void snakeLoop(){
  if (restart){
    setupGame();
    return;
  }

  float deltaTime = calculateDeltaTime();
  timer += deltaTime;

  coord newDir = checkDir();
  if (newDir.x != 0 || newDir.y != 0)
    // avoid 180 deg turns
    if (newDir.x != -snake.prevDir.x || newDir.y != -snake.prevDir.y) 
      snake.dir = newDir;
  
  if(timer > 1000/updateRate){
    timer = 0;
    updateSnake();
    renderSnake();
  }
}

void gameOver(){
  restart = true;
  //draw horizontal lines from top to bottom until the screen is filled with white
  for(int h=-3*GRID_SIZE; h<SCREEN_HEIGHT; h+=GRID_SIZE){
    gfx->fillRect(0, h+3*GRID_SIZE, GAME_WIDTH, GRID_SIZE, WHITE);
    gfx->fillRect(0, h+2*GRID_SIZE, GAME_WIDTH, GRID_SIZE, LIGHTGREY);
    gfx->fillRect(0, h+GRID_SIZE,   GAME_WIDTH, GRID_SIZE, DARKGREY);
    gfx->fillRect(0, h,             GAME_WIDTH, GRID_SIZE, BLACK);
    delay(40);
  }
  delay(200);
  printCentered("GAME", SCREEN_HEIGHT/3-30, RED, 2);
  delay(400);
  printCentered("OVER", SCREEN_HEIGHT/3-10, RED, 2);
  delay(1000);
  printCentered("POINTS: " + String(snake.len-5), 2*SCREEN_HEIGHT/3-18);
  delay(500);

  int i = 0;
  while(!buttonA.isPressed()){
    buttonsLoop();
    i++;
    if(i == 50){
      printCentered("PRESS   TO RESTART", 2*SCREEN_HEIGHT/3+20, WHITE);
      printCentered("      A           ", 2*SCREEN_HEIGHT/3+20, YELLOW);
    } else if (i == 100){
      i = 0;
      printCentered("PRESS A TO RESTART", 2*SCREEN_HEIGHT/3+20, BLACK); // clear the text
    }
    delay(10);
  }
}

coord moveApple(){
  coord newApple = {(int)random(0,COLS),(int)random(0,ROWS)};
  while(gameMap[newApple.x][newApple.y]){
    newApple = {(int)random(0,COLS),(int)random(0,ROWS)};
  }
  return newApple;
}

void setup() {
  setupBadge();
  splashScreen();
  setupGame();
}

void loop() {
  snakeLoop();
}