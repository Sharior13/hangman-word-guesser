#include <stdio.h>
#include "drawing.h"

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
 
    char rbuf[32], hbuf[32];
    snprintf(rbuf, sizeof(rbuf), "Round: %d", round);
    snprintf(hbuf, sizeof(hbuf), "Best Score: %d", highScore);
    int rw = MeasureText(rbuf, 36), hw = MeasureText(hbuf, 36);
    DrawText(rbuf, (SCREEN_WIDTH - rw) / 2, 500, 36, COLOR_TEXT);
    DrawText(hbuf, (SCREEN_WIDTH - hw) / 2, 550, 36, COLOR_TEXT);
 
    //draw start screen buttons
    drawButton(&buttons->play);
    drawButton(&buttons->settings);
    drawButton(&buttons->quit);
}

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