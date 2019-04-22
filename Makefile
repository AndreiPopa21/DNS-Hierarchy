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

files: tree.in tree.out queries.in queries.out hierarchy.out users.in

tree.in:
	touch tree.in
	cat tree_in_input > tree.in
tree.out:
	touch tree.out
hierarchy.out:
	touch hierarchy.out
users.in:
	touch users.in
	cat users_in_input > users.in
queries.in:
	touch queries.in
queries.out:
	touch queries.out
