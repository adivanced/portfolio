#include "callback.c"
#include "callback_empl.c"

char buf[4096];



extern uint64_t (*callbacks[30])(void*, uint64_t, PGconn*, int); 
extern uint64_t (*callbacks_empl[30])(void*, uint64_t, PGconn*, int); 




void server_pipeline(int clientsock_fd, struct sockaddr_in client_addr, PGconn* db_conn, uint64_t(*call[30])(void*, uint64_t, PGconn*, int), void* wins[20]){
	uint64_t win_id = 0;
	uint64_t retb;

	printf("Enter pipeline\n");

	for(;;){
		if(win_id != -1){ // -1 means previous callback sent dynamic window
			retb = send(clientsock_fd, (wins[win_id])+8, *((uint64_t*)wins[win_id])-8, 0);
		}
			//printf("%ld %ld\n", retb, *((uint64_t*)windows[win_id])-8);

		retb = 0;

		printf("Sent window %ld, wait for callback\n", win_id);

		//while(!retb || retb == -1){
			retb = recv(clientsock_fd, buf, 4096, 0);
			printf("GOT %ld BYTES\n", retb);
		//}

		uint64_t i_callback = *((uint64_t*)buf);

		printf("ICALLBACK: %ld\n", i_callback);

		win_id = call[i_callback](buf+8, retb-8, db_conn, clientsock_fd);
	}


}



void handle_client(int clientsock_fd, struct sockaddr_in client_addr){

	//const void* reg_lo_ptr= (void*)window_reg_log+8;

	//printf("%ld\n", *((uint64_t*)(reg_lo_ptr-8)));

	printf("Got Client connection: %d %s\n", clientsock_fd, inet_ntoa(client_addr.sin_addr));

	//PGconn* db_conn = PQconnectdb("host=pg dbname=studs user=s333219 password=Cno8u70h2A6zPvXn");
	PGconn* db_conn = PQsetdbLogin("pg", NULL, NULL, NULL, "studs", "s333219", "Cno8u70h2A6zPvXn");

	if(PQstatus(db_conn) == CONNECTION_BAD){
		printf("Could not connect to DB\n");
		printf("Disconnecting...\n");
		exit(0);

	}else if(PQstatus(db_conn) == CONNECTION_OK){
		printf("Successfully connected to DB\n");
	}



	//uint64_t retb;

	//retb = send(clientsock_fd, reg_lo_ptr, *((uint64_t*)(reg_lo_ptr-8)), 0);
	// printf("retb: %ld", retb);
	server_pipeline(clientsock_fd, client_addr, db_conn, callbacks, windows);

	//recv(clientsock_fd, buf, 10, 0);
}

void handle_employee(int clientsock_fd, struct sockaddr_in client_addr){
	printf("Got Employee connection: %d %s\n", clientsock_fd, inet_ntoa(client_addr.sin_addr));



	PGconn* db_conn = PQsetdbLogin("pg", NULL, NULL, NULL, "studs", "s333219", "Cno8u70h2A6zPvXn");

	if(PQstatus(db_conn) == CONNECTION_BAD){
		printf("Could not connect to DB\n");
		printf("Disconnecting...\n");
		exit(0);

	}else if(PQstatus(db_conn) == CONNECTION_OK){
		printf("Successfully connected to DB\n");
	}

	//uint64_t retb;

	//retb = send(clientsock_fd, reg_lo_ptr, *((uint64_t*)(reg_lo_ptr-8)), 0);
	// printf("retb: %ld", retb);
	server_pipeline(clientsock_fd, client_addr, db_conn, callbacks_empl, windows_empl);
}