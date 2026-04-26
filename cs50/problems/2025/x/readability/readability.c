#import <stdio.h>
#import <cs50.h>
#import <string.h>
#import <ctype.h>
#import <math.h>

int calculate_idx(float l_s[]);
void avg_l_s(string text, float l_s[]);
void print_ans(int idx);
bool end_sentence(char c);
bool end_word(char c);

int main(void) {
  string text = get_string("Text: ");

  float l_s[2];

  avg_l_s(text, l_s);

  int idx = calculate_idx(l_s);

  print_ans(idx);

  return 0;
}

int calculate_idx(float l_s[]) {
  return round(0.0588 * l_s[0] - 0.296 * l_s[1] - 15.8);
}

void print_ans(int idx) {
  if(idx >= 16) {
    printf("Grade 16+\n");
    return;
  }
  if (idx > 1) {
    printf("Grade %d\n", idx);
    return;
  }
  printf("Before Grade 1\n");
}

void avg_l_s(string text, float l_s[]) {
  int letter_cnt = 0;
  int sentence_cnt = 0;

  int words_cnt = 0;

  for(int i = 0; i < strlen(text); i++) {
      if(isalpha(text[i])) {
        letter_cnt++;
        continue;
      }
      if(isspace(text[i]) && !isspace(text[i - 1]) && !end_sentence(text[i - 1])){
       words_cnt++;
       continue;
      }
      if(end_sentence(text[i])) {
        sentence_cnt++;
        words_cnt++;
      }
  }

  float l = ((float) letter_cnt / (float) words_cnt) * 100.0;
  float s = ((float) sentence_cnt / (float) words_cnt) * 100.0;
  l_s[0] = l;
  l_s[1] = s;

}

bool end_sentence(char c) {
  return c == '!' || c == '?' || c == '.';
}
