#include "game.h"
#include "drawing.h"
#include "raylib.h"

//game screen states
typedef enum{
	SCREEN_START,
	SCREEN_SETTINGS,
	SCREEN_PLAYING,
	SCREEN_GAMEOVER
} Screen;


int main(){
	//make the game full screen
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	
	//initialize game window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hangman Word Guesser");
	SetTargetFPS(60);

	//initialize screen state
	Screen currentScreen = SCREEN_START;
	int gameWon = 0;

	//initialize buttons in start scren
	StartScreenButtons startButtons = createStartScreenButtons();
	Button backBtn = createBackButton();


	initGame();

	//loop until user closes the window
	while(!WindowShouldClose()){
		//check mouse position and clicks
		Vector2 mousePos = GetMousePosition();
        int mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

		if(currentScreen == SCREEN_START){
			//user clicks play button
			if(isButtonClicked(&startButtons.play, mousePos, mouseClicked)){
				currentScreen = SCREEN_PLAYING;
				state.message[0] = '\0';
				//write code to clear ui input buffer
			}
			//user clicks settings button
			if(isButtonClicked(&startButtons.settings, mousePos, mouseClicked)){
				currentScreen = SCREEN_SETTINGS;
				//write code for settings
			}
			//user clicks quit button
			if(isButtonClicked(&startButtons.quit, mousePos, mouseClicked)){
				break;
			}
		}
		else if(currentScreen == SCREEN_SETTINGS){
			//write settings logic

			//exiting settings menu logic
			if(isButtonClicked(&backBtn, mousePos, mouseClicked) || IsKeyPressed(KEY_ESCAPE)){
				currentScreen = SCREEN_START;
			}
		}
		else if(currentScreen == SCREEN_PLAYING){
			//write code to enter guesses
		}
		else if(currentScreen == SCREEN_GAMEOVER){
			//handle replay/exit logic 
		}

		//start drawing
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if(currentScreen == SCREEN_START){
			//draw start screen
			drawStartScreen(state.highScore, state.round, &startButtons);
		}
		else if(currentScreen == SCREEN_SETTINGS){
			//draw settings screen
			const char *title = "SETTINGS";
            int textSize = 90;
            int textWidth = MeasureText(title, textSize);
            DrawText(title, (SCREEN_WIDTH - textWidth) / 2, 260, textSize, COLOR_HIGHLIGHT);
 
            const char *note = "Settings options coming soon.";
            int noteWidth = MeasureText(note, 32);
            DrawText(note, (SCREEN_WIDTH - noteWidth) / 2, 420, 32, COLOR_DIM);
 
            drawButton(&backBtn);
		}
		else if(currentScreen == SCREEN_PLAYING){
			//draw main game (hangman, guesses, lives etc)
		}
		else if(currentScreen == SCREEN_GAMEOVER){
			//draw game over screen
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}