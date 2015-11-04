all:
	gcc -pthread -o main main.c
	chmod 755 main
clean:
	rm main
