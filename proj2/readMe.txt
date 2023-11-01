1. Allen Dai

2. For proj2, I have 2 files: raid.c and diar.c to encode and decode.
-raid.c
    Takes 2 command line arguments, first being flag -f then the txt file.
    When compiled and ran, the program takes the txt file name and stores it
    in a variable. Then `encodeRAID2` is ran with the file name as the arg.
    encodeRAID2 reads the input file and creates 7 output files with the
    appropriate names. Then using the input file, it reads one character 
    at a time and breaks it into 2 nibbles, nibble_left/nibble_right. The
    Hamming(7,4) algorithm is run on both nibbles(using helper function hamming74()), and each bit is added to 
    one of the 7 buffers. Once each buffer contains 8 bits, it is full and 
    each byte is written to its respective file from the buffer. My implementation
    of a buffer is in an array, so there is a helper function arrToBin to convert from 
    binary array to decimal. 


-diar
    Takes 4 arguments: -f text.txt -s byteSize
    The program takes the text file name, and opens a new file with the '.2' extension.
    It opens the 7 ouptut part files and then reads characters until EOF. Each char in
    file is stored as int then put into int[] temp_buffer which will contain 7 ints
    where each int is the integer representation of the character read. Using bit shifting
    and masking, we take the most significant bit on the left of each int, and then add them
    together to form a 7 bit binaryString. Then using hamming74 algorithm, the 7 bit binaryString
    is error checked and parity bits removed, leaving a 4 bit nibble in which its stored
    in a res_buffer. We do this process again with the next character so that another 4 bit nibble
    is put into res_buffer, making it a full byte. Every time the res_buffer is full with 8 bits,
    the byte is written to the output file.


3. Makefile

'CC=cc' : This line creates variable CC and sets it to cc which specifies which compiler to use

'CFLAGS=-Wall -O2' : This creates variable CFLAGS setting it to -Wall -02. This essentially sets
                    the variable CFLAGS to contain compiler flags which control the compilation process.
                    -Wall allows compiler warnings and -02 is for optimization


'all: raid diar' : This specifies that when no argument is given, it will build both raid and diar

'%: %.c' : This rule specifies how to build executables from C files with '%' and '.c' files 
        It uses previous defined variables CC and CFLAGS to compile C files. '$@ refers to
        the target file while '$<' is the dependency file.

'clean:
	rm -f a.out *.part? *.2' : This specifies a command clean so that a.out file is removed and
                                files ending in .part and .2 are also removed.



4. getopt
    raid.c reads command line arguments using argc and argv. It checks if there's exactly
    3 arguments and checks if second argument is the flag '-f'. If these cases fail,
    then it will return error, otherwise, set our inputFilename to argv[2].

    diar.c reads arguments using a while loop and getopt. It will continuously look
    for the flags f and s and using a switch case, if its f then store the filename, and 
    if its s, store the number of bytes.


