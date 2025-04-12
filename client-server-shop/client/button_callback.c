#include <unistd.h>
#include "net.h"
#include "graph.h"

extern int sockfd;

extern char buf[4096];

extern uint64_t got_pending_win;

extern uint64_t window_updated;

extern struct window* window;

uint64_t app_type = 0;

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

void callback_1(){ // register
	uint64_t i = 1;
	send(sockfd, &i, 8, 0);
	//free_win();
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_2(){ // log in
	uint64_t i = 2;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_3(){ // apply button in register
	*((uint64_t*)buf) = 3; // callback id

	uint64_t len = gather_all_input(((void*)buf)+8);
	if(len == -1){
		return;
	}else{
		send(sockfd, buf, len+8, 0);
		recv(sockfd, buf, 4096, 0);
		got_pending_win = 1;
	}
}

void callback_4(){ // apply button in login

	*((uint64_t*)buf) = 4;	

	uint64_t len = gather_all_input(((void*)buf)+8);
	if(len == -1){
		return;
	}else{
		send(sockfd, buf, len+8, 0);
		recv(sockfd, buf, 4096, 0);
		got_pending_win = 1;
	}	

}

void callback_5(){
	uint64_t i = 5;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_6(){
	uint64_t i = 6;
	send(sockfd, &i, 8, 0);
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
	uint64_t i = 9;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}

void callback_10(){
	uint64_t i = 10;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_11(){
	*((uint64_t*)buf) = 11;	

	uint64_t len = gather_all_input(((void*)buf)+8);
	if(len == -1){
		return;
	}else{
		send(sockfd, buf, len+8, 0);
		recv(sockfd, buf, 4096, 0);
		got_pending_win = 1;
	}	
}

void callback_12(){ // beer button in catalogue
	*((uint64_t*)buf) = 12;
	for(uint64_t i = 0; i < window->n_buttons; i++){
		if(window->buttons[i].selected){
			uint64_t len = strlen(window->buttons[i].text);
			memcpy(buf+8, window->buttons[i].text, len+1);
			send(sockfd, buf, 8+len+1, 0);
			recv(sockfd, buf, 4096, 0);
			got_pending_win = 1;
			return;
		}
	}
}

void callback_13(){ // prev button in catalogue
	uint64_t i = 13;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}

void callback_14(){ // next button in catalogue
	uint64_t i = 14;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_15(){
	*((uint64_t*)buf) = 15;	

	void* ptr = buf+8;

	//printf("%s\n", window->texts[0].text);

	memcpy(ptr, window->texts[window->n_text-1].text, strlen(window->texts[window->n_text-1].text)+1);

	ptr += strlen(window->texts[window->n_text-1].text)+1;

	//printf("!\n");

	uint64_t len = gather_all_input(ptr);
	if(len == -1){
		return;
	}else{
		send(sockfd, buf, len+8+strlen(window->texts[window->n_text-1].text)+1, 0);
		recv(sockfd, buf, 4096, 0);
		got_pending_win = 1;
	}		
}

void callback_16(){ // confirm button in cart
	uint64_t n_order = 0;
	for(uint64_t i = 2; i < window->n_buttons; i++){
		if(window->buttons[i].button_return.type == 19 || window->buttons[i].button_return.type == 18){
			return;
		}
		if(window->buttons[i].button_return.type == 17){n_order++;}
		if(window->buttons[i].button_return.type == 16 && window->buttons[i].selected == 1){break;}
	}
	//n_order++;

	*((uint64_t*)buf) = 16;

	sprintf(buf+8, "%ld", n_order);
	send(sockfd, buf, 8+strlen(buf+8)+1, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;
}

void callback_17(){
	uint64_t n_order = 0;
	for(uint64_t i = 2; i < window->n_buttons; i++){
		if(window->buttons[i].button_return.type == 19 || window->buttons[i].button_return.type == 18){
			return;
		}
		if(window->buttons[i].button_return.type == 17 && window->buttons[i].selected == 1){
			break;
		}else if(window->buttons[i].button_return.type == 17){n_order++;}
		
	}
	//n_order++;

	*((uint64_t*)buf) = 17;

	sprintf(buf+8, "%ld", n_order);
	send(sockfd, buf, 8+strlen(buf+8)+1, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}

void callback_18(){
	uint64_t i = 18;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}

void callback_19(){
	uint64_t i = 19;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}

void callback_20(){
	uint64_t i = 20;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}

void callback_21(){
	uint64_t i = 21;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}

void callback_22(){
	*((uint64_t*)buf) = 22; // callback id

	uint64_t len = gather_all_input(((void*)buf)+8);
	if(len == -1){
		return;
	}else{
		send(sockfd, buf, len+8, 0);
		recv(sockfd, buf, 4096, 0);
		got_pending_win = 1;
	}	
}

void callback_23(){
	uint64_t i = 23;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}

void callback_24(){
	uint64_t i = 24;
	send(sockfd, &i, 8, 0);
	recv(sockfd, buf, 4096, 0);
	got_pending_win = 1;	
}



void (*callbacks[30])() = {callback_0, callback_1, callback_2, callback_3, callback_4, callback_5, callback_6, callback_7, callback_8, callback_9, callback_10, callback_11, callback_12, callback_13, callback_14, callback_15, callback_16, callback_17, callback_18, callback_19, callback_20, callback_21, callback_22, callback_23, callback_24};