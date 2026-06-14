#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "game.h"

GameState state = { .round = 1, .score = 0 };


void loadSecretWord(char *word, int *wordSize){
	strcpy(word, "COMPUTER"); //edit to take word from file
	*wordSize = strlen(word);
}

void giveHint(char *word, char *displayedLetters){
	int hintCount = round(strlen(word) * 0.25);
	for(int i=0; i<hintCount; i++){
		int randomIndex = getRandomNumber(0, strlen(word));
		displayedLetters[randomIndex] = word[randomIndex];
	}
}

void checkWord(){
	if(strlen(state.letter) == 0){
        printf("\nInvalid input! Please enter letter or word only.");
        state.correctFlag = 2;
        return;
    }
    
	if(strlen(state.letter) > 1){
        if(strcmp(state.letter, state.secretWord) == 0){
            printf("\nCorrect! You guessed the whole word!");
            state.correctCount = state.secretWordSize;
        }
		else{
        	printf("\nWrong word guess!");
        	for(int i=0; i<strlen(state.letter); i++){
	    		int letterMatched = 0;
    			for(int j=0; j<state.secretWordSize; j++){
        			if(state.letter[i] == state.secretWord[j]){
            			letterMatched = 1;
            			if(state.letter[i] != state.correctLetters[j]){
            	    		state.correctLetters[j] = state.secretWord[j];
        	        		state.correctCount++;
    	            		state.correctFlag = 1;
	            		}
        			}
    			}
    			if(!letterMatched){
        			int alreadyWrong = 0;
        			for(int j=0; j<state.wrongCount; j++){
            			if(state.letter[i] == state.wrongLetters[j]){
                			alreadyWrong = 1;
            	    		break;
        	    		}
    	    		}
	        		if(!alreadyWrong){
            			state.wrongLetters[state.wrongCount] = state.letter[i];
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
		for(int i=0; i<state.wrongCount; i++){
        	if(state.letter[0] == state.wrongLetters[i]){
            	printf("\nAlready guessed! Try something else.");
            	state.correctFlag = 2;
            	break;
        	}
    	}
    	if(state.correctFlag != 2){
    		for(int i=0; i<state.secretWordSize; i++){
            	if(state.letter[0] == state.secretWord[i] && state.letter[0] != state.correctLetters[i]){
        	        state.correctLetters[i] = state.secretWord[i];
    	            state.correctCount++;
	                state.correctFlag = 1;
            	}
            	else if(state.letter[0] == state.correctLetters[i]){
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
    }
}

int getHighScore(){
	//add file handling and remove ts after adding file handling
	return state.highScore;
}

void setHighScore(int currentScore){
	//add file handling
	state.highScore = currentScore;
}

int calculateScore(){
	int baseScore = state.correctCount * BASE_SCORE_PER_LETTER;
	int penalty   = state.wrongCount * WRONG_GUESS_PENALTY;
	int tryBonus  = (state.tryCount > 0) ? (TRY_BONUS / state.tryCount) : TRY_BONUS;

	int finalScore = baseScore - penalty + tryBonus;
	
	if(finalScore < 0){
		finalScore = 0;
	}
		
	return finalScore;
}

void initGame(){
	srand(time(NULL));
	
	state.secretWordSize = 0;
	state.tryCount = 0;
	state.wrongCount = 0;
	state.correctCount = 0;
	state.correctFlag = 0;
	state.highScore = getHighScore();
	
	for(int i=0; i<MAX_WORD_LENGTH; i++){
        state.secretWord[i] = '\0';
        state.correctLetters[i] = '\0';
        state.letter[i] = '\0';
    }
    for(int i=0; i<MAX_WRONG_LENGTH; i++){
        state.wrongLetters[i] = '\0';
    }
    
	loadSecretWord(state.secretWord, &state.secretWordSize);
	for(int i=0; i<state.secretWordSize; i++){
        state.correctLetters[i] = '_';
    }
    giveHint(state.secretWord, state.correctLetters);
    printf("\n====================== Hangman Word Guesser ======================");
    printf("\nHighscore: %d", state.highScore);
    printf("\nRound: %d", state.round);
    printf("\nPress any key to start: ");
    getch();
    printf("\n");
}

void updateGame(){
    state.correctFlag = 0;
    printf("\nWord: ");
    for(int i=0; i<state.secretWordSize; i++){
        printf("%c ", state.correctLetters[i]);
    }
    printf("\nLives left: %d", MAX_WRONGS - state.wrongCount);
    printf("\nIncorrect letters: ");
    for(int i=0; i<MAX_WRONG_LENGTH; i++){
    	printf("%c ", state.wrongLetters[i]);
	}
    printf("\n\nEnter a guess: ");
    scanf("%30s", state.letter);
    sanitizeInput(state.letter);
    flushInput();
    checkWord();
    
    if(strlen(state.letter) == 0){
        return;
    }
    state.tryCount++;
}

int gameShouldEnd(){
    if(state.correctCount == state.secretWordSize){
        return 1;
    }
    else if(state.wrongCount >= MAX_WRONGS){
        return 1;
    }
    else{
    	return 0;
	}
}

void sanitizeInput(char *letters) {
    int len = strlen(letters);
    int last = 0;

    for (int i = 0; i < len; i++) {
        char c = letters[i];

        if (c >= 'a' && c <= 'z') {
            letters[last++] = c - 32;
        } else if (c >= 'A' && c <= 'Z') {
            letters[last++] = c;
        }
    }
    letters[last] = '\0';
}

void flushInput(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int getRandomNumber(int MIN, int MAX){
	return (rand() % MAX) + MIN;
}