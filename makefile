run:
	gcc main.c -lm -o main
	./main
	rm main
debug:
	gcc -g main.c -lm -o main
	gdb ./main
	rm main