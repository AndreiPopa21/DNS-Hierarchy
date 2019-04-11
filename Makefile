build: tema2
	
run: tema2
	./tema2
clean: tema2
	rm -f tema2
tema2:
	gcc -g -Wall main.c -o tema2
push_m:
	git push origin master
