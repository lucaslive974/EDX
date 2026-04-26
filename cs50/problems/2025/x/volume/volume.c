// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
typedef int8_t byte;
typedef int16_t sample;

void amplifyVolume(FILE* input, FILE * output, float factor);
void copyHeader(FILE *input, FILE *output);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    amplifyVolume(input, output, factor);

    // TODO: Read samples from input file and write updated data to output file
    // Close files
    fclose(input);
    fclose(output);
}


void amplifyVolume(FILE* input, FILE* output, float factor) {
    copyHeader(input, output);

    sample sample_digit;

    while(fread(&sample_digit, sizeof(byte), 2, input)) {
        sample sample_amplified = sample_digit * factor;
        fwrite(&sample_amplified, sizeof(sample), 1, output);
    }
}

void copyHeader(FILE* input, FILE* output) {
    byte header[44];

    fread(&header, sizeof(byte), 44, input);
    fwrite(&header, sizeof(byte), 44, output);
}
