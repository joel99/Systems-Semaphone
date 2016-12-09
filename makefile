compile: sem.c client.c
	gcc -o sem sem.c
	gcc -o client client.c

clean:
	rm *~
