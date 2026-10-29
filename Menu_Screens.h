#ifndef MENU_SCREENS_H
#define MENU_SCREENS_H

#include "Game_Config.h"

void drawMainMenu(int touchState) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(31, 8); 
  display.print("REFLEX TEST");
  display.drawFastHLine(10, 20, 108, WHITE);
  display.setCursor(12, 32);
  display.print("Joy: 360 Move");
  display.setCursor(12, 44);
  display.print("Touch BTN to START!");
  display.drawRect(0, 0, 128, 64, WHITE);
  display.display();

  if (touchState == LOW) {
    delay(250); 
    resetGame();
    gameState = 1; 
  }
}

void drawGameOverScreen(int touchState) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);
  display.print("GAME OVER! Score: ");
  display.print(score);
  display.setCursor(5, 20);
  display.print("Avg Reflex: ");
  if (reactionCount > 0) {
    display.print(avgReactionTime);
    display.print(" ms");
  } else {
    display.print("N/A");
  }
  display.setCursor(5, 35);
  display.print("Rating: ");
  display.setTextSize(2); 
  
  if (reactionCount == 0) display.print("N/A");
  else if (avgReactionTime < 400) display.print("GOD!");   
  else if (avgReactionTime < 800) display.print("FAST");   
  else if (avgReactionTime < 1200) display.print("NORM");   
  else display.print("SLOW");   
  
  display.setTextSize(1);
  display.setCursor(10, 54); 
  display.print("Press Touch to Menu"); 
  display.display();

  if (touchState == LOW) {
    delay(300); 
    gameState = 0; 
  }
}

#endif
