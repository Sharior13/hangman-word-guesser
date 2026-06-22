#ifndef DRAWING_H
#define DRAWING_H

#include "raylib.h"
#include "game.h"

//define application screen size
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

//colors used in game
#define COLOR_BG (Color){ 15,  15,  20,  255 }
#define COLOR_GALLOWS (Color){ 180, 180, 200, 255 }
#define COLOR_BODY (Color){ 230, 100, 80,  255 }
#define COLOR_TEXT (Color){ 220, 220, 235, 255 }
#define COLOR_DIM (Color){ 120, 120, 140, 255 }
#define COLOR_CORRECT (Color){ 80,  200, 120, 255 }
#define COLOR_WRONG (Color){ 220, 80,  80,  255 }
#define COLOR_HIGHLIGHT (Color){ 255, 200, 60,  255 }
#define COLOR_PANEL (Color){ 25,  25,  35,  255 }
#define COLOR_BORDER (Color){ 50,  50,  70,  255 }

//start screen button layout
#define START_BTN_WIDTH 360
#define START_BTN_HEIGHT 80
#define START_BTN_GAP 30
#define START_BTN_TOP_Y 650

//game over screen layout
#define GAMEOVER_PANEL_WIDTH 900
#define GAMEOVER_PANEL_HEIGHT 520
#define GAMEOVER_BTN_WIDTH 360
#define GAMEOVER_BTN_HEIGHT 80
#define GAMEOVER_BTN_GAP 40

//pause menu layout
#define PAUSE_BTN_WIDTH 360
#define PAUSE_BTN_HEIGHT 80
#define PAUSE_BTN_GAP 30
#define PAUSE_PANEL_WIDTH 600
#define PAUSE_PANEL_HEIGHT 560
#define PAUSE_TITLE_SIZE 70

//maximum number of bgm that can be loaded 
#define MAX_MUSIC_TRACKS 3

//button blueprint
typedef struct{
    Rectangle bounds;
    const char *label;
    int isHovered;
} Button;

//factory function to make one individual button
Button createButton(int x, int y, int width, int height, const char *label);

//start screen buttons
typedef struct{
    Button play;
    Button settings;
    Button quit;
} StartScreenButtons;

//game over screen buttons
typedef struct{
    Button tryAgain;
    Button playAgain;
    Button mainMenu;
} GameOverButtons;

//pause menu buttons
typedef struct{
    Button resume;
    Button settings;
    Button mainMenu;
} PauseButtons;

//ui sound effects
typedef struct{
    Sound click;
    Sound correct;
    Sound wrong;
} UISounds;

//background music
typedef struct{
    Music tracks[MAX_MUSIC_TRACKS];
    int trackCount;
    int currentTrack;
    float volume;
} BackgroundMusic;

//factory function for making start screen buttons
StartScreenButtons createStartScreenButtons();
//factory function for back button
Button createBackButton();
//factory function for making game over buttons
GameOverButtons createGameOverButtons();
//factory function for making pause menu buttons
PauseButtons createPauseButtons();
//factory function for loading all ui sound effects
UISounds loadUISounds();
//factory function for loading background music tracks
BackgroundMusic loadBackgroundMusic(const char *fileName1, const char *fileName2, const char *fileName3, float volume);

//play a ui click sound effect
void playClickSound(UISounds *sounds);
//play the correct guess sound effect
void playCorrectSound(UISounds *sounds);
//play the wrong guess sound effect
void playWrongSound(UISounds *sounds);

//start background music playing the first loaded track
void startBackgroundMusic(BackgroundMusic *music);
//keep the current track buffered
void updateBackgroundMusic(BackgroundMusic *music);

//unload all ui sound effects
void unloadUISounds(UISounds *sounds);
//unload all bgm
void unloadBackgroundMusic(BackgroundMusic *music);

//drawing functions
void drawHeart(Texture2D heartTexture, int x, int y, int size, Color tint);
void drawStartScreen(int highScore, int round, StartScreenButtons *buttons);
void drawSettingsScreen(Button *backBtn);
void drawPlayingScreen(Texture2D heartTexture, GameState *state, char *currentInput);
void drawGameOver(int won, const char *secretWord, int score, GameOverButtons *buttons);
void drawPauseMenu(PauseButtons *buttons);
void drawButton(Button *btn);
int isButtonClicked(Button *btn, Vector2 mousePos, int mousePressed);

#endif