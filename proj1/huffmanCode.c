#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char data;
    int freq;
    struct Node *left;
    struct Node *right;
};

struct Node *newNode(char data, int freq)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->data = data;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printCodes(struct Node *root, int arr[], int top)
{

    if (root->right)
    {
        arr[top] = 0;
        printCodes(root->right, arr, top + 1);
    }
    if (root->left)
    {
        arr[top] = 1;
        printCodes(root->left, arr, top + 1);
    }

    if (!root->left && !root->right)
    {

        if (root->freq > 0)
        {
            printf("%c\t%d\t", root->data, root->freq);
            for (int i = 0; i < top; i++)
            {

                printf("%d", arr[i]);
            }

            printf("\n");
        }
    }
}

char *findHuffmanCode(struct Node *root, char targetChar, char *binaryString, int depth, FILE *file3)
{
    if (root == NULL)
    {
        return NULL;
    }

    // Append '0' for left  and '1' for right
    if (root->left != NULL)
    {
        binaryString[depth] = '1';
        char *leftRes = findHuffmanCode(root->left, targetChar, binaryString, depth + 1, file3);
        if (leftRes != NULL)
        {
            return leftRes;
        }
    }

    if (root->right != NULL)
    {
        binaryString[depth] = '0';
        char *rightRes = findHuffmanCode(root->right, targetChar, binaryString, depth + 1, file3);
        if (rightRes != NULL)
        {
            return rightRes;
        }
    }

    // target char is found, return the binary as a string
    if (root->left == NULL && root->right == NULL && root->data == targetChar)
    {
        char *result = (char *)malloc((depth + 1) * sizeof(char));
        if (result != NULL)
        {
            strncpy(result, binaryString, depth);
            result[depth] = '\0';
        }
        return result;
    }

    return NULL; // Return NULL if the targetChar is not found in the subtree
}

struct Node *buildHuffmanTree(struct Node *nodes[], int size)
{
    while (size > 1)
    {
        // Find the two nodes with the lowest frequencies
        int min1 = 0, min2 = 1;
        if (nodes[min1]->freq > nodes[min2]->freq)
        {
            min1 = 1;
            min2 = 0;
        }

        for (int i = 2; i < size; i++)
        {
            if (nodes[i]->freq < nodes[min1]->freq)
            {
                min2 = min1;
                min1 = i;
            }
            else if (nodes[i]->freq < nodes[min2]->freq)
            {
                min2 = i;
            }
        }

        // Create a new node with the sum of the frequencies
        struct Node *mergedNode = newNode('\0', nodes[min1]->freq + nodes[min2]->freq);
        mergedNode->left = nodes[min1];
        mergedNode->right = nodes[min2];

        // Remove the two lowest nodes and add the merged node
        nodes[min1] = mergedNode;
        nodes[min2] = nodes[size - 1];

        size--;
    }

    // The remaining node is the root of the Huffman tree
    return nodes[0];
}
int main(int argc, char *argv[])
{

    char filename[] = "completeShakespeare.txt";
    char output_filename[] = "huffman.out";
    int frequencies[256] = {0};
    int character;

    // Check for flags
    if (argc == 5)
    {

        // check for -i flag
        if (argv[1] == "-i")
        {
            strcpy(filename, argv[2]);
        }
        // check for -o flag
        if (argv[3] == "-o")
        {
            strcpy(output_filename, argv[4]);
        }
    }
    FILE *file;
    FILE *file3 = fopen(output_filename, "wb");

    file = fopen(filename, "r");

    // Create an array of character frequencies
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        frequencies[c]++;
    }
    fclose(file);

    struct Node *nodes[256];
    for (int i = 0; i < 256; i++)
    {
        char ch = i;
        nodes[i] = newNode(ch, frequencies[i]);
    }

    struct Node *root = buildHuffmanTree(nodes, 256);

    int arr[500];
    int top = 0;

    printf("Huffman Codes:\n");

    printCodes(root, arr, top);

    char binaryString[500];

    // read in specified filename
    FILE *file2 = fopen(filename, "r");

    int bitCount = 0;
    char byteBuffer = 0;
    int i;

    // A  B  C     D D  A A A A D
    // 0 110 1110 10 10 0 0 0 0 10
    while ((character = fgetc(file2)) != EOF)
    {

        // get huffman code for character
        char *x = findHuffmanCode(root, character, binaryString, 0, file3); // ex. "110"
        // go through the binary string and add each digit to buffer
        for (i = 0; i < strlen(x); i++)
        {
            byteBuffer <<= 1;     // make space for adding digit to the right
            int bit = x[i] - '0'; // Turn '0' into 0 and '1' into 1
            byteBuffer |= bit;
            bitCount++;

            // if buffer full, write it and empty it
            if (bitCount == 8)
            {
                fwrite(&byteBuffer, 1, 1, file3);
                // reset buffer
                bitCount = 0;
                byteBuffer = 0;
            }
        }
    }
    if (bitCount > 0)
    {
        fwrite(&byteBuffer, 1, 1, file3);
    }
    return 0;
}