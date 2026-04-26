#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#define BLOCK_SIZE 512
typedef uint8_t BYTE;
typedef BYTE JPEG_SIGN[4];

static inline bool verify_signature(BYTE block[BLOCK_SIZE]);

int main(int argc, char *argv[])
{
    if(argc < 2) {
        printf("Usage: ./filter card.raw");
        return 1;
    }

    char* raw_file = argv[1];
    FILE* raw_ptr = fopen(raw_file, "r");
    if(raw_ptr == NULL) {
        printf("Could not open %s", raw_file);
        return 1;
    }

    int cnt = 0;

    BYTE block[BLOCK_SIZE];
    FILE *out_ptr = NULL;
    char* file_name = malloc(sizeof(char) * 8);

    while(fread(&block, BLOCK_SIZE, 1, raw_ptr)) {
        if(verify_signature(block))  {
            if(out_ptr != NULL) fclose(out_ptr);

            sprintf(file_name, "%03i.jpg", cnt++);
            out_ptr = fopen(file_name, "w");
        }

        if(out_ptr != NULL) fwrite(block, BLOCK_SIZE, 1, out_ptr);
    }

    fclose(out_ptr);
    fclose(raw_ptr);
    free(file_name);

    return 0;
}

static inline bool verify_signature(BYTE block[BLOCK_SIZE]) {
    static JPEG_SIGN sign = { 0xff, 0xd8, 0xff, 0xe };
    return
        (block[0] == sign[0]) &&
        (block[1] == sign[1]) &&
        (block[2] == sign[2]) &&
        (block[3] >> 4 == sign[3]);
}
