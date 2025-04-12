#include <unistd.h>

#include "../net.h"
#include "../graph.h"

extern int sockfd;

extern char buf[4096];

extern uint64_t got_pending_win;

extern uint64_t window_updated;

extern struct window* window;

uint64_t app_type = 0xFFFFFFFFFFFFFFFF;

uint64_t gather_all_input(void* buff){
	uint64_t totallen = 0;
	for(uint64_t i = 0; i < window->n_text_fields; i++){
		uint64_t len = strlen(window->text_fields[i].text);
		if(len <= 1){return -1;}
		memcpy(buff, window->text_fields[i].text, len-1);
		buff += len;
		*((char*)buff-1) = '\0';
		totallen += len;
	}
	return totallen;
}



void callback_0(){ // exit
	uint64_t i = 0;
	send(sockfd, &i, 8, 0);
	close(sockfd);
	exit(0);
}

void callback_1(){ // apply employee registration
	*((uint64_t*)buf) = 1; // callback id

	uint64_t len = gather_all_input(((void*)buf)+8);
	if(len == -1){
		return;
	}else{
		send(sockfd, buf, len+8, 0);
		recv(sockfd, buf, 4096, 0);
		got_pending_win = 1;
	}
}

void callback_2(){ // list of orders button
	uint64_t i = 2;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_3(){ // list of users button
	uint64_t i = 3;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_4(){
	uint64_t i = 4;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_5(){
	*((uint64_t*)buf) = 5;	

	uint64_t n_order = 0;
	for(uint64_t i = 2; i < window->n_buttons; i++){
		if(window->buttons[i].button_return.type == 5 && window->buttons[i].selected == 1){
			break;
		}else if(window->buttons[i].button_return.type == 5){n_order++;}
	}

	sprintf(buf+8, "%ld", n_order);
	printf("NCLIENT: %ld\n", n_order);

	send(sockfd, buf, 8+strlen(buf+8), 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_6(){
	printf("CALB 6\n");

	*((uint64_t*)buf) = 6;	

	uint64_t n_order = 0;
	for(uint64_t i = 2; i < window->n_buttons; i++){
		if(window->buttons[i].button_return.type == 6 && window->buttons[i].selected == 1){
			break;
		}else if(window->buttons[i].button_return.type == 5){n_order++;}
	}
	n_order--;

	printf("NORDER: %ld\n", n_order);

	char* ptr = buf+8;
	char* tosend = window->texts[n_order*2].text+7;
	char* carette = tosend;
	for(; *carette != ' '; carette++){;}
	*carette = '\0';

	memcpy(ptr, tosend, strlen(tosend)+1);
	printf("UNM: %s\n", ptr);
	ptr += strlen(tosend)+1;
	tosend += strlen(tosend)+1+9;
	memcpy(ptr, tosend, strlen(tosend)+1);
	printf("PWD: %s\n", ptr);
	ptr += strlen(tosend)+1;



	send(sockfd, buf, ptr-buf, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_7(){
	uint64_t i = 7;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_8(){
	uint64_t i = 8;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_9(){
	*((uint64_t*)buf) = 9;	

	uint64_t n_order = 0;
	for(uint64_t i = 2; i < window->n_buttons; i++){
		if(window->buttons[i].button_return.type == 9 && window->buttons[i].selected == 1){
			break;
		}else if(window->buttons[i].button_return.type == 10){n_order++;}
	}

	n_order--;

	sprintf(buf+8, "%ld", n_order);
	printf("NORDER: %ld\n", n_order);

	send(sockfd, buf, 8+strlen(buf+8), 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}


void callback_10(){
	*((uint64_t*)buf) = 10;	

	uint64_t n_order = 0;
	for(uint64_t i = 2; i < window->n_buttons; i++){
		if(window->buttons[i].button_return.type == 10 && window->buttons[i].selected == 1){
			break;
		}else if(window->buttons[i].button_return.type == 10){n_order++;}
	}

	sprintf(buf+8, "%ld", n_order);
	printf("NORDER: %ld\n", n_order);

	send(sockfd, buf, 8+strlen(buf+8), 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_11(){
	uint64_t i = 11;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_12(){
	uint64_t i = 12;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_13(){
	*((uint64_t*)buf) = 13; // callback id

	uint64_t len = gather_all_input(((void*)buf)+8);
	if(len == -1){
		return;
	}else{
		send(sockfd, buf, len+8, 0);
		recv(sockfd, buf, 4096, 0);
		got_pending_win = 1;
	}
}

void callback_14(){
	uint64_t i = 14;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}

void callback_15(){
	uint64_t i = 15;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}


void (*callbacks[30])() = {callback_0, callback_1, callback_2, callback_3, callback_4, callback_5, callback_6, callback_7, callback_8, callback_9, callback_10, callback_11, callback_12, callback_13, callback_14, callback_15};