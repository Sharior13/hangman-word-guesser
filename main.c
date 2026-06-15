#include <stdio.h>
#include "game.h"
#include "drawing.h"

int main(){
    char replay;
    //loop until users says no
    do{
    	clearScreen();
        initGame();
        //loop until end condition is met
        while(!gameShouldEnd()){
    		clearScreen();
            drawHangman(state.wrongCount);
            updateGame();
        }
        
        //calculate score and set highscores
        state.score += calculateScore();
        if(state.score > state.highScore){
			setHighScore(state.score);
		}
		
		clearScreen(); 
		
		//win condition
		if(state.correctCount == state.secretWordSize){
        	state.round++;
        	printf("\nWord guessed successfully!");
        	printf("\nTotal Score: %d", state.score);
    	}
    	//lose condition
    	else if(state.wrongCount >= MAX_WRONGS){
        	printf("\nOut of lives! The correct word was %s.", state.secretWord);
	        printf("\nTotal Score: %d", state.score);
        	state.round = 1;
        	state.score = 0;
    	}
        printf("\n\nPlay again? (y/n): ");
		scanf(" %c", &replay);
		flushInput();
    } while(replay == 'y' || replay == 'Y');

    printf("\nThanks for playing!\nYour highest score was %d.", state.highScore);
    return 0;
}