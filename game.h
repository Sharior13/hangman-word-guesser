#ifndef GAME_H
#define GAME_H

//game constants
#define MAX_WRONGS 6
#define MAX_WORD_LENGTH 30
#define MAX_WRONG_LENGTH 26
#define MAX_MESSAGE_LENGTH 200
#define BASE_SCORE_PER_LETTER 100
#define WRONG_GUESS_PENALTY 30
#define TRY_BONUS 200

//how long to show the final hangman pose before the game over panel appears
#define GAMEOVER_REVEAL_DELAY 1.0f

//game variables blueprint
typedef struct{
	char secretWord[MAX_WORD_LENGTH];
	int secretWordSize, tryCount, wrongCount, correctCount, correctFlag;
	int score, highScore, round, gameWon;
	char letter[MAX_WORD_LENGTH], correctLetters[MAX_WORD_LENGTH], wrongLetters[MAX_WRONG_LENGTH], message[MAX_MESSAGE_LENGTH];	
} GameState;

extern GameState state;

void initGame();
void checkWord();
void loadSecretWord(char *word, int *wordSize);
void giveHint(char *word, char *displayedLetters);
int calculateScore();
int getHighScore();  
void setHighScore(int currentScore);  
int gameShouldEnd();
void sanitizeInput(char *letters);
void resetVariables();
int getRandomNumber(int MIN, int MAX);

#endif