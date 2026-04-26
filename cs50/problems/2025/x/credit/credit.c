#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool checksum(char* number);
int plusdigits(int product);
int convertInt(char c);
char* parseInput(char* number);
char* identify(char* cardNumber);

int main(void) {
  char* input = get_string("Number: ");
  char* cardNumber = parseInput(input);
  bool isValid = checksum(cardNumber);

  if(!isValid) {
    printf("INVALID\n");
    return 0;
  }

  printf("%s", identify(cardNumber));

  free(cardNumber);
  return 0;
}

char* parseInput(char* input) {
  char* cardNumber = malloc(sizeof(input) + 1);
  int j = 0;

  for(int i = 0; i < strlen(input); i++) {
    if(!isdigit(input[i])) continue;

    cardNumber[j] = input[i];
    j++;
  }

  cardNumber[j] = '\0';
  return cardNumber;
}

char* identify(char* cardNumber) {
  char* i = malloc(3);
  char* res = "INVALID\n";
  int len = strlen(cardNumber);

  strncpy(i, cardNumber, 2);
  i[3] = '\0';

  if(!strncmp(i, "4", 1) && (len == 13 || len == 16))  res = "VISA\n";
  if((!strncmp(i, "34", 2) || !strncmp(i, "37", 2)) && len == 15) res = "AMEX\n";

  int j = atoi(i);
  if((j > 50 && j < 56) && len == 16) res = "MASTERCARD\n";

  free(i);
  return res;
}

bool checksum(char* cardNumber) {
  int idx = strlen(cardNumber) - 1;
  int res = 0;

  int cnt = 1;
  for(int i = idx; i >= 0; i--) {
    int val = convertInt(cardNumber[i]);
    if(cnt % 2 == 0) {
      res += plusdigits(val * 2);
    } else {
      res += val;
    }
    cnt++;
  }

  return res % 10 == 0;
}

int plusdigits(int product) {
  return (product < 10) ? product : (product / 10) + (product % 10);
}

int convertInt(char c) {
  return c - '0';
}
