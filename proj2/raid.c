#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char hamming74(int nibble)
{
    // put in 0110 -> 1100110
    unsigned int x3 = (nibble >> 3) & 1; // rightmost bit
    unsigned int x2 = (nibble >> 2) & 1;
    unsigned int x1 = (nibble >> 1) & 1;
    unsigned int x0 = (nibble >> 0) & 1; // leftmost bit
    unsigned int p1 = x3 ^ x0 ^ x2;
    unsigned int p2 = x1 ^ x0 ^ x3;
    unsigned int p4 = x0 ^ x1 ^ x2;
    // printf("%d%d%d%d%d%d%d\n", p1,p2,x3,p4,x2,x1,x0);
    // printf("%d%d%d%d\n", x3, x2, x1, x0);

    unsigned char binary = ((p1 << 7) | (p2 << 6) | (x3 << 5) | (p4 << 4) | (x2 << 3) | (x1 << 2) | (x0 << 1)) >> 1;

    // unsigned char binary = 0b0;
    return binary;
} 

void encodeRAID2(char *inputFilename)
{
    // Open the input file for reading
    FILE *inputFile = fopen(inputFilename, "rb");
    if (inputFile == NULL)
    {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Create seven output files
    FILE *outputFiles[7];
    char outputFilenames[7][256];
    // 7 buffers, 8 bits per buffer
    char suffixes[7][8] = {"part0", "part1", "part2", "part3", "part4", "part5", "part6"};

    for (int i = 0; i < 7; i++)
    {
        snprintf(outputFilenames[i], sizeof(outputFilenames[i]), "%s.%s", inputFilename, suffixes[i]);
        outputFiles[i] = fopen(outputFilenames[i], "wb");
        if (outputFiles[i] == NULL)
        {
            perror("Error creating output file");
            exit(EXIT_FAILURE);
        }
    }

    // Perform RAID2 encoding

    int dataBlocks[4] = {0};   // Data blocks: D1, D2, D3, D4
    int parityBlocks[3] = {0}; // Parity blocks: P1, P2, P4
    int index = 0;
    int byte;

    while (1)
    {
        byte = fgetc(inputFile); // get char from input file
        unsigned char nibble_left = (byte >> 4) & 0x0F;
        unsigned char nibble_right = byte & 0x0F;
        if (nibble_left ^ nibble_right)
        {
            printf("%d : ", nibble_left);
            printf("%d\n", hamming74(nibble_left));
            printf("%d : ", nibble_right);
            printf("%d\n", hamming74(nibble_right));

        //    hamming74(nibble_right);
            
        }

        if (byte == EOF)
        {
            break;
        }

        index++;
    }

    // Close all the files
    for (int i = 0; i < 7; i++)
    {
        fclose(outputFiles[i]);
    }

    fclose(inputFile);
}

int main(int argc, char *argv[])
{
    if (argc != 3 || strcmp(argv[1], "-f") != 0)
    {
        printf("Usage: %s -f filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *inputFilename = argv[2];

    
    encodeRAID2(inputFilename);

    return EXIT_SUCCESS;
}
