#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1

// 用 extern 告訴編譯器：這些變數在主程式那邊，你先認得名字就好，不要在這邊重複建立！
extern Adafruit_SSD1306 display;
extern const int xPin, yPin, btPin, touchPin;  
extern float playerX, playerY; 
extern const int PLAYER_SIZE; 
extern int starX, starY;
extern const int STAR_SIZE;    
extern int score;
extern unsigned long startTime;     
extern const int GAME_DURATION; 
extern int gameState; 
extern const int CENTER_X, CENTER_Y, DEADZONE;        
extern const float MAX_STICK_RANGE, MAX_SPEED;    
extern unsigned long starSpawnTime;    
extern bool trackingReaction;      
extern float lastMoveX, lastMoveY; 
extern unsigned long reactionTimes[50];
extern int reactionCount;
extern unsigned long avgReactionTime;

// 宣告函式原型
void resetGame();
void generateCounterStar();

#endif
