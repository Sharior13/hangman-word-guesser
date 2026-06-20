#include <string.h>
#include "game.h"
#include "drawing.h"
#include "input.h"
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

	//initialize buttons in start scren
	StartScreenButtons startButtons = createStartScreenButtons();
	Button backBtn = createBackButton();

	//initialize buttons in gameover screen
	GameOverButtons gameOverButtons = createGameOverButtons();

	//initialize text inputs
	TextInput guessInput = createTextInput();

	//prepare first round
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
				clearInput(&guessInput);
				state.message[0] = '\0';
			}
			//user clicks settings button
			if(isButtonClicked(&startButtons.settings, mousePos, mouseClicked)){
				currentScreen = SCREEN_SETTINGS;
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
			updateInput(&guessInput);

			//submit guess on user pressing enter
            if(IsKeyPressed(KEY_ENTER) && guessInput.length > 0){
                strncpy(state.letter, guessInput.buffer, INPUT_MAX);
                state.letter[INPUT_MAX] = '\0';
                sanitizeInput(state.letter);
 
                state.correctFlag = 0;
                state.message[0]  = '\0';
 
                if(strlen(state.letter) > 0){
                    checkWord();
                    state.tryCount++;
                }
                clearInput(&guessInput);
 
                //check end condition after the guess
                if(gameShouldEnd()){
                    state.score += calculateScore();
                    if(state.score > state.highScore){
                        setHighScore(state.score);
                        state.highScore = state.score;
                    }
                    state.gameWon = (state.correctCount == state.secretWordSize);
                    currentScreen = SCREEN_GAMEOVER;
                }
            }
		}
		else if(currentScreen == SCREEN_GAMEOVER){
			//handle replay/exit logic 
    		int tryAgainTriggered = isButtonClicked(&gameOverButtons.tryAgain, mousePos, mouseClicked);
			int mainMenuTriggered = isButtonClicked(&gameOverButtons.mainMenu, mousePos, mouseClicked);
 
			//check is user clicked try again
            if(tryAgainTriggered){
                if (state.gameWon){
                    state.round++;
                }
				else{
                    state.round = 1;
                    state.score = 0;
                }
                initGame();
                clearInput(&guessInput);
                currentScreen = SCREEN_PLAYING;
            }
			//check is user clicked main menu
            else if(mainMenuTriggered){
                currentScreen = SCREEN_START;
            }
		}

		//start drawing
		BeginDrawing();
		ClearBackground(COLOR_BG);

		if(currentScreen == SCREEN_START){
			//draw start screen
			drawStartScreen(state.highScore, state.round, &startButtons);
		}
		else if(currentScreen == SCREEN_SETTINGS){
			//draw settings screen
			drawSettingsScreen(&backBtn);
		}
		else if(currentScreen == SCREEN_PLAYING){
			//draw main game (hangman, guesses, lives etc)
			drawPlayingScreen(&state, guessInput.buffer);
		}
		else if(currentScreen == SCREEN_GAMEOVER){
			//draw last game state behind the overlay
			drawPlayingScreen(&state, guessInput.buffer);

			//draw game over screen overlay
			drawGameOver(state.gameWon, state.secretWord, state.score, &gameOverButtons);
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}