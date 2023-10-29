void findHuffmanCode(struct Node *root, char targetChar, char *currentPath, int depth, FILE *file3, int *bitCount, char *byteBuffer)
{
    if (root == NULL)
    {
        return;
    }

    // Append '0' for left branch and '1' for right branch
    if (root->left != NULL)
    {
        currentPath[depth] = '0';
        findHuffmanCode(root->left, targetChar, currentPath, depth + 1, file3, bitCount, byteBuffer);
    }

    if (root->right != NULL)
    {
        currentPath[depth] = '1';
        findHuffmanCode(root->right, targetChar, currentPath, depth + 1, file3, bitCount, byteBuffer);
    }

    // If it's a leaf node and matches the target character
    if (root->left == NULL && root->right == NULL && root->data == targetChar)
    {
        int i;
        for (i = 0; i < depth; i++)
        {
            *byteBuffer <<= 1;
            int bit = currentPath[i] - '0'; // Turn '0' into 0 and '1' into 1
            *byteBuffer |= bit;

            (*bitCount)++;

            if (*bitCount == 8)
            {
                fwrite(byteBuffer, 1, 1, file3);
                *bitCount = 0;
                *byteBuffer = 0;
            }
        }

        if (*bitCount > 0)
        {
            // Write any remaining bits
            fwrite(byteBuffer, 1, 1, file3);
        }
    }
}
