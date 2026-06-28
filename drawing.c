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
    int panelWidth = 960;
    int panelMargin = 60;
    int maxWidth = panelWidth - panelMargin * 2;

    int spacing = 92;
    int fontSize = 68;
    int minSpacing = 28;
    
    //shrink if the word is too wide to fit
    int totalWidth = wordSize * spacing;
    while(totalWidth > maxWidth && spacing > minSpacing){
        spacing -= 4;
        fontSize -= 3;
        totalWidth = wordSize * spacing;
    }

    //center in the right half
    int startX = 960 + (960 - totalWidth) / 2;

    int underlineWidth = spacing - 28;
    if(underlineWidth < 16){
        underlineWidth = 16;
    }

    for(int i=0; i<wordSize; i++){
        int x = startX + i * spacing;
 
        //draw underline
        DrawRectangle(x, y + 72, underlineWidth, 5, COLOR_DIM);
 
        //assign character to buffer
        char buf[2] = { correctLetters[i], '\0' };
        if(correctLetters[i] != '_'){
            Color c = (correctLetters[i] != '\0') ? COLOR_CORRECT : COLOR_TEXT;
            DrawText(buf, x + 10, y, fontSize, c);
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
 
//draw heart icon from loaded texture
void drawHeart(Texture2D heartTexture, int x, int y, int size, Color tint){
    Rectangle source = { 0, 0, (float)heartTexture.width, (float)heartTexture.height };
    Rectangle dest    = { x - size / 2.0f, y - size / 2.0f, (float)size, (float)size };
    DrawTexturePro(heartTexture, source, dest, (Vector2){ 0, 0 }, 0.0f, tint);
}

void drawHUD(Texture2D heartTexture, int round, int score, int highScore, int lives){
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
        Color heartTint = (i < lives) ? WHITE : Fade(GRAY, 0.4f);
        drawHeart(heartTexture, heartX + 150 + i * 48, heartY + 20, 40, heartTint);
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
 
void drawStartScreen(Texture2D logoTexture, int highScore, int round, StartScreenButtons *buttons){
    //logo
    int logoH = 400;
    int logoW = (int)((float)logoTexture.width / logoTexture.height * logoH);
    int logoX = (SCREEN_WIDTH - logoW) / 2;
    int logoY = 60;
    Rectangle logoSrc  = { 0, 0, (float)logoTexture.width, (float)logoTexture.height };
    Rectangle logoDest = { logoX, logoY, logoW, logoH };
    DrawTexturePro(logoTexture, logoSrc, logoDest, (Vector2){ 0, 0 }, 0.0f, WHITE);
 
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

//helper function to draw volume slider in settings
float drawSlider(const char *label, float value, int sliderX, int sliderY, Vector2 mousePos, int mouseDown){

    int labelSize = 34;
    DrawText(label, sliderX, sliderY - 50, labelSize, COLOR_TEXT);

    //track
    DrawRectangleRounded((Rectangle){ sliderX, sliderY, SETTINGS_SLIDER_WIDTH, SETTINGS_SLIDER_HEIGHT }, 0.5f, 6, COLOR_BORDER);

    //filled portion
    int fillW = (int)(value * SETTINGS_SLIDER_WIDTH);
    if(fillW > 0){
        DrawRectangleRounded((Rectangle){ sliderX, sliderY, fillW, SETTINGS_SLIDER_HEIGHT }, 0.5f, 6, COLOR_HIGHLIGHT);
    }

    //knob
    int knobX = sliderX + fillW - SETTINGS_SLIDER_KNOB / 2;
    int knobY = sliderY + SETTINGS_SLIDER_HEIGHT / 2 - SETTINGS_SLIDER_KNOB / 2;
    DrawCircle(knobX + SETTINGS_SLIDER_KNOB / 2, knobY + SETTINGS_SLIDER_KNOB / 2, SETTINGS_SLIDER_KNOB / 2, COLOR_HIGHLIGHT);

    //percentage label to the right
    char pctBuf[8];
    snprintf(pctBuf, sizeof(pctBuf), "%d%%", (int)(value * 100));
    DrawText(pctBuf, sliderX + SETTINGS_SLIDER_WIDTH + 20, sliderY - 6, labelSize, COLOR_DIM);

    //drag interaction
    Rectangle hitZone = {
        sliderX - 10,
        sliderY - SETTINGS_SLIDER_KNOB,
        SETTINGS_SLIDER_WIDTH + 20,
        SETTINGS_SLIDER_HEIGHT + SETTINGS_SLIDER_KNOB * 2
    };
    if(mouseDown && CheckCollisionPointRec(mousePos, hitZone)){
        float newVal = (mousePos.x - sliderX) / (float)SETTINGS_SLIDER_WIDTH;
        if(newVal < 0.0f){
            newVal = 0.0f;
        }
        if(newVal > 1.0f){
            newVal = 1.0f;
        }
        return newVal;
    }

    return value;
}

void drawSettingsScreen(Button *backBtn, AudioSettings *settings, Vector2 mousePos, int mouseDown){
    //dim overlay
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));
    
    int panelW = SETTINGS_PANEL_WIDTH;
    int panelH = SETTINGS_PANEL_HEIGHT;
    int panelX = (SCREEN_WIDTH  - panelW) / 2;
    int panelY = (SCREEN_HEIGHT - panelH) / 2;

    //panel background and border
    DrawRectangleRounded((Rectangle){panelX, panelY, panelW, panelH}, 0.08f, 12, COLOR_PANEL);
    DrawRectangleRoundedLines((Rectangle){panelX, panelY, panelW, panelH}, 0.08f, 12, 2.0f, COLOR_BORDER);

    //title
    const char *title = "SETTINGS";
    int titleSize = 70;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, panelX + (panelW - titleW) / 2, panelY + 50, titleSize, COLOR_HIGHLIGHT);

    //divider under title
    DrawRectangle(panelX + 40, panelY + 140, panelW - 80, 2, COLOR_BORDER);

    //sliders centered inside the panel
    int sliderX = panelX + (panelW - SETTINGS_SLIDER_WIDTH) / 2;

    settings->mainVolume  = drawSlider("Main Volume",  settings->mainVolume,  sliderX, panelY + 200, mousePos, mouseDown);
    settings->musicVolume = drawSlider("Music Volume", settings->musicVolume, sliderX, panelY + 340, mousePos, mouseDown);

    drawButton(backBtn);
}

void drawPlayingScreen(Texture2D heartTexture, GameState *state, char *currentInput){
    //left/right panel divider
    DrawRectangle(960, 100, 3, SCREEN_HEIGHT - 100, COLOR_BORDER);
 
    //HUD (top bar)
    drawHUD(heartTexture, state->round, state->score, state->highScore, MAX_WRONGS - state->wrongCount);
 
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
 
    char wordBuf[64];
    snprintf(wordBuf, sizeof(wordBuf), "The word was: %s", secretWord);
    int wordWidth = MeasureText(wordBuf, 38);
    DrawText(wordBuf, panelX + (panelW - wordWidth) / 2, panelY + 175, 38, COLOR_TEXT);
 
    char scoreBuf[64];
    snprintf(scoreBuf, sizeof(scoreBuf), "Score: %d", score);
    int scoreWidth = MeasureText(scoreBuf, 44);
    DrawText(scoreBuf, panelX + (panelW - scoreWidth) / 2, panelY + 250, 44, COLOR_HIGHLIGHT);

    //draw buttons
    drawButton(won ? &buttons->playAgain: &buttons->tryAgain);
    drawButton(&buttons->mainMenu);
}

void drawPauseMenu(PauseButtons *buttons){
    //dim overlay over the paused game behind it
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f));

    int panelX = (SCREEN_WIDTH  - PAUSE_PANEL_WIDTH)  / 2;
    int panelY = (SCREEN_HEIGHT - PAUSE_PANEL_HEIGHT) / 2;

    DrawRectangleRounded((Rectangle){panelX, panelY, PAUSE_PANEL_WIDTH, PAUSE_PANEL_HEIGHT}, 0.08f, 12, COLOR_PANEL);
    DrawRectangleRoundedLines((Rectangle){panelX, panelY, PAUSE_PANEL_WIDTH, PAUSE_PANEL_HEIGHT}, 0.08f, 12, 2.0f, COLOR_BORDER);

    const char *title = "PAUSED";
    int titleW = MeasureText(title, PAUSE_TITLE_SIZE);
    DrawText(title, (SCREEN_WIDTH - titleW) / 2, panelY + 80, PAUSE_TITLE_SIZE, COLOR_HIGHLIGHT);

    drawButton(&buttons->resume);
    drawButton(&buttons->restart);
    drawButton(&buttons->settings);
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
    int panelX = (SCREEN_WIDTH  - SETTINGS_PANEL_WIDTH)  / 2;
    int panelY = (SCREEN_HEIGHT - SETTINGS_PANEL_HEIGHT) / 2;
    int btnX = panelX + (SETTINGS_PANEL_WIDTH - START_BTN_WIDTH) / 2;
    int btnY = panelY + SETTINGS_PANEL_HEIGHT - START_BTN_HEIGHT - 40;
    //initialize struct literals with values and return
    return createButton(btnX, btnY, START_BTN_WIDTH, START_BTN_HEIGHT, "Back");
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

