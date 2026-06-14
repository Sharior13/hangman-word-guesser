#include <stdio.h>
#include "game.h"
#include "drawing.h"

int main(){
    char replay;
    do {
        initGame();
        while(!gameShouldEnd()){
            updateGame();
            drawHangman(state.wrongCount);
        }
        
        state.score += calculateScore();
        if(state.score > state.highScore){
			setHighScore(state.score);
		}
		
		if(state.correctCount == state.secretWordSize){
        	state.round++;
        	printf("\nWord guessed successfully!");
        	printf("\nTotal Score: %d", state.score);
    	}
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

    printf("\nThanks for playing!\n");
    return 0;
}