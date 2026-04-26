#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printRow(int row, int height);
void printRows(int height);
char* repeatChar(char c, int qtd);

int main(void) {
  int height;

  while(true) {
    height = get_int("Insert the desired pyramid height: ");
    if(height > 0 && height < 9) break;
    printf("Invalid height, input again.\n");
  }

  printRows(height);

  return 0;
}

void printRows(int height) {
  for(int i = 1; i <= height; i++) {
    printRow(i, height);
    printf("\n");
  }
}

void printRow(int row, int height) {
  char* whiteSpaces = repeatChar(' ', height - row);
  char* hashes = repeatChar('#', row);

  printf("%s%s  %s", whiteSpaces, hashes, hashes);

  free(whiteSpaces);
  free(hashes);
}

char* repeatChar(char c, int qtd) {
  char* string = malloc(qtd + 1);

  for(int i = 0; i < qtd; i++) {
    string[i] = c;
  }
  string[qtd] = '\0';

  return string;
}
