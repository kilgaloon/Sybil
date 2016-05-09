CFLAGS=-Wall -g

compile:
	gcc -g main.c config.c lib/parser.c lib/storage.c -o sybil

clean:
	rm -f sybil

memory-check:
	valgrind --leak-check=yes --track-origins=yes ./sybil