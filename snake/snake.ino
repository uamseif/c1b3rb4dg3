#include "base.h"

#define GRID_SIZE               8
#define COLS                    SCREEN_WIDTH/GRID_SIZE 
#define ROWS                    SCREEN_HEIGHT/GRID_SIZE 
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
  gfx->fillRect(snake.body[0].x * GRID_SIZE, snake.body[0].y * GRID_SIZE, GRID_SIZE, GRID_SIZE, BLACK);
  gameMap[snake.body[0].x][snake.body[0].y] = false;
  for(int j=1; j<snake.len; j++){
    snake.body[j-1]= snake.body[j];
  }
  gameMap[snake.head.x][snake.head.y] = true;
}

void updateSnake(){
  coord newHead = {snake.head.x+snake.dir.x, snake.head.y+snake.dir.y};

  //Handle Borders
  if(newHead.x == COLS){
    newHead.x = 0;
  }else if(newHead.x == -1){
    newHead.x = COLS-1;
  }else if(newHead.y == ROWS){
    newHead.y = 0;
  }else if(newHead.y == -1){
    newHead.y = ROWS-1;
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
  }else{
    removeFirst(); //Shifting the array to the left
  }
  
  snake.body[snake.len-1] = newHead;
  snake.head = newHead;
  snake.prevDir = snake.dir;
}

void renderSnake(){
  gfx->fillRect(apple.x * GRID_SIZE, apple.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, RED);
  gfx->drawRect(apple.x * GRID_SIZE, apple.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, BLACK);
  for (int i=0; i<snake.len-1; i++){
    gfx->fillRect(snake.body[i].x * GRID_SIZE, snake.body[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE, WHITE);
    gfx->drawRect(snake.body[i].x * GRID_SIZE, snake.body[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE, BLACK);
  }
  gfx->fillRect(snake.head.x * GRID_SIZE, snake.head.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, GREEN);
  gfx->drawRect(snake.head.x * GRID_SIZE, snake.head.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, BLACK);
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
  for(int i=0; i<ROWS; i++){
    gfx->drawLine(0, i*GRID_SIZE, SCREEN_WIDTH, i*GRID_SIZE, WHITE);
    delay(50);
  }
  //draw text GAME OVER centre of the screen with withe background
  int offset = 6;
  gfx->fillRect(SCREEN_WIDTH/2-27-offset, SCREEN_HEIGHT/2-3-offset, 54+2*offset, 7+2*offset, WHITE);
  gfx->setCursor(SCREEN_WIDTH/2-27, SCREEN_HEIGHT/2-3);
  gfx->setTextColor(RED);
  gfx->setTextSize(1);
  gfx->print("GAME OVER");

  delay(3000);
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
  setupGame();
  splashScreen();
}

void loop() {
  snakeLoop();
}