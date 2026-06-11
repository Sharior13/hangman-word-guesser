#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define MAX_TRIES 7
#define MAX_WRONGS 5
int main()
{
	int tryCount=0, wrongCount=0, correctCount=0;
	char word[3]= "who";
	char letter, guessedLetters[5] = {0};
	printf("====================== Hangman Word Guesser ======================\n");
	printf("Press enter to play:");
	getch();
	while(tryCount < MAX_TRIES){
		printf("Word: ");
		for(int i=0; i<sizeof(word); i++){
			if(word[i]==guessedLetters[i]){
				printf("%c ", word[i]);
			}
			else{
				printf("_ ");
			}
		}
		printf("\n");
		printf("Enter a guess: ");
		scanf(" %c", &letter);
		if(letter == word[correctCount]){
			printf("Correct!\n");
			guessedLetters[correctCount] = letter;
			correctCount++;
		}
		else{
			printf("Incorrect!\n");
			wrongCount++;
		}
		tryCount++;
		if(correctCount == sizeof(word)){
			printf("You guessed correctly!");
			exit(0);
		}
		else if(tryCount >= MAX_TRIES){
			printf("Out of live! The correct word was %s.", word);
		}
	}
	
    return 0;
}