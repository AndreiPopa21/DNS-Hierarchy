build: main.c utils.c structs.c tasks.c
	gcc -g -Wall main.c utils.c structs.c tasks.c -o tema2
	
run: tema2
	./tema2
clean: tema2
	rm -f tema2
tema2:
	gcc -g -Wall main.c utils.c structs.c tasks.c -o tema2
push_m:
	git push origin new_struct

leaks: tema2
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./tema2
