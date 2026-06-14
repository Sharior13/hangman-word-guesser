#ifndef GAME_H
#define GAME_H

#define MAX_WRONGS 6
#define MAX_WORD_LENGTH 30
#define MAX_WRONG_LENGTH 26
#define BASE_SCORE_PER_LETTER 100
#define WRONG_GUESS_PENALTY 30
#define TRY_BONUS 200

typedef struct{
	char secretWord[MAX_WORD_LENGTH];
	int secretWordSize, tryCount, wrongCount, correctCount, correctFlag, score, highScore, round;
	char letter[MAX_WORD_LENGTH], correctLetters[MAX_WORD_LENGTH], wrongLetters[MAX_WRONG_LENGTH];	
} GameState;

extern GameState state;

void initGame();
void updateGame();
void checkWord();
void loadSecretWord(char *word, int *wordSize);
void giveHint(char *word, char *displayedLetters);
int calculateScore();
int getHighScore();  
void setHighScore(int currentScore);  
int gameShouldEnd();
void sanitizeInput(char *letters);
void flushInput();
int getRandomNumber(int MIN, int MAX);

#endif