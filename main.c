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
        printf("\n\nPlay again? (y/n): ");
		scanf(" %c", &replay);
		flushInput();
    } while(replay == 'y' || replay == 'Y');

    printf("\nThanks for playing!\n");
    return 0;
}