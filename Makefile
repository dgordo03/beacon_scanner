all:
	gcc -o run/scan -lm src/main.c

clean:
	$(RM) run/scan
