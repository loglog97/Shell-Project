wish: wish.o commandParser.o built_in.o
	gcc -o wish wish.o commandParser.o built_in.o

wish.o: wish.c
	gcc -c wish.c

commandParser.o: commandParser.c
	gcc -c commandParser.c

built_in.o: built_in.c
	gcc -c built_in.c

clean:
	rm -f wish *.o submission.zip

zip:
	zip submission.zip *.c *.h Makefile