//factory function for creating pause menu buttons
PauseButtons createPauseButtons(){
    int panelX = (SCREEN_WIDTH  - PAUSE_PANEL_WIDTH)  / 2;
    int panelY = (SCREEN_HEIGHT - PAUSE_PANEL_HEIGHT) / 2;

    int x = panelX + (PAUSE_PANEL_WIDTH - PAUSE_BTN_WIDTH) / 2;
    int rowHeight = PAUSE_BTN_HEIGHT + PAUSE_BTN_GAP;
    int firstBtnY = panelY + 220;

    PauseButtons buttons = {
        .resume   = createButton(x, firstBtnY + rowHeight * 0, PAUSE_BTN_WIDTH, PAUSE_BTN_HEIGHT, "Resume"),
        .restart  = createButton(x, firstBtnY + rowHeight * 1, PAUSE_BTN_WIDTH, PAUSE_BTN_HEIGHT, "Restart"),
        .settings = createButton(x, firstBtnY + rowHeight * 2, PAUSE_BTN_WIDTH, PAUSE_BTN_HEIGHT, "Settings"),
        .mainMenu = createButton(x, firstBtnY + rowHeight * 3, PAUSE_BTN_WIDTH, PAUSE_BTN_HEIGHT, "Main Menu"),
    };
    return buttons;
}

