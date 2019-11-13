#include <stdio.h>
#include <stdlib.h>

int main(){
	char ch, letter;
	char word[100], sentence[100];
	int sentenceIndex = 0;

	scanf("%c", &letter);
	scanf("%s", word);
	getchar();
	while((ch = getchar()) != '\n'){
		sentence[sentenceIndex] = ch;
		sentenceIndex++;
	}
	sentence[sentenceIndex] = '\0';

	printf("%c\n", letter);
	printf("%s\n", word);
	printf("%s\n", sentence);

	return 0;
}