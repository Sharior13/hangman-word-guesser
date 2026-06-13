#include <stdio.h>
#include "drawing.h"

void drawHangman(int wrongCount){
	const char* hangmanParts[] = {
	"     _________",
	"    |         |",
	"    |         O",
	"    |        /|\\",
	"    |        / \\",
	"    |" 
	};

    printf("\n");
    for (int i = 0; i<wrongCount; i++) {
        printf("%s\n", hangmanParts[i]);
    }
}