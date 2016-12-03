main: main.o file_read.o string.o
	gcc main.o file_read.o string.o -o main
main.o:
	gcc -c main.c
file_read.o:
	gcc -c lib/file_read.c
string.o:
	gcc -c lib/string.c

clean:
	rm -rf *\.o main
