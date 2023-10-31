#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

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
int detectError(unsigned char buffer[7])
{
    int errorCode = 0;
    // P1 ^ D1 ^ D2 ^ D4
    if (buffer[0] ^ buffer[2] ^ buffer[4] ^ buffer[6] == 1)
    {
        errorCode = 1;
    } // P2 ^ D1 ^ D3 ^ D4
    else if (buffer[1] ^ buffer[2] ^ buffer[5] ^ buffer[6])
    {
        errorCode = 2;
    } // P4 ^ D2 ^ D3 ^ D4
    else if (buffer[3] ^ buffer[4] ^ buffer[5] ^ buffer[6])
    {
        errorCode = 4;
    }
    return errorCode;
}

void decodeRAID2(char *inputFilename)
{
    // FILE *inputFile = fopen(inputFilename, "rb");
    // take file name, open filename.part0, filename.part1, etc.
    FILE *outputFiles[7];
    char outputFilenames[7][256];
    char suffixes[7][8] = {"part0", "part1", "part2", "part3", "part4", "part5", "part6"};
    int temp_buffer[7];
    unsigned char buffers[7][8];
    unsigned char binaryString[7];
    int errorCode = 0;
    int bufferCount = 0;
    unsigned char res_buffer[8];
    char outputFilename[256];
    strcpy(outputFilename, inputFilename);
    strcat(outputFilename, ".2");
    FILE *outputFile = fopen(outputFilename, "w");

    // Open all the filename parts
    for (int i = 0; i < 7; i++)
    {
        snprintf(outputFilenames[i], sizeof(outputFilenames[i]), "%s.%s", inputFilename, suffixes[i]);

        outputFiles[i] = fopen(outputFilenames[i], "rb");
    }
    // continuously take a byte from each and fill our temp_buffer
    while ((temp_buffer[0] = fgetc(outputFiles[0])) != EOF)
    {
        // continuously take byte from each part
        for (int i = 1; i < 7; i++)
        {
            

            int byte = fgetc(outputFiles[i]);
            temp_buffer[i] = byte; // fill the buffer
        }

        // get the binary string from taking ith of each buffer
        for (int i = 0; i < 8; i++)
        {
            // create binary string using 1 bit from each buffer 
            for (int j = 0; j < 7; j++)
            {

                // ((temp_buffer[j] >> (7-k-1)) % 2)
                // take that decimal and get the most signifigant digit/left-most
                binaryString[j] = ((temp_buffer[j] >> (8-i-1)) % 2);
                // printf("%d ", binaryString[j]);
            }
            // printf("\n");
            // check binary string for error
            errorCode = detectError(binaryString);
            if (errorCode != 0)
            {
                binaryString[errorCode] = 1 - binaryString[errorCode];
            }
            if (bufferCount == 4) // buffer is full so write output and empty
            {
                res_buffer[4] = binaryString[2];
                res_buffer[5] = binaryString[4];
                res_buffer[6] = binaryString[5];
                res_buffer[7] = binaryString[6];
              
                
                // take the buffer [1,0,0,1,0,0,1,1] and turn it into binary then ASCII
                char *endptr;

                long decimalValue = strtol(arrToBin(res_buffer), &endptr, 2);
                fputc(decimalValue, outputFile);
                // printf("decimal value: %lu\n", decimalValue);

                bufferCount = 0;
            }
            else
            { // otherwise put bits at 0,1,2,3
                res_buffer[0] = binaryString[2];
                res_buffer[1] = binaryString[4];
                res_buffer[2] = binaryString[5];
                res_buffer[3] = binaryString[6];
                bufferCount += 4;
            }
        }
    }

    // Close all the file name parts
    for (int i = 0; i < 7; i++)
    {
        fclose(outputFiles[i]);
    }
    fclose(outputFile);
}

int main(int argc, char *argv[])
{
    char *filename = NULL;
    int numberOfBytes = 0;

    int opt;
    while ((opt = getopt(argc, argv, "f:s:")) != -1)
    {
        switch (opt)
        {
        case 'f':
            filename = optarg;
            break;
        case 's':
            numberOfBytes = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s -f filename.txt -s numberOfBytes\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Check if both -f and -s options were provided
    if (filename == NULL || numberOfBytes == 0)
    {
        fprintf(stderr, "Both -f and -s options are required.\n");
        exit(EXIT_FAILURE);
    }

    // Now you can use 'filename' and 'numberOfBytes' in your program
    printf("File: %s\n", filename);
    printf("Number of Bytes: %d\n", numberOfBytes);
    decodeRAID2(filename);

    return 0;
}
