#include <stdio.h>
#include <string.h>
#include "drawing.h"

//functions for playing screen
void drawHangman(int wrongCount){
    const int gallowsX = 420;
    const int gallowsY = 220;
    int parts = wrongCount;
    if(parts > 6){
        parts = 6;
    } 
 
    float thickness = 10.0f;
 
    //always draw the gallows structure
    //draw base
    DrawRectangle(gallowsX - 160, gallowsY + 640, 320, thickness, COLOR_GALLOWS);
    //draw vertical pole
    DrawRectangle(gallowsX - 20, gallowsY, thickness, 650, COLOR_GALLOWS);
    //draw horizontal pole
    DrawRectangle(gallowsX - 20, gallowsY, 240, thickness, COLOR_GALLOWS);
    //draw rope
    DrawRectangle(gallowsX + 210, gallowsY, thickness, 100, COLOR_GALLOWS);
 
    //draw head
    if(parts >= 1){
        DrawCircleLines(gallowsX + 215, gallowsY + 150, 50, COLOR_BODY);
        DrawCircle(gallowsX + 215, gallowsY + 150, 46, Fade(COLOR_BODY, 0.15f));
    }
    //draw body
    if(parts >= 2){
        DrawLineEx(
            (Vector2){ gallowsX + 215, gallowsY + 200 },
            (Vector2){ gallowsX + 215, gallowsY + 400 },
            thickness, COLOR_BODY
        );
    }
    //draw left arm
    if(parts >= 3){
        DrawLineEx(
            (Vector2){ gallowsX + 215, gallowsY + 260 },
            (Vector2){ gallowsX + 130, gallowsY + 340 },
            thickness, COLOR_BODY
        );
    }
    //draw right arm
    if(parts >= 4){
        DrawLineEx(
            (Vector2){ gallowsX + 215, gallowsY + 260 },
            (Vector2){ gallowsX + 300, gallowsY + 340 },
            thickness, COLOR_BODY
        );
    }
    //draw left leg
    if(parts >= 5){
        DrawLineEx(
            (Vector2){ gallowsX + 215, gallowsY + 400 },
            (Vector2){ gallowsX + 130, gallowsY + 510 },
            thickness, COLOR_BODY
        );
    }
    //draw right leg
    if(parts >= 6){
        DrawLineEx(
            (Vector2){ gallowsX + 215, gallowsY + 400 },
            (Vector2){ gallowsX + 300, gallowsY + 510 },
            thickness, COLOR_BODY
        );
    }
 
    //draw X eyes when dead
    if(parts >= 6){
        int eyesX = gallowsX + 215, eyesY = gallowsY + 150;
        DrawLineEx((Vector2){eyesX - 30, eyesY - 20}, (Vector2){eyesX - 10, eyesY + 0},  5.0f, COLOR_WRONG);
        DrawLineEx((Vector2){eyesX - 10, eyesY - 20}, (Vector2){eyesX - 30, eyesY + 0},  5.0f, COLOR_WRONG);
        DrawLineEx((Vector2){eyesX + 10, eyesY - 20}, (Vector2){eyesX + 30, eyesY + 0},  5.0f, COLOR_WRONG);
        DrawLineEx((Vector2){eyesX + 30, eyesY - 20}, (Vector2){eyesX + 10, eyesY + 0},  5.0f, COLOR_WRONG);
    }
}
 
void drawWordDisplay(const char *correctLetters, int wordSize){
    int y = 380;
    int spacing = 92;
    int totalWidth = wordSize * spacing;
    //center in the right half
    int startX = 960 + (960 - totalWidth) / 2;
 
    for(int i=0; i<wordSize; i++){
        int x = startX + i * spacing;
 
        //draw underline
        DrawRectangle(x, y + 72, 64, 5, COLOR_DIM);
 
        //assign character to buffer
        char buf[2] = { correctLetters[i], '\0' };
        if(correctLetters[i] != '_'){
            Color c = (correctLetters[i] != '\0') ? COLOR_CORRECT : COLOR_TEXT;
            DrawText(buf, x + 10, y, 68, c);
        }
    }
}
 
