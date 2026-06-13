#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "game.h"

GameState state = { .round = 1, .score = 0};


void loadSecretWord(char *word, int *wordSize){
	strcpy(word, "football"); //edit to take word from file
	*wordSize = strlen(word);
}

void checkWord(){
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
		for(int w = 0; w < state.wrongCount; w++){
        	if(state.letter[0] == state.wrongLetters[w]){
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
	state.secretWordSize=0;
	state.tryCount=0;
	state.wrongCount=0;
	state.correctCount=0;
	state.correctFlag=0;
	state.highScore = getHighScore();
	
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
    printf("\nHighscore: %d", state.highScore);
    printf("\nRound: %d", state.round);
    printf("\nPress any key to start: ");
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
    scanf("%30s", state.letter);
    flushInput();
    checkWord();
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

void flushInput(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}