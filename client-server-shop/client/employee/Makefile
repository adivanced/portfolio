employee: ../client.o ../graph.o ../net.o button_callback.o
	cc ../client.o ../graph.o ../net.o button_callback.o -o employee -lGL -lGLU -lglut

client.o: ../client.c
	cc ../client.c -c 

graph.o: ../graph.c ../graph.h
	cc ../graph.c -c -lGL -lGLU -lglut

net.o: ../net.c ../net.h
	cc ../net.c -c

button_callback.o: button_callback.c
	cc button_callback.c -c
