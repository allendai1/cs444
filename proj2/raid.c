#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *arrToBin(unsigned char arr[])
{
    char binaryString[9];
    char *str_to_ret = malloc(sizeof(char) * 9);
    char term = '\0';
    strcpy(binaryString, "");

    for (int p = 0; p < 8; p++)
    {
        char bitChar = (arr[p] == 1) ? '1' : '0';
        strcat(str_to_ret, &bitChar);
    }

    return str_to_ret;
}
unsigned char hamming74(int nibble)
{
    unsigned int x3 = (nibble >> 3) & 1; // rightmost bit
    unsigned int x2 = (nibble >> 2) & 1;
    unsigned int x1 = (nibble >> 1) & 1;
    unsigned int x0 = (nibble >> 0) & 1; // leftmost bit
    unsigned int p1 = x3 ^ x0 ^ x2;
    unsigned int p2 = x1 ^ x0 ^ x3;
    unsigned int p4 = x0 ^ x1 ^ x2;
    unsigned char binary = ((p1 << 7) | (p2 << 6) | (x3 << 5) | (p4 << 4) | (x2 << 3) | (x1 << 2) | (x0 << 1)) >> 1;
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
    //{"p1", "p2", "x3", "p4", "x2", "x1", "x0"}

    char suffixes[7][8] = {"part0", "part1", "part2", "part3", "part4", "part5", "part6"};

    unsigned char buffers[7][8];

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

    int index = 0;
    int byte;
    int lindex = 0;
    int rindex = 1;
    int bufferCount = 0;

    while (1)
    {
        byte = fgetc(inputFile); // get char from input file
        if (byte == EOF)
        {
            // buffer may not be full, but write the remaining bits
            break;
        }
        unsigned char nibble_left = (byte >> 4) & 0x0F;
        unsigned char nibble_right = byte & 0x0F;

        // if (nibble_left && nibble_right)
        // {
        int i;

        unsigned char hl = hamming74(nibble_left);  // 102 = 1100110
        unsigned char hr = hamming74(nibble_right); // 105 = 1101001
        // turn the binary into a binary int array

        int hlBinary[7];
        int hrBinary[7];
        for (int p = 6; p >= 0; p--)
        {
            hlBinary[p] = (hl >> 6 - p) & 1;
            hrBinary[p] = (hr >> 6 - p) & 1;
        }

        // for each p1,p2,p4,x0,x1,x2,x3, place lbitrbit into each
        for (i = 0; i < 7; i++)
        {
            // put leftmost bit of each and put into p1,p2, etc.
            char a = hlBinary[i];
            char b = hrBinary[i];
            buffers[i][lindex] = a;
            buffers[i][rindex] = b;
            bufferCount += 2;
        }

        // increase index for where to place the bits
        lindex += 2;
        rindex += 2;
        // reset position

        // } // end if
        // every 4 chars read, output to file and clear buffer
        if (bufferCount == 56)
        {
            // for each buffer, print and output it
            for (int j = 0; j < 7; j++)
            {

                char *endptr;
                const char *binaryString = arrToBin(buffers[j]);
                long decimalValue = strtol(binaryString, &endptr, 2);
                // printf("Binary String: %s\n", binaryString);
                // printf("Decimal Value: %ld\n", decimalValue);

                fputc(decimalValue, outputFiles[j]);
            }


            lindex = 0;
            rindex = 1;
            bufferCount = 0;
        }
    }
    // end of while

    // (bufferCount / 8) full buffers to put into disk + bufferCount % 8 bits into the next disk
    // printf("\n%d extra bits in buffer\n", bufferCount);

    // if (bufferCount > 0)
    // {
    //     int bufferRemainder = (bufferCount / 7);
    //     for (int i = 0; i < 7; i++)
    //     {
    //         char *endptr;

    //         const char *binaryString = arrToBin(buffers[index]);

    //         long decimalValue = strtol(binaryString, &endptr, 2);
    //         fputc((decimalValue >> (8 - bufferRemainder)), outputFiles[i]);
    //         bufferCount -= bufferRemainder;
    //     }
    // }
    // printf("%d extra bits in buffer\n", bufferCount);

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
