client: client.o graph.o net.o button_callback.o
	cc client.o graph.o net.o button_callback.o -o client -lGL -lGLU -lglut

client.o: client.c
	cc client.c -c 

graph.o: graph.c graph.h
	cc graph.c -c -lGL -lGLU -lglut

net.o: net.c net.h
	cc net.c -c

button_callback.o: button_callback.c
	cc button_callback.c -c


# ssh -L 35103:127.0.0.1:35103 -N bobysh
