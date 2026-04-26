#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
void compute_scores(int scores[], string words[]);
void menu(string quest[]);
void result(int scores[]);

int main(void) {
  string q[2];
  menu(q);

  int s[2];
  compute_scores(s, q);

  result(s);

  return 0;
}

void menu(string quest[]) {
  quest[0] = get_string("Player 1: ");
  quest[1] = get_string("Player 2: ");
}

void compute_scores(int scores[], string words[]) {
  for(int i = 0; i < 2; i++) {
    int res = 0;

    string word = words[i];
    for(int j = 0; j < strlen(word); j++) {
      if(!isalpha(word[j])) continue;
      res += POINTS[toupper(word[j]) - 'A'];
    }

    scores[i] = res;
  }
}

void result(int scores[]) {
  string res = "Tie!\0";
  if(scores[0] > scores[1]) res = "Player 1 wins!\0";
  if(scores[0] < scores[1]) res = "Player 2 wins!\0";

  printf("%s\n", res);
}
