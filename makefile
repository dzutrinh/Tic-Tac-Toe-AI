all: c3

c3: c3.c
	gcc --std=c99 c3.c -o c3

clean:
	rm c3

