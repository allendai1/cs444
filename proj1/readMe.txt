Allen Dai

For this project, I put my code into huffman.c. It contains functions to build the huffman tree, print the huffman tree codes, and to find a code given a character.
The main function takes in command line argument flags to change the input/output files. It then reads the completeShakespeare.txt file and creates a frequencies array
to track each unique character's frequency. Given the character and its frequencies, the huffman tree is built. Once it is built, I go through the completeShakespeare.txt
file again, reading each character and using the findHuffmanCode on each character. With the huffman code for that character, the bits are put into a buffer then outputted
once the buffer reaches 8 bits or a full byte. Once all the bytes are outputted to huffman.out, the file is closed.

10/3/2023
My actual output differs by 2 bytes. I suspect it's how my huffman tree is implemented. My huffman code is "/       12      1101000011011010010" while the correct
one should be "/       12      110100001101101001". There is an extra 0 at the end of mine which could be due to how I implemented the tree or how it's traversed.