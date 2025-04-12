#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "graph.h"
#include "net.h"



int sockfd;


int main(int argc, char** argv){

	//printf("%ld\n", GLUT_BITMAP_HELVETICA_18);
	init_server_connection();

	

	ui_init(argc, argv);

	
	//server_addr.sin_addr = inet_addr(SERVER_ADDR);

	

	//for(;;){;}


	return 0;
}