main: food/src/food.c game/src/game.c screen/src/screen.c snake/src/snake.c main.c a.out *.o
	@gcc -c food/src/food.c
	@gcc -c game/src/game.c
	@gcc -c screen/src/screen.c
	@gcc -c snake/src/snake.c
	@gcc -c main.c
	@gcc *.o -lncurses -lpthread
	@./a.out
	@make clean

clean: a.out *.o
	@rm *.o
	@rm a.out
