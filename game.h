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

//path to game save file
#define GAME_FILE_PATH "data/game-file.bin"

//game variables blueprint
typedef struct{
	char secretWord[MAX_WORD_LENGTH];
	int secretWordSize, tryCount, wrongCount, correctCount, correctFlag;
	int score, highScore, round, gameWon;
	char letter[MAX_WORD_LENGTH], correctLetters[MAX_WORD_LENGTH], wrongLetters[MAX_WRONG_LENGTH], message[MAX_MESSAGE_LENGTH];	
} GameState;

extern GameState state;

//persistent save data written to file
typedef struct{
    int highScore;
    int round;
    int score;
    int correctCount;
    int wrongCount;
    char secretWord[MAX_WORD_LENGTH];
    char correctLetters[MAX_WORD_LENGTH];
    char wrongLetters[MAX_WRONG_LENGTH];
    float mainVolume;
    float musicVolume;
} SaveData;

void initGame();
void checkWord();
void loadSecretWord(char *word, int *wordSize);
void giveHint(char *word, char *displayedLetters);
int calculateScore();
SaveData loadSaveData();
void writeSaveData(SaveData *data);
int gameShouldEnd();
void sanitizeInput(char *letters);
void resetVariables();
int getRandomNumber(int MIN, int MAX);
void restoreGameState(SaveData *saveData);
void persistGameState(SaveData *saveData);

#endif