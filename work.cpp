#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define maxword_len 50

int getword( char *chosenword)
{
	FILE *fp;
	fp=fopen("words.txt","r");
	if(fp==NULL)
	{
		printf("error");
		return 0;
	}
	
	char word[maxword_len];
	int wordcount=0;
	while(fscanf(fp,"%s",word)!=EOF)
	{
		wordcount ++;
		
	}
	//printf("%d\n",wordcount);
	if(wordcount==0)
	{
		printf("word file is empty\n");
		fclose(fp);
	    return 0;
	}
	int random=rand()%wordcount;
	rewind(fp);
	int currentline=0;
	while(fscanf(fp,"%s",word)!=EOF)
	{
		if(currentline==random)
		{
			strcpy(chosenword,word);
			break;
		}
		currentline++;
	}
	fclose(fp);
	return 1;

}
void maskword(const char *secretword, char *maskedword) {
    int length = strlen(secretword);
    for (int i = 0; i < length; i++) {
        maskedword[i] = '_';
    }

    maskedword[length] = '\0'; 
}
int main() {
    srand(time(NULL));

    char secretword[maxword_len];
    char maskedword[maxword_len];

    if (getword(secretword)) {
        //printf("Successfully picked a secret word!\n");
        	
        maskword(secretword, maskedword);

        //printf("Secret Word : %s\n", secretword);
        printf("Masked Word : %s\n", maskedword);
        //printf("Word Length: %lu\n", strlen(secretword));
    }
    else{
        printf("DEBUG: The function failed! Either words.txt is missing from the folder, or it's empty.\n");
    }

    return 0;
}