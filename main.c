#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "utils.h"

#define MAX_WRONGS 5
int main()
{
    char word[8] = "football";
    int tryCount=0, wrongCount=0, correctCount=0, correctFlag=0;
	char letter, correctLetters[sizeof(word)], wrongLetters[26 - sizeof(word)];
    for(int i=0; i<sizeof(word);i++){
        correctLetters[i] = '_';
    }
    printf("====================== Hangman Word Guesser ======================\n");
    printf("Press enter to play:");
    getch();
    while(1>0){
        correctFlag = 0;
        printf("\nWord: ");
        for(int i=0; i<sizeof(word); i++){
            printf("%c ", correctLetters[i]);
        }
        printf("\n");
        printf("Enter a guess: ");
        scanf(" %c", &letter);
        for(int i=0; i<sizeof(word); i++){
            if(letter == word[i] && letter != correctLetters[i]){
                correctLetters[i] = word[i];
                correctCount++;
                correctFlag = 1;
            }
            else if(letter == correctLetters[i] || letter == wrongLetters[i]){
                printf("\nAlready guessed! Try something else.");
                correctFlag = 2;
                break;
            }
            else{
                wrongLetters[i] = letter;
            }
        }
        if(correctFlag==1){
            printf("\nCorrect guess!");
        }
        else if(correctFlag == 0){
            printf("\nIncorrect!");
            wrongCount++;
        }
        tryCount++;
        if(correctCount == sizeof(word)){
            printf("\nWord guessed successfully!");
            exit(0);
        }
    	else if(wrongCount >= MAX_WRONGS){
            printf("\nOut of lives! The correct word was %s.", word);
            exit(0);
        }
    }
	return 0;
}