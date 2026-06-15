#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "game.h"

GameState state = { .round = 1, .score = 0 };

//load secret word from file
void loadSecretWord(char *word, int *wordSize){
	FILE *fp;
	fp=fopen("data/word-bank.txt","r");
	if(fp==NULL)
	{
		strcpy(state.message,"Error opening word-bank.txt file");
	}
	
	char fileWords[MAX_WORD_LENGTH];
	int wordCount=0;
	while(fscanf(fp,"%s",fileWords)!=EOF)
	{
		wordCount ++;
		
	}
	if(wordCount==0)
	{
		strcpy(state.message, "Word file is empty");
		fclose(fp);
	}
	int random=getRandomNumber(0,wordCount);
	rewind(fp);
	int currentline=0;
	while(fscanf(fp,"%s",fileWords)!=EOF)
	{
		if(currentline==random)
		{
			strcpy(word,fileWords);
			break;
		}
		currentline++;
	}
	*wordSize = strlen(word);
	fclose(fp);
}

//give 25% of the words letters as hint
void giveHint(char *word, char *displayedLetters){
	int hintCount = round(strlen(word) * 0.25);

	for(int i=0; i<hintCount; i++){
		int randomIndex;
		//loop if the random index is already given
		do{
			randomIndex = getRandomNumber(0, strlen(word));
		} while(displayedLetters[randomIndex] != '_');

		displayedLetters[randomIndex] = word[randomIndex];
		//check if the hinted letter exists in other places
		for(int j=0; j<state.secretWordSize; j++){
			if((displayedLetters[randomIndex] == word[j]) && randomIndex != j){
				displayedLetters[j] = word[j];
				hintCount++;
			}
		}
	}
	
	//set hint count as correctly guessed count
	state.correctCount = hintCount;
}

void checkWord(){
	//handle invalid input
	if(strlen(state.letter) == 0){
		strcpy(state.message, "Invalid input! Please enter letter or word only.");
		state.correctFlag = 2;
		return;
	}

	//handle multiple letters/string guesses
	if(strlen(state.letter) > 1){
		//handle whole word correctly guessed
		if(strcmp(state.letter, state.secretWord) == 0){
			strcpy(state.message, "Correct! You guessed the whole word!");
			state.correctCount = state.secretWordSize;
			strcpy(state.correctLetters, state.secretWord);
			state.correctFlag = 1;
		}
		//handle wrong whole word guesses
		else{
			strcpy(state.message, "Wrong word guess!");
			//check for any correct letters within the wrong whole word
			for(int i=0; i<strlen(state.letter); i++){
				int letterMatched = 0;
				for(int j=0; j<state.secretWordSize; j++){
					if(state.letter[i] == state.secretWord[j]){
						letterMatched = 1;
						//check if the letter isnt already guessed then set it as correct letters
						if(state.letter[i] != state.correctLetters[j]){
							state.correctLetters[j] = state.secretWord[j];
							state.correctCount++;
							state.correctFlag = 1;
						}
					}
				}
				//handle none correct letter
				if(!letterMatched){
					int alreadyWrong = 0;
					//skip if wrong letter is already guessed
					for(int j=0; j<state.wrongCount; j++){
						if(state.letter[i] == state.wrongLetters[j]){
							alreadyWrong = 1;
							break;
						}
					}
					//set the letter as wrong letter if not already guessed
					if(!alreadyWrong){
						if(state.wrongCount < MAX_WRONG_LENGTH){
							state.wrongLetters[state.wrongCount] = state.letter[i];
							state.wrongCount++;
						}
						if(state.wrongCount >= MAX_WRONGS){
							break;
						}
					}
				}
			}
			//message for some correct letters within wrong whole word guesses
			if(state.correctFlag == 1){
				strcat(state.message, " But some letters were correct!");
			}
		}
	}
	//handle single letter guesses
	else {
		//check if the letter is already-guessed wrong letter
		for(int i=0; i<state.wrongCount; i++){
			if(state.letter[0] == state.wrongLetters[i]){
				strcpy(state.message, "Already guessed! Try something else.");
				state.correctFlag = 2;
				break;
			}
		}
		if(state.correctFlag != 2){
			for(int i=0; i<state.secretWordSize; i++){
				//check if the letter matches the secret word and isnt already guessed
				if(state.letter[0] == state.secretWord[i] && state.letter[0] != state.correctLetters[i]){
					state.correctLetters[i] = state.secretWord[i];
					state.correctCount++;
					state.correctFlag = 1;
				}
				//handle already-guessed correct letter
				else if(state.letter[0] == state.correctLetters[i]){
					strcpy(state.message, "Already guessed! Try something else.");
					state.correctFlag = 2;
					break;
				}
			}
			//messages for correct and incorrect guessed
			if(state.correctFlag == 1){
				strcpy(state.message, "Correct guess!");
			}
			else if(state.correctFlag == 0){
				strcpy(state.message, "Incorrect guess!");
				
				//check if wrong count exceeds max wrong length
				if(state.wrongCount < MAX_WRONG_LENGTH){
					state.wrongLetters[state.wrongCount] = state.letter[0];
					state.wrongCount++;
				}
			}
		}
	}
	
	//final check of the word
	if(strcmp(state.correctLetters, state.secretWord) == 0){
		state.correctCount = state.secretWordSize;
		return;
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

//calculate total score of each round 
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

//initialize the game
void initGame(){
	srand(time(NULL));
	
	//reset all game variables
	resetVariables();

	//load secret word from file
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

//display round stats, input guesses then check those guesses
void updateGame(){
	printf("\nRound: %d", state.round);
	state.correctFlag = 0;
	printf("\nWord: ");
	for(int i=0; i<state.secretWordSize; i++){
		printf("%c ", state.correctLetters[i]);
	}
	printf("\nLives left: %d", MAX_WRONGS - state.wrongCount);
	printf("\nIncorrect letters: ");
	for(int i=0; i<state.wrongCount; i++){
		printf("%c ", state.wrongLetters[i]);
	}
	printf("\n");
	if(strlen(state.message) > 0){
		printf("\n%s", state.message);
	}
	printf("\nEnter a guess: ");
	scanf("%30s", state.letter);
	sanitizeInput(state.letter);
	flushInput();
	state.message[0] = '\0';
	checkWord();

	//return if invalid input
	if(strlen(state.letter) == 0){
		return;
	}
	state.tryCount++;
}

//check end condition
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

//capitalize all input and remove invalid characters
void sanitizeInput(char *letters) {
	int len = strlen(letters);
	int last = 0;

	for(int i=0; i<len; i++){
		char c = letters[i];

		if(c >= 'a' && c <= 'z'){
			letters[last++] = c - 32;
		} else if(c >= 'A' && c <= 'Z'){
			letters[last++] = c;
		}
	}
	letters[last] = '\0';
}

void resetVariables(){
	state.secretWordSize = 0;
	state.tryCount = 0;
	state.wrongCount = 0;
	state.correctCount = 0;
	state.correctFlag = 0;
	state.highScore = getHighScore();
	state.message[0] = '\0';

	for(int i=0; i<MAX_WORD_LENGTH; i++){
		state.secretWord[i] = '\0';
		state.correctLetters[i] = '\0';
		state.letter[i] = '\0';
	}
	for(int i=0; i<MAX_WRONG_LENGTH; i++){
		state.wrongLetters[i] = '\0';
	}
	
}

void flushInput(){
	int c;
	while((c = getchar()) != '\n' && c != EOF);
}

int getRandomNumber(int MIN, int MAX){
	return (rand() % MAX) + MIN;
}

void clearScreen(){
	system("cls");
}