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

void printCodes(struct Node *root, int arr[], int top, int closed, char filename[])
{

    if (closed == 0)
    {
        fopen(filename, "w+");
        closed = 1;
    }
    if (root->right)
    {
        // printf("%c", root->data);
        arr[top] = 0;
        printCodes(root->right, arr, top + 1, closed, filename);
    }
    if (root->left)
    {
        arr[top] = 1;
        printCodes(root->left, arr, top + 1, closed, filename);
    }

    if (!root->left && !root->right)
    {

        if (root->freq > 0)
        {
            FILE *file = fopen("h.out", "a");
            printf("%c\t%d\t", root->data, root->freq);
            for (int i = 0; i < top; i++)
            {

                printf("%d", arr[i]);
                fprintf(file, "%d", arr[i]);
            }
            fclose(file);

            printf("\n");
        }
    }
}

void findHuffmanCode(struct Node *root, char targetChar, char *currentPath, int depth, FILE *file3)
{
    if (root == NULL)
    {
        return;
    }

    // Append '0' for left branch and '1' for right branch
    if (root->left != NULL)
    {
        currentPath[depth] = '1';
        findHuffmanCode(root->left, targetChar, currentPath, depth + 1, file3);
    }
    if (root->right != NULL)
    {
        currentPath[depth] = '0';
        findHuffmanCode(root->right, targetChar, currentPath, depth + 1, file3);
    }

    // If it's a leaf node and matches the target character, print the code
    if (root->left == NULL && root->right == NULL && root->data == targetChar)
    {
        currentPath[depth] = '\0'; // Null-terminate the string
        // printf("Huffman code for '%c': %s\n", targetChar, currentPath);
        // put the huffman code into h.out
        // fprintf(file3, "%s", currentPath);
        fwrite(currentPath, sizeof(char), (sizeof(currentPath) / sizeof(char)), file3);
    }
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

        // Create a new node with the sum of the frequencies of the two lowest nodes
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
{ // argc==#of args, argv==args
    // char filename[] = "test.txt";
    char filename[] = "completeShakespeare.txt";

    // char filename[] = "completeShakespeare.txt";
    char output_filename[] = "h.out";
    // unsigned char binaryData[] = {0x12, 0x34, 0x56, 0x78};
    int frequencies[256] = {0}; // index == ASCII code
    int character;              // To store the read character

    FILE *file;
    FILE *file3 = fopen("h.out", "wb");


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

    int arr[100]; // Assuming a maximum code length of 100
    int top = 0;

    printf("Huffman Codes:\n");

    // printCodes(root, arr, top, 0, output_filename);

    char currentPath[100];


    FILE *file2 = fopen(filename, "r");


    while ((character = fgetc(file2)) != EOF)
    {
        findHuffmanCode(root, character, currentPath, 0, file3);

    }
    return 0;
}