main: main.c
	@gcc main.c -lncurses -lpthread
	@./a.out
	@make clean

clean: a.out *.o
	@rm *.o
	@rm a.out