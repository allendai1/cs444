Allen Dai
10/3/2023

For this project, I put my code into huffman.c. It contains functions to build the huffman 
tree, print the huffman tree codes, and to find a code given a character.
The main function takes in command line argument flags to change the input/output files. 
It then reads the completeShakespeare.txt file and creates a frequencies array
to track each unique character's frequency. Given the character and its frequencies, the 
huffman tree is built. Once it is built, I go through the completeShakespeare.txt
file again, reading each character and using the findHuffmanCode on each character. With 
the huffman code for that character, the bits are put into a buffer then outputted
once the buffer reaches 8 bits or a full byte. Once all the bytes are outputted to huffman.out, the file is closed.

The tree is made using C structs, where I create a Node for the tree and a struct to create
a new node. Each node has properties like data, left, right, and frequency.

The buildHuffmanTree function creates the huffman tree by taking in all the nodes
and then finding the 2 nodes with lowest frquencies, in which they're merged
with a new node to combine them. The 2 nodes are then removed and the new node is added 
to the array.

The printCodes function recursively goes through the tree similar to a depth-first search, 
prints out the codes backwards.

The findHuffmanCode function is also similar as it recursively goes through
and instead of printing, it appends to a final string to return.

The Makefile runs `huffmanCode.c` and creats a huffman_execute.out and runs it

In the main function, the implemented functions are used to output the huffman encoding.
Starting with reading the .txt file, then finding the frequencies, and using the frequencies
to create the huffman tree. Then, read the .txt file again, and output the huffman code for each
character using the implemented findHuffmanCode function. To do this,
The returned binary string from findHuffmanCode is put into an 8-bit buffer, where
if the buffer is full, write the byte.

10/3/2023
My actual output differs by 2 bytes. I suspect it's how my huffman tree is implemented. My huffman code is "/       12      1101000011011010010" while the correct
one should be "/       12      110100001101101001". There is an extra 0 at the end of mine which could be due to how I implemented the tree or how it's traversed.