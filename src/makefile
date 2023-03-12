main:
	gcc -c main.c
	gcc -c screen/screen.c -o screen/screen.o
	gcc -c snake/snake.c -o snake/snake.o
	gcc main.o screen/screen.o snake/snake.o
	./a.out
	make clean

clean:
	rm main.o
	rm screen/screen.o
	rm snake/snake.o
	rm a.out
