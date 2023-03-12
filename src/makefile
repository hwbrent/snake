main:
	gcc -c main.c
	gcc -c screen/screen.c -o screen/screen.o
	gcc main.o screen/screen.o
	./a.out
	make clean

comp:
	gcc -c main.c -o screen/main.o

clean:
	rm main.o
	rm screen/screen.o
	rm a.out
