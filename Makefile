parser: driver.o parser.o keytoktab.o lexer.o symtab.o optab.o
	gcc -Wall -o parser driver.o parser.o keytoktab.o lexer.o symtab.o optab.o

driver.o: driver.c
	gcc -Wall -c driver.c

parser.o: parser.c
	gcc -Wall -c parser.c

keytoktab.o: keytoktab.c
	gcc -Wall -c keytoktab.c

lexer.o: lexer.c
	gcc -Wall -c lexer.c

symtab.o: symtab.c
	gcc -Wall -c symtab.c

optab.o: optab.c
	gcc -Wall -c optab.c

test:
	./buildall

clean:
	rm -f parser *.o *.old