void drawWrongLetters(const char *wrongLetters, int wrongCount){
    int x = 1000, y = 720;
    DrawText("Wrong guesses:", x, y, 36, COLOR_DIM);
    y += 55;
 
    for(int i = 0; i<wrongCount; i++){
        //assign character to buffer
        char buf[2] = { wrongLetters[i], '\0' };
        DrawText(buf, x + i * 56, y, 46, COLOR_WRONG);
    }
}
 
void drawMessage(const char *message, int correctFlag){
    if(strlen(message) == 0){
        return;
    }
 
    Color c = COLOR_TEXT;
    if(correctFlag == 1){
        c = COLOR_CORRECT;
    }
    else if(correctFlag == 0){
        c = COLOR_WRONG;
    }
    else{
        c = COLOR_HIGHLIGHT;
    }
 
    //draw message
    int msgWidth = MeasureText(message, 34);
    DrawText(message, (SCREEN_WIDTH - msgWidth) / 2, 860, 34, c);
}
 
void drawHUD(int round, int score, int highScore, int lives){
    //draw top bar
    DrawRectangle(0, 0, SCREEN_WIDTH, 100, COLOR_PANEL);
    DrawRectangle(0, 99, SCREEN_WIDTH, 3, COLOR_BORDER);
 
    char buf[64];

    //draw round
    snprintf(buf, sizeof(buf), "ROUND %d", round);
    DrawText(buf, 40, 30, 38, COLOR_TEXT);
 
    snprintf(buf, sizeof(buf), "SCORE: %d", score);
    int scoreWidth = MeasureText(buf, 38);
    DrawText(buf, (SCREEN_WIDTH - scoreWidth) / 2, 30, 38, COLOR_HIGHLIGHT);
 
    snprintf(buf, sizeof(buf), "BEST: %d", highScore);
    int bestWidth = MeasureText(buf, 38);
    DrawText(buf, SCREEN_WIDTH - bestWidth - 40, 30, 38, COLOR_DIM);
 
    //draw live hearts
    int heartX = 1000, heartY = 580;
    DrawText("Lives:", heartX, heartY, 34, COLOR_DIM);
    for(int i=0; i<6; i++){
        Color heartColor = (i < lives) ? COLOR_CORRECT : COLOR_BORDER;
        DrawText("♥", heartX + 130 + i * 48, heartY, 38, heartColor);
    }
}
 
void drawInputBox(const char *currentInput){
    int boxWidth = 700, boxHeight = 90;
    int boxX = (SCREEN_WIDTH - boxWidth) / 2;
    int boxY = 940;
    int padding = 28;
    int maxTextWidth = boxWidth - padding * 2;
 
    DrawRectangleRounded((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.2f, 8, COLOR_PANEL);
    DrawRectangleRoundedLines((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.2f, 8, 2.0f, COLOR_BORDER);
 
    DrawText("Guess:", boxX, boxY - 46, 30, COLOR_DIM);
 
    if(strlen(currentInput) > 0){
        //shrink the font size if text is too big
        int fontSize = 44;
        while(fontSize > 18 && MeasureText(currentInput, fontSize) > maxTextWidth){
            fontSize -= 2;
        }
        int textY = boxY + (boxHeight - fontSize) / 2;
        DrawText(currentInput, boxX + padding, textY, fontSize, COLOR_TEXT);
 
        //blinking cursor
        if((GetTime() * 2) - (int)(GetTime() * 2) < 0.5){
            int textWidth = MeasureText(currentInput, fontSize);
            int currentX = boxX + padding + textWidth;
            int maxCx = boxX + boxWidth - padding;
            if (currentX > maxCx) currentX = maxCx;
            DrawRectangle(currentX, boxY + (boxHeight - fontSize) / 2 - 4, 3, fontSize + 8, COLOR_TEXT);
        }
    }
    else{
        DrawText("type & press ENTER", boxX + padding, boxY + 26, 34, COLOR_DIM);
    }
}

void drawButton(Button *btn){
    Color bg = btn->isHovered ? COLOR_HIGHLIGHT : COLOR_PANEL;
    Color border = btn->isHovered ? COLOR_HIGHLIGHT : COLOR_BORDER;
    Color text = btn->isHovered ? COLOR_BG : COLOR_TEXT;
 
    DrawRectangleRounded(btn->bounds, 0.25f, 10, bg);
    DrawRectangleRoundedLines(btn->bounds, 0.25f, 10, 2.0f, border);
 
    int fontSize = 36;
    int textWidth = MeasureText(btn->label, fontSize);
    int textX = btn->bounds.x + (btn->bounds.width  - textWidth) / 2;
    int textY = btn->bounds.y + (btn->bounds.height - fontSize) / 2;
    DrawText(btn->label, textX, textY, fontSize, text);
}
 
void drawStartScreen(int highScore, int round, StartScreenButtons *buttons){
    //title
    const char *title = "HANGMAN";
    int textSize = 140;
    int textWidth = MeasureText(title, textSize);
    DrawText(title, (SCREEN_WIDTH - textWidth) / 2, 220, textSize, COLOR_HIGHLIGHT);
 
    const char *sub = "Word Guesser";
    int subWidth = MeasureText(sub, 50);
    DrawText(sub, (SCREEN_WIDTH - subWidth) / 2, 380, 50, COLOR_DIM);
 
    //divider
    DrawRectangle(SCREEN_WIDTH / 2 - 230, 460, 460, 3, COLOR_BORDER);
 
    char roundBuf[32], highScoreBuf[32];
    snprintf(roundBuf, sizeof(roundBuf), "Round: %d", round);
    snprintf(highScoreBuf, sizeof(highScoreBuf), "Best Score: %d", highScore);
    int roundWidth = MeasureText(roundBuf, 36), hintWidth = MeasureText(highScoreBuf, 36);
    DrawText(roundBuf, (SCREEN_WIDTH - roundWidth) / 2, 500, 36, COLOR_TEXT);
    DrawText(highScoreBuf, (SCREEN_WIDTH - hintWidth) / 2, 550, 36, COLOR_TEXT);
 
    //draw start screen buttons
    drawButton(&buttons->play);
    drawButton(&buttons->settings);
    drawButton(&buttons->quit);
}

void drawSettingsScreen(Button *backBtn){
    const char *title = "SETTINGS";
    int textSize = 90;
    int textWidth = MeasureText(title, textSize);
    DrawText(title, (SCREEN_WIDTH - textWidth) / 2, 260, textSize, COLOR_HIGHLIGHT);

    const char *note = "Settings options coming soon.";
    int noteWidth = MeasureText(note, 32);
    DrawText(note, (SCREEN_WIDTH - noteWidth) / 2, 420, 32, COLOR_DIM);

    drawButton(backBtn);
}

void drawPlayingScreen(GameState *state, char *currentInput){
    //left/right panel divider
    DrawRectangle(960, 100, 3, SCREEN_HEIGHT - 100, COLOR_BORDER);
 
    //HUD (top bar)
    drawHUD(state->round, state->score, state->highScore, MAX_WRONGS - state->wrongCount);
 
    //hangman figure
    drawHangman(state->wrongCount);
 
    //word display
    drawWordDisplay(state->correctLetters, state->secretWordSize);
 
    //wrong letters
    drawWrongLetters(state->wrongLetters, state->wrongCount);
 
    //feedback/error message
    drawMessage(state->message, state->correctFlag);
 
    //input box
    drawInputBox(currentInput);
}

void drawGameOver(int won, const char *secretWord, int score, GameOverButtons *buttons){
    //dim overlay
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
 
    int panelW = 900, panelH = 480;
    int panelX = (SCREEN_WIDTH - panelW) / 2, panelY = (SCREEN_HEIGHT - panelH) / 2;
 
    DrawRectangleRounded((Rectangle){panelX, panelY, panelW, panelH}, 0.12f, 12, COLOR_PANEL);
    DrawRectangleRoundedLines((Rectangle){panelX, panelY, panelW, panelH}, 0.12f, 12, 2.0f, won ? COLOR_CORRECT : COLOR_WRONG);
 
    const char *title = won ? "YOU WIN!" : "GAME OVER";
    Color titleColor = won ? COLOR_CORRECT : COLOR_WRONG;
    int titleSize = 84;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, panelX + (panelW - titleW) / 2, panelY + 50, titleSize, titleColor);
 
    if(!won){
        char wordBuf[64];
        snprintf(wordBuf, sizeof(wordBuf), "The word was: %s", secretWord);
        int wordWidth = MeasureText(wordBuf, 38);
        DrawText(wordBuf, panelX + (panelW - wordWidth) / 2, panelY + 175, 38, COLOR_TEXT);
    }
 
    char scoreBuf[64];
    snprintf(scoreBuf, sizeof(scoreBuf), "Score: %d", score);
    int scoreWidth = MeasureText(scoreBuf, 44);
    DrawText(scoreBuf, panelX + (panelW - scoreWidth) / 2, panelY + 250, 44, COLOR_HIGHLIGHT);

    //draw buttons
    drawButton(won ? &buttons->playAgain: &buttons->tryAgain);
    drawButton(&buttons->mainMenu);
}

//check button collision
int isButtonClicked(Button *btn, Vector2 mousePos, int mousePressed){
    btn->isHovered = CheckCollisionPointRec(mousePos, btn->bounds);
    return btn->isHovered && mousePressed;
}

//factory function to create a single individual button
Button createButton(int x, int y, int width, int height, const char *label){
    //initialize struct literals with values
    Button b = { { x, y, width, height }, label, 0 };
    return b;
}

//factory function for creating start screen buttons
StartScreenButtons createStartScreenButtons(){
    int x = (SCREEN_WIDTH - START_BTN_WIDTH) / 2;
    int rowHeight = START_BTN_HEIGHT + START_BTN_GAP;
 
    //initialize struct literals with values
    StartScreenButtons buttons = {
        .play = createButton(x, START_BTN_TOP_Y + rowHeight * 0, START_BTN_WIDTH, START_BTN_HEIGHT, "Play"),
        .settings = createButton(x, START_BTN_TOP_Y + rowHeight * 1, START_BTN_WIDTH, START_BTN_HEIGHT, "Settings"),
        .quit = createButton(x, START_BTN_TOP_Y + rowHeight * 2, START_BTN_WIDTH, START_BTN_HEIGHT, "Quit"),
    };
    return buttons;
}

//factory function for creating back button
Button createBackButton(){
    int x = (SCREEN_WIDTH - START_BTN_WIDTH) / 2;
    //initialize struct literals with values and return
    return createButton(x, 900, START_BTN_WIDTH, START_BTN_HEIGHT, "Back");
}

//factory function for creating game over screen buttons
GameOverButtons createGameOverButtons(){
    int panelX = (SCREEN_WIDTH  - GAMEOVER_PANEL_WIDTH)  / 2;
    int panelY = (SCREEN_HEIGHT - GAMEOVER_PANEL_HEIGHT) / 2;
 
    //two buttons side by side
    int pairWidth = GAMEOVER_BTN_WIDTH * 2 + GAMEOVER_BTN_GAP;
    int startX = panelX + (GAMEOVER_PANEL_WIDTH - pairWidth) / 2;
    int btnY = panelY + GAMEOVER_PANEL_HEIGHT - GAMEOVER_BTN_HEIGHT - 60;
 
    GameOverButtons buttons = {
        .tryAgain = createButton(startX, btnY, GAMEOVER_BTN_WIDTH, GAMEOVER_BTN_HEIGHT, "Try Again"),
        .playAgain = createButton(startX, btnY, GAMEOVER_BTN_WIDTH, GAMEOVER_BTN_HEIGHT, "Play Again"),
        .mainMenu = createButton(startX + GAMEOVER_BTN_WIDTH + GAMEOVER_BTN_GAP, btnY, GAMEOVER_BTN_WIDTH, GAMEOVER_BTN_HEIGHT, "Main Menu"),
    };
    return buttons;
}