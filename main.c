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
	SCREEN_PAUSED,
	SCREEN_GAMEOVER
} Screen;

int main(){
	//make the game windowed full screen
	SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MAXIMIZED);
	
	//initialize game window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hangman Word Guesser");
	SetTargetFPS(60);

	//audio setup
	InitAudioDevice();
	UISounds uiSounds = loadUISounds();
	//edit to take volume from file if it exists
	BackgroundMusic bgMusic = loadBackgroundMusic("assets/audio/background-music-1.ogg", "assets/audio/background-music-2.ogg", "assets/audio/background-music-3.ogg", 0.3f);
	startBackgroundMusic(&bgMusic);

	//dont close program on pressing escape button
	SetExitKey(KEY_NULL);

	//initialize screen state
	Screen currentScreen = SCREEN_START;
	//remember which screen to return to after leaving settings
	Screen previousScreen = SCREEN_START;

	//initialize buttons in start scren
	StartScreenButtons startButtons = createStartScreenButtons();
	Button backBtn = createBackButton();

	//initialize buttons in gameover screen
	GameOverButtons gameOverButtons = createGameOverButtons();

	//initialize buttons in pause menu
	PauseButtons pauseButtons = createPauseButtons();

	//initialize text inputs
	TextInput guessInput = createTextInput();

	//load heart icon texture
	Texture2D heartTexture = LoadTexture("assets/sprite/heart.png");

	//prepare first round
	initGame();

	//track the pause between the game ending and the game over panel appearing
	float gameOverRevealTimer = 0.0f;

	//loop until user closes the window
	while(!WindowShouldClose()){

		//keep the background music stream buffered
		updateBackgroundMusic(&bgMusic);

		//check mouse position and clicks
		Vector2 mousePos = GetMousePosition();
        int mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

		if(currentScreen == SCREEN_START){
			//user clicks play button
			if(isButtonClicked(&startButtons.play, mousePos, mouseClicked)){
				playClickSound(&uiSounds);
				currentScreen = SCREEN_PLAYING;
				clearInput(&guessInput);
				state.message[0] = '\0';
				gameOverRevealTimer = 0.0f;
			}
			//user clicks settings button
			if(isButtonClicked(&startButtons.settings, mousePos, mouseClicked)){
				playClickSound(&uiSounds);
				previousScreen = SCREEN_START;
				currentScreen = SCREEN_SETTINGS;
			}
			//user clicks quit button
			if(isButtonClicked(&startButtons.quit, mousePos, mouseClicked)){
				break;
			}
		}
		else if(currentScreen == SCREEN_SETTINGS){
			//write settings logic

			//exiting settings menu logic — return to wherever Settings was opened from
			if(isButtonClicked(&backBtn, mousePos, mouseClicked) || IsKeyPressed(KEY_ESCAPE)){
				playClickSound(&uiSounds);
				currentScreen = previousScreen;
			}
		}
		else if(currentScreen == SCREEN_PLAYING){
			//pause the game
			if(gameOverRevealTimer <= 0.0f && IsKeyPressed(KEY_ESCAPE)){
				playClickSound(&uiSounds);
				currentScreen = SCREEN_PAUSED;
			}

			//freeze input while the final hangman pose is being revealed and skip entirely if the player just paused this frame
			if(currentScreen == SCREEN_PLAYING && gameOverRevealTimer <= 0.0f){
				updateInput(&guessInput);

				//submit guess on user pressing enter
				if(IsKeyPressed(KEY_ENTER) && guessInput.length > 0){
					strncpy(state.letter, guessInput.buffer, INPUT_MAX);
					state.letter[INPUT_MAX - 1] = '\0';
					sanitizeInput(state.letter);

					state.correctFlag = 0;
					state.message[0]  = '\0';

					if(strlen(state.letter) > 0){
						checkWord();
						state.tryCount++;

						//feedback sound based on guess result
						if(state.correctFlag == 1){
							playCorrectSound(&uiSounds);
						}
						else if(state.correctFlag == 2){
							playWrongSound(&uiSounds);
						}
						else if(state.correctFlag == 0){
							playWrongSound(&uiSounds);
						}
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

						//play sound on win/loss
						if(state.gameWon){
							playRoundWinSound(&uiSounds);
						}
						else{
							playRoundLossSound(&uiSounds);
						}

						//delay showing game over screen to show game state to user
						gameOverRevealTimer = GAMEOVER_REVEAL_DELAY;
					}
				}
			}
			else if(currentScreen == SCREEN_PLAYING){
				//show the game over panel once the reveal delay has passed
				gameOverRevealTimer -= GetFrameTime();
				if(gameOverRevealTimer <= 0.0f){
					gameOverRevealTimer = 0.0f;
					currentScreen = SCREEN_GAMEOVER;
				}
			}
		}
		else if(currentScreen == SCREEN_PAUSED){
			//go back to the game or via escape as shortcut
			if(isButtonClicked(&pauseButtons.resume, mousePos, mouseClicked) || IsKeyPressed(KEY_ESCAPE)){
				playClickSound(&uiSounds);
				currentScreen = SCREEN_PLAYING;
			}
			//open settings remembering to return to the pause menu afterward
			else if(isButtonClicked(&pauseButtons.settings, mousePos, mouseClicked)){
				playClickSound(&uiSounds);
				previousScreen = SCREEN_PAUSED;
				currentScreen = SCREEN_SETTINGS;
			}
			//quit to main menu
			else if(isButtonClicked(&pauseButtons.mainMenu, mousePos, mouseClicked)){
				playClickSound(&uiSounds);
				initGame();
				clearInput(&guessInput);
				state.round = 1;
				state.score = 0;
				state.message[0] = '\0';
				gameOverRevealTimer = 0.0f;
				currentScreen = SCREEN_START;
			}
		}
		else if(currentScreen == SCREEN_GAMEOVER){
			//handle replay/exit logic 
    		int retryTriggered = isButtonClicked(state.gameWon ? &gameOverButtons.playAgain: &gameOverButtons.tryAgain, mousePos, mouseClicked);
			int mainMenuTriggered = isButtonClicked(&gameOverButtons.mainMenu, mousePos, mouseClicked);
 
			//check is user clicked try again
            if(retryTriggered){
				playClickSound(&uiSounds);
                if(state.gameWon){
                    state.round++;
                }
				else{
                    state.round = 1;
                    state.score = 0;
                }
                initGame();
                clearInput(&guessInput);
                gameOverRevealTimer = 0.0f;
                currentScreen = SCREEN_PLAYING;
            }
			//check is user clicked main menu
            else if(mainMenuTriggered){
				playClickSound(&uiSounds);
				initGame();
                clearInput(&guessInput);
				state.round = 1;
                state.score = 0;
				state.message[0] = '\0';
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
			drawPlayingScreen(heartTexture, &state, guessInput.buffer);
		}
		else if(currentScreen == SCREEN_PAUSED){
			//draw the frozen game state behind the pause overlay
			drawPlayingScreen(heartTexture, &state, guessInput.buffer);

			//draw pause menu overlay
			drawPauseMenu(&pauseButtons);
		}
		else if(currentScreen == SCREEN_GAMEOVER){
			//draw last game state behind the overlay
			drawPlayingScreen(heartTexture, &state, guessInput.buffer);

			//draw game over screen overlay
			drawGameOver(state.gameWon, state.secretWord, state.score, &gameOverButtons);
		}
		EndDrawing();
	}

	//free GPU resources before closing
	UnloadTexture(heartTexture);
	//free audio resources and close the audio device
	unloadBackgroundMusic(&bgMusic);
	unloadUISounds(&uiSounds);
	CloseAudioDevice();

	CloseWindow();
	return 0;
}