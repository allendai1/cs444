CC = gcc


all: huffman_execute.out
	./huffman_execute.out -i completeShakespeare.txt -o huffman.out

huffman_execute.out: huffman.c
	$(CC)  -o $@ $^

clean:
	rm -f huffman_execute.out huffman.out

