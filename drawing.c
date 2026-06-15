#include <stdio.h>
#include <string.h>
#include "drawing.h"

void drawHangman(int wrongCount){
	int parts = wrongCount;
	const char* hangmanParts[] = {
	"     _________",
	"    |         |",
	"    |         O",
	"    |        /|\\",
	"    |        / \\",
	"    |" 
	};
	
	//check if the wrong count exceeds the no. of body parts
	if(parts > 6){
		parts = 6;
	}

	//display hangman
    printf("\n");
    for(int i=0; i<=parts-1; i++){
        printf("%s\n", hangmanParts[i]);
    }
}