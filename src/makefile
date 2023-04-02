main: main.c
	@gcc main.c -lncurses -lpthread
	@./a.out
	@make clean

clean: a.out *.o
	@rm *.o
	@rm a.out

main2: food/src/food.c game/src/game.c screen/src/screen.c snake/src/snake.c main.c
	@gcc -c food/src/food.c -lncurses -lpthread
	@gcc -c game/src/game.c -lncurses -lpthread
	@gcc -c screen/src/screen.c -lncurses -lpthread
	@gcc -c snake/src/snake.c -lncurses -lpthread
	@gcc -c main.c -lncurses -lpthread
#	@gcc **/*.o
