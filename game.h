#ifndef GAME_H
#define GAME_H

#define MAX_WRONGS 5
#define MAX_WORD_LENGTH 30

typedef struct{
	char secretWord[MAX_WORD_LENGTH];
	int secretWordSize, tryCount, wrongCount, correctCount, correctFlag;
	char letter[MAX_WORD_LENGTH], correctLetters[MAX_WORD_LENGTH], wrongLetters[26];	
} GameState;

extern GameState state;

void initGame();
void updateGame();
void loadSecretWord(char *word, int *wordSize);
int gameShouldEnd();
void flushInput();

#endif