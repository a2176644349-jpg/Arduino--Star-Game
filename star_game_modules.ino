/*
脑部健康小游戏
说明：通电后进入开始界面，按下独立按键后开始游戏，用操作杆移动方块的位置来捕捉星星，操作杆的z按键按下后方块会瞬间回到屏幕中心，每局30秒。
可通过分析玩家游戏表现来评估其操控力，反应力，策略水平等认知领域的能力。

此file为star game 的GitHub测试版本

*/


// 引入我們做好的三個頭文件
#include "Game_Config.h"
#include "Menu_Screens.h"
#include "Gameplay.h"

// 實體變數定義：在此處給予引腳和所有變數真正的實體與初值！
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 【修正】在這裡補上常數引腳的定義，Linker 就不會再報錯了！
const int xPin = A0;    
const int yPin = A1;    
const int btPin = 2;     
const int touchPin = 3;  

float playerX = 60.0; 
float playerY = 32.0; 
int starX, starY;
int score = 0;
unsigned long startTime;     
int gameState = 0; 

const int PLAYER_SIZE = 16; 
const int STAR_SIZE = 4;    
const int GAME_DURATION = 30; 
const int CENTER_X = 370;
const int CENTER_Y = 367;
const int DEADZONE = 60;        
const float MAX_STICK_RANGE = 450.0; 
const float MAX_SPEED = 3.5;    

unsigned long starSpawnTime = 0;    
bool trackingReaction = false;      
float lastMoveX = 1.0, lastMoveY = 0.0; 

// 【新加入】吃星緩衝控制變數
unsigned long starEatenTime = 0;
bool isInEatenDelay = false;


unsigned long reactionTimes[50]; // 補上陣列大小50
int reactionCount = 0;
unsigned long avgReactionTime = 0;

void setup() {
  Serial.begin(9600);   
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED 初始化失敗"));
    for(;;);
  }
  pinMode(btPin, INPUT_PULLUP);
  pinMode(touchPin, INPUT_PULLUP); 
  randomSeed(analogRead(A3)); 
}

void loop() {
  int btnState = digitalRead(btPin);       
  int touchState = digitalRead(touchPin); 

  switch (gameState) {
    case 0: drawMainMenu(touchState); break;
    case 1: runGameplay(btnState, touchState); break;
    case 2: drawGameOverScreen(touchState); break;
  }
  delay(15); 
}

void generateCounterStar() {
  int baseStarX = random(2, SCREEN_WIDTH - STAR_SIZE - 2);
  int baseStarY = random(12, SCREEN_HEIGHT - STAR_SIZE - 2);
  int randomChance = random(100);

  if (randomChance >= 80) {
    float moveLen = sqrt(lastMoveX * lastMoveX + lastMoveY * lastMoveY);
    float dirX = (moveLen > 0) ? (lastMoveX / moveLen) : 1.0;
    float dirY = (moveLen > 0) ? (lastMoveY / moveLen) : 0.0;
    float vectorToStarX = baseStarX - playerX;
    float vectorToStarY = baseStarY - playerY;
    float dot = vectorToStarX * dirX + vectorToStarY * dirY;

    if (dot > 0) { 
      starX = playerX - vectorToStarX;
      starY = playerY - vectorToStarY;
    } else {
      starX = baseStarX;
      starY = baseStarY;
    }
  } else {
    starX = baseStarX;
    starY = baseStarY;
  }

  if (starX < 2) starX = 2;
  if (starX > SCREEN_WIDTH - STAR_SIZE - 2) starX = SCREEN_WIDTH - STAR_SIZE - 2;
  if (starY < 12) starY = 12;
  if (starY > SCREEN_HEIGHT - STAR_SIZE - 2) starY = SCREEN_HEIGHT - STAR_SIZE - 2;

  starSpawnTime = millis();
  trackingReaction = true;
}

void resetGame() {
  playerX = 60.0; playerY = 32.0; 
  score = 0;
  reactionCount = 0;
  avgReactionTime = 0;
  lastMoveX = 1.0; lastMoveY = 0.0;
  trackingReaction = false;
  generateCounterStar();
  startTime = millis(); 
  isInEatenDelay = false; // 確保重置遊戲時不會卡在延遲狀態

}
