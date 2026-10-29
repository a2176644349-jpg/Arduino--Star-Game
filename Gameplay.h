#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "Game_Config.h"

// 宣告我們在 .ino 建立的新變數，讓 Gameplay.h 認得它們
extern unsigned long starEatenTime;
extern bool isInEatenDelay;

void runGameplay(int btnState, int touchState) {
  if (touchState == LOW) {
    delay(250); 
    gameState = 0;
    return;
  }
  if (btnState == LOW) {
    playerX = 60.0; playerY = 32.0;
    delay(150); 
  }

  unsigned long elapsedMillis = millis() - startTime; 
  int remainingTime = GAME_DURATION - (elapsedMillis / 1000); 
  if (remainingTime <= 0) {
    remainingTime = 0;
    if (reactionCount > 0) {
      unsigned long sum = 0;
      for (int i = 0; i < reactionCount; i++) sum += reactionTimes[i];
      avgReactionTime = sum / reactionCount;
    }
    gameState = 2; 
  }

  // ======= 核心邏輯：吃星後的 0.2 秒延遲生成檢查 =======
  if (isInEatenDelay) {
    // 如果已經過了 150 毫秒 (0.15 秒)
    if (millis() - starEatenTime >= 150) {
      isInEatenDelay = false; // 結束緩衝狀態
      generateCounterStar();   // 這時候才真正移走舊星、生成新星
    }
  }

  // 搖桿控制與計算 (在這 0.2 秒內，此段依然正常執行，方塊保持自由移動)
  int rawX = analogRead(xPin);  
  int rawY = analogRead(yPin);  
  long dx = rawX - CENTER_X;
  long dy = rawY - CENTER_Y; 

  float stickDist = sqrt(dx * dx + dy * dy);
  float currentMoveX = 0.0, currentMoveY = 0.0;

  if (stickDist > DEADZONE) {
    float rad = atan2(-dy, dx); 
    float intensity = (stickDist - DEADZONE) / (MAX_STICK_RANGE - DEADZONE);
    if (intensity > 1.0) intensity = 1.0;
    currentMoveX = cos(rad) * MAX_SPEED * intensity;
    currentMoveY = -sin(rad) * MAX_SPEED * intensity;
    playerX += currentMoveX;
    playerY += currentMoveY;

    // 只有在非緩衝期間，才追蹤反應時間（確保 T0 是新星星出現的瞬間）
    if (trackingReaction && !isInEatenDelay) {
      float targetDx = (starX + STAR_SIZE/2.0) - (playerX + PLAYER_SIZE/2.0);
      float targetDy = (starY + STAR_SIZE/2.0) - (playerY + PLAYER_SIZE/2.0);
      float dotProduct = currentMoveX * targetDx + currentMoveY * targetDy;
      if (dotProduct > 0.1) { 
        unsigned long reactionTime = millis() - starSpawnTime;
        if (reactionTime > 80 && reactionCount < 50) {
          reactionTimes[reactionCount] = reactionTime;
          reactionCount++;
        }
        trackingReaction = false; 
      }
    }
  }

  if (playerX < 0) playerX = 0;
  if (playerX > SCREEN_WIDTH - PLAYER_SIZE) playerX = SCREEN_WIDTH - PLAYER_SIZE;
  if (playerY < 12) playerY = 12; 
  if (playerY > 46) playerY = 46; 

  // ======= 碰撞偵測（只有在非緩衝期間才進行檢測，防止重複加分） =======
  if (!isInEatenDelay) {
    if (playerX < starX + STAR_SIZE && playerX + PLAYER_SIZE > starX && 
        playerY < starY + STAR_SIZE && playerY + PLAYER_SIZE > starY) {
      
      score++;                    // 分數立刻加 1
      starEatenTime = millis();    // 記錄被吃掉的時間點 (T_eaten)
      isInEatenDelay = true;       // 啟動 0.2 秒延遲狀態
      
      if (stickDist > DEADZONE) {
        lastMoveX = currentMoveX;
        lastMoveY = currentMoveY;
      }
      // 【注意】此處先不呼叫 generateCounterStar()
    }
  }

  // 繪製畫面 (在延遲期間，舊星星依然會被繪製在畫面上)
  display.clearDisplay();   
  display.drawRect((int)playerX, (int)playerY, PLAYER_SIZE, PLAYER_SIZE, WHITE); 
  display.fillRect(starX, starY, STAR_SIZE, STAR_SIZE, WHITE); 
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2, 2);
  display.print("Score:"); display.print(score);
  display.setCursor(85, 2);
  display.print("Time:"); display.print(remainingTime);
  display.display();        
}

#endif
