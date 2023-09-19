#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){ // argc==#of args, argv==args
    char filename[] = "completeShakespeare.txt";
    char output_filename[] = "huffman.out";
    if (argc == 5){
        printf("You have entered %d arguments:\n", argc);
        // check for -i flag
        if (argv[1] == "-i"){ 
            strcpy(filename, argv[2]);

        }
        // check for -o flag
        if (argv[3] == "-o"){ 
            strcpy(output_filename, argv[4]);

    }
    }



   




    
    
    
       
  

   

    return 0;
}