//factory function for default audio values
AudioSettings defaultAudioSettings(){
    AudioSettings s = { .mainVolume = 1.0f, .musicVolume = 0.8f };
    return s;
}

//factory function for loading all ui sound effects
UISounds loadUISounds(){
    UISounds sounds = {
        .click = LoadSound("assets/audio/click.wav"),
        .correct = LoadSound("assets/audio/correct.wav"),
        .wrong = LoadSound("assets/audio/wrong.wav"),
        .roundWin = LoadSound("assets/audio/round-win.wav"),
        .roundLoss = LoadSound("assets/audio/round-loss.wav"),
    };
    return sounds;
}

//factory function for loading bgm
BackgroundMusic loadBackgroundMusic(const char *fileName1, const char *fileName2, const char *fileName3){
    BackgroundMusic music = { .trackCount = 0, .currentTrack = 0 };

    const char *fileNames[MAX_MUSIC_TRACKS] = { fileName1, fileName2, fileName3 };

    for(int i=0; i<MAX_MUSIC_TRACKS; i++){
        if(fileNames[i] == NULL){
            continue;
        }
        music.tracks[music.trackCount] = LoadMusicStream(fileNames[i]);
        //turn off looping for manual switching between multiple bgm
        music.tracks[music.trackCount].looping = false;
        music.trackCount++;
    }

    return music;
}

//apply current volume to all audio
void applyAudioSettings(AudioSettings *settings, UISounds *sounds, BackgroundMusic *music){
    float sfxVol  = settings->mainVolume;
    float musicVol = settings->mainVolume * settings->musicVolume;

    //apply to every sfx slot
    SetSoundVolume(sounds->click, sfxVol);
    SetSoundVolume(sounds->correct, sfxVol);
    SetSoundVolume(sounds->wrong, sfxVol);
    SetSoundVolume(sounds->roundWin, sfxVol);
    SetSoundVolume(sounds->roundLoss, sfxVol);

    //apply to every bgm track
    for(int i=0; i<music->trackCount; i++){
        SetMusicVolume(music->tracks[i], musicVol);
    }
}

void playClickSound(UISounds *sounds){
    PlaySound(sounds->click);
}

void playCorrectSound(UISounds *sounds){
    PlaySound(sounds->correct);
}

void playWrongSound(UISounds *sounds){
    PlaySound(sounds->wrong);
}

void playRoundWinSound(UISounds *sounds){
    PlaySound(sounds->roundWin);
}

void playRoundLossSound(UISounds *sounds){
    PlaySound(sounds->roundLoss);
}

void startBackgroundMusic(BackgroundMusic *music){
    if(music->trackCount == 0){
        return;
    }
    PlayMusicStream(music->tracks[music->currentTrack]);
}

void updateBackgroundMusic(BackgroundMusic *music){
    if(music->trackCount == 0){
        return;
    }

    Music *current = &music->tracks[music->currentTrack];
    UpdateMusicStream(*current);

    if(!IsMusicStreamPlaying(*current)){
        // ensure state is clean
        StopMusicStream(*current);
        music->currentTrack = (music->currentTrack + 1) % music->trackCount;
        PlayMusicStream(music->tracks[music->currentTrack]);
    }
}

void unloadUISounds(UISounds *sounds){
    UnloadSound(sounds->click);
    UnloadSound(sounds->correct);
    UnloadSound(sounds->wrong);
    UnloadSound(sounds->roundWin);
    UnloadSound(sounds->roundLoss);
}

void unloadBackgroundMusic(BackgroundMusic *music){
    for(int i=0; i<music->trackCount; i++){
        UnloadMusicStream(music->tracks[i]);
    }
}