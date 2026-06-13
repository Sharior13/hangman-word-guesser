#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "game.h"

GameState state;

void loadSecretWord(char *word, int *wordSize){
	strcpy(word, "football"); //edit to take word from file
	*wordSize = strlen(word);
}

void initGame(){
	state.secretWordSize=0, state.tryCount=0, state.wrongCount=0, state.correctCount=0, state.correctFlag=0;
	
	for(int i=0; i<MAX_WORD_LENGTH; i++){
        state.secretWord[i] = '\0';
        state.correctLetters[i] = '\0';
        state.letter[i] = '\0';
    }
    for(int i=0; i<26; i++){
        state.wrongLetters[i] = '\0';
    }
    
	loadSecretWord(state.secretWord, &state.secretWordSize);
	for(int i=0; i<state.secretWordSize; i++){
        state.correctLetters[i] = '_';
    }
    printf("\n====================== Hangman Word Guesser ======================");
    printf("\nPress enter to play:");
    getch();
}

void updateGame(){
    state.correctFlag = 0;
    printf("\nWord: ");
    for(int i=0; i<state.secretWordSize; i++){
        printf("%c ", state.correctLetters[i]);
    }
    printf("\nLives left: %d", MAX_WRONGS - state.wrongCount);
    printf("\n\nEnter a guess: ");
    scanf("%s", state.letter);
    flushInput();

    if(strlen(state.letter) > 1){
        if(strcmp(state.letter, state.secretWord) == 0){
            printf("\nCorrect! You guessed the whole word!");
            state.correctCount = state.secretWordSize;
        }
		else{
        	printf("\nWrong word guess!");
        	for(int g=0; g<strlen(state.letter); g++){
	    		int letterMatched = 0;
    			for(int i=0; i<state.secretWordSize; i++){
        			if(state.letter[g] == state.secretWord[i]){
            			letterMatched = 1;
            			if(state.letter[g] != state.correctLetters[i]){
            	    		state.correctLetters[i] = state.secretWord[i];
        	        		state.correctCount++;
    	            		state.correctFlag = 1;
	            		}
        			}
    			}
    			if(!letterMatched){
        			int alreadyWrong = 0;
        			for(int w=0; w<state.wrongCount; w++){
            			if(state.letter[g] == state.wrongLetters[w]){
                			alreadyWrong = 1;
            	    		break;
        	    		}
    	    		}
	        		if(!alreadyWrong){
            			state.wrongLetters[state.wrongCount] = state.letter[g];
            			state.wrongCount++;
        			}
    			}
			}
        	if(state.correctFlag == 1){
            	printf(" But some letters were correct!");
        	}
    	}
    }
	else {
        for(int i=0; i<state.secretWordSize; i++){
            if(state.letter[0] == state.secretWord[i] && state.letter[0] != state.correctLetters[i]){
                state.correctLetters[i] = state.secretWord[i];
                state.correctCount++;
                state.correctFlag = 1;
            }
            else if(state.letter[0] == state.correctLetters[i] || state.letter[0] == state.wrongLetters[i]){
                printf("\nAlready guessed! Try something else.");
                state.correctFlag = 2;
                break;
            }
        }
        if(state.correctFlag == 1){
            printf("\nCorrect guess!");
        }
        else if(state.correctFlag == 0){
            printf("\nIncorrect!");
            state.wrongLetters[state.wrongCount] = state.letter[0];
            state.wrongCount++;
        }
    }
    state.tryCount++;
}

int gameShouldEnd(){
    if(state.correctCount == state.secretWordSize){
        printf("\nWord guessed successfully!");
        return 1;
    }
    else if(state.wrongCount >= MAX_WRONGS){
        printf("\nOut of lives! The correct word was %s.", state.secretWord);
        return 1;
    }
    else{
    	return 0;
	}
}

void flushInput(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}