#include <stdio.h>
#include "game.h"
#include "drawing.h"
#include "raylib.h"

typedef enum{
	SCREEN_START,
	SCREEN_PLAYING,
	SCREEN_GAMEOVER
} Screen;

int main(){
	//make the game full screen
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	//initialize game window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hangman Word Guesser");
	SetTargetFPS(60);

	Screen currentScreen = SCREEN_START;
	int gameWon = 0;

	initGame();

	//loop until user closes the window
	while(!WindowShouldClose()){
		if(currentScreen == SCREEN_START){
			if(IsKeyPressed(KEY_ENTER)){
				currentScreen = SCREEN_PLAYING;
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
		DrawText("Congrats! You created your first window!", SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2, 20, BLACK);
		if(currentScreen == SCREEN_START){
			//draw start screen
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

    // char replay;
    // //loop until users says no
    // do{
    // 	clearScreen();
    //     initGame();
    //     //loop until end condition is met
    //     while(!gameShouldEnd()){
    // 		clearScreen();
    //         drawHangman(state.wrongCount);
    //         updateGame();
    //     }
        
    //     //calculate score and set highscores
    //     state.score += calculateScore();
    //     if(state.score > state.highScore){
	// 		setHighScore(state.score);
	// 	}
		
	// 	clearScreen(); 
		
	// 	//win condition
	// 	if(state.correctCount == state.secretWordSize){
    //     	state.round++;
    //     	printf("\nWord guessed successfully!");
    //     	printf("\nTotal Score: %d", state.score);
    // 	}
    // 	//lose condition
    // 	else if(state.wrongCount >= MAX_WRONGS){
    //     	printf("\nOut of lives! The correct word was %s.", state.secretWord);
	//         printf("\nTotal Score: %d", state.score);
    //     	state.round = 1;
    //     	state.score = 0;
    // 	}
    //     printf("\n\nPlay again? (y/n): ");
	// 	scanf(" %c", &replay);
	// 	flushInput();
    // } while(replay == 'y' || replay == 'Y');

    // printf("\nThanks for playing!\nYour highest score was %d.", state.highScore);
    // return 0;
}