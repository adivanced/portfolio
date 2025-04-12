#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <libpq-fe.h>


#include "incbin.h"
#include "handler.h"

#define SERVER_PORT 35103
#define SERVER_ADDR "0.0.0.0"//"77.234.196.4"



int main(int argc, char** argv){

	printf("%ld\n", *((uint64_t*)(window_reg_log)));
	struct sockaddr_in server_addr = (struct sockaddr_in){sin_family:AF_INET, sin_port:htons(SERVER_PORT), sin_addr:inet_addr(SERVER_ADDR)};

	struct sockaddr_in client_addr;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	printf("%d\n", sockfd);

	//for(uint16_t i = 1024; i < 1025; i++){
		server_addr.sin_port=htons(SERVER_PORT);
		int rval = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
		printf("%d %d\n", rval, SERVER_PORT);	
	//	if(!rval){break;}
	//}

	if(listen(sockfd, 10) < 0){
		printf("Error while listening\n");
	}

	int client_sock;
	int client_sock_len;

	try_again:
	printf("check connections...\n");

	client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &client_sock_len);
	if(client_sock > 0){
		if(!(fork())){
			//printf("forked\n");
			uint64_t app_type;
			recv(client_sock, &app_type, 8, 0);
			printf("APP TYPE %ld\n", app_type);
			if(!app_type){
				handle_client(client_sock, client_addr);
			}else if(app_type == 0xFFFFFFFFFFFFFFFF){
				handle_employee(client_sock, client_addr);
			}
		}else{
			//printf("Parent\n");
			goto try_again;
		}
	}else{
		//usleep(1000);
		goto try_again;
	}


	return 0;
}