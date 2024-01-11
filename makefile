cflags = -g -Wall -pedantic-errors -D_GNU_SOURCE -Wno-switch -std=c2x

parsetest.exe : main.o scanner.o parser.o calc.o
	gcc main.o scanner.o parser.o calc.o -o parsetest -LC:\\msys64\\usr\\lib\\ -lfl

main.o : main.c scanner.h parser.h calc.h
	gcc $(cflags) -c main.c

scanner.o: scanner.c scanner.h
	gcc $(cflags) -c scanner.c

parser.o: parser.c parser.h
	gcc $(cflags) -c parser.c

scanner.c scanner.h : scanner.l parser.y
	flex scanner.l

parser.c parser.h : scanner.l parser.y 
	bison -d parser.y

calc.o : calc.c calc.h parser.h
	gcc $(cflags) -c calc.c

clean:
	rm main.o parser.o scanner.o calc.o scanner.c scanner.h parser.c parser.h parsetest.exe