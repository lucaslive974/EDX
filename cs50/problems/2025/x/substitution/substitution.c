#include <string.h>
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int validate_key(string s);
string to_upper_case(string s);
string encrypt(string s, string key);

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    if(strlen(argv[1]) != 26) {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    string key = to_upper_case(argv[1]);
    int is_invalid = validate_key(key);

    if(is_invalid) {
        printf("Invalid key\n");
        return 1;
    }

    string plaintext = get_string("plaintext: ");
    string cipher = encrypt(plaintext, key);

    printf("ciphertext: %s\n", cipher);

    free(cipher);

    return 0;
}

int validate_key(string s) {
    int dp[26] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    for(int i = 0; i < strlen(s); i++) {
        int code = s[i] - 'A';
        if(++dp[code] > 1) return 1;
    }
    return 0;
}

string to_upper_case(string s) {
    for(int i = 0; i < strlen(s); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}

string encrypt(string s, string key) {
    string cipher = malloc(strlen(s) + 1);
    int i = 0;
    while(i < strlen(s)) {
        if(!isalpha(s[i])) {
            cipher[i] = s[i];
            i++;
            continue;
        }
        int code = toupper(s[i]) - 'A';
        bool is_upper = isupper(s[i]);
        cipher[i] = (is_upper) ? toupper(key[code]) : tolower(key[code]);
        i++;
    }

    cipher[i] = '\0';

    return cipher;
}

