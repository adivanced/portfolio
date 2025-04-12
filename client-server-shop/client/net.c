#include "net.h"
#include "graph.h"



extern int sockfd;
extern struct window* window;
extern void* fonts[7];

extern uint64_t app_type;

char buf[4096];


void init_server_connection(){
	struct sockaddr_in server_addr = (struct sockaddr_in){sin_family:AF_INET, sin_port:htons(SERVER_PORT), sin_addr:inet_addr(SERVER_ADDR)};
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr))){
		printf("Unable to connect\n");
		exit(0);
	}else{
		printf("Connected!\n");
	}

	send(sockfd, &app_type, 8, 0);

	uint64_t retb = recv(sockfd, buf, 4096, 0);
	printf("retb: %ld\n", retb);


	conv_win(buf, retb);
	//printf("!\n");
}


void conv_win(void* packet, uint64_t len){
	//free_win();

	struct window_packet_header* hdr = packet;

	window = (struct window*)malloc(sizeof(struct window));

	window->n_text_fields = hdr->n_text_fields;
	window->n_buttons = hdr->n_buttons;
	window->n_boxes = hdr->n_boxes;
	window->n_text = hdr->n_text;
	window->bckg_col.r = hdr->r;
	window->bckg_col.g = hdr->g;
	window->bckg_col.b = hdr->b;
	window->text_fields = (struct text_field*)malloc(sizeof(struct text_field) * hdr->n_text_fields);
	window->buttons = (struct button*)malloc(sizeof(struct button) * hdr->n_buttons);
	window->boxes = (struct box*)malloc(sizeof(struct box) * hdr->n_boxes);
	window->texts = (struct text*)malloc(sizeof(struct text) * hdr->n_text);

	printf("Parsing window: %d text fields, %d buttons, %d texts, %d boxes\n", window->n_text_fields, window->n_buttons, window->n_text, window->n_boxes);

	packet += sizeof(struct window_packet_header);

	for(uint64_t i = 0; i < hdr->n_text_fields; i++){
		struct window_packet_text_field* tf = packet;
		window->text_fields[i].color.r = tf->r;
		window->text_fields[i].color.g = tf->g;
		window->text_fields[i].color.b = tf->b;
		window->text_fields[i].coord.x = tf->x;
		window->text_fields[i].coord.y = tf->y;
		window->text_fields[i].text = (char*)calloc(2, 1);
		window->text_fields[i].text[0] = '|';
		window->text_fields[i].text_color.r = tf->tr;
		window->text_fields[i].text_color.g = tf->tg;
		window->text_fields[i].text_color.b = tf->tb;
		window->text_fields[i].length = tf->length;
		window->text_fields[i].heigth = tf->heigth;
		window->text_fields[i].select_color.r = tf->sr;
		window->text_fields[i].select_color.g = tf->sg;
		window->text_fields[i].select_color.b = tf->sb;
		window->text_fields[i].font = fonts[tf->font_ptr];
		window->text_fields[i].selected = 0;
		packet += sizeof(struct window_packet_text_field);
	}

	char* button_text = packet + hdr->n_buttons * sizeof(struct window_packet_button);

	for(uint64_t i = 0; i < hdr->n_buttons; i++){
		struct window_packet_button* bt = packet;
		window->buttons[i].color.r = bt->r;
		window->buttons[i].color.g = bt->g;
		window->buttons[i].color.b = bt->b;
		window->buttons[i].coord.x = bt->x;
		window->buttons[i].coord.y = bt->y;
		printf("%s\n", button_text);
		uint64_t len = strlen(button_text);
		window->buttons[i].text = (char*)calloc(len+1, 1);
		strcpy(window->buttons[i].text, button_text);
		button_text += len + 1;
		window->buttons[i].text_color.r = bt->tr;
		window->buttons[i].text_color.g = bt->tg;
		window->buttons[i].text_color.b = bt->tb;
		window->buttons[i].length = bt->length;
		window->buttons[i].heigth = bt->heigth;
		window->buttons[i].select_color.r = bt->sr;
		window->buttons[i].select_color.g = bt->sg;
		window->buttons[i].select_color.b = bt->sb;
		window->buttons[i].font = fonts[bt->font_ptr];
		window->buttons[i].selected = 0;
		window->buttons[i].button_return.type = bt->button_r_type;
		//window->buttons[i].button_return.n = bt->button_n_max_r;
		packet += sizeof(struct window_packet_button);
	}

	packet = button_text;

	for(uint64_t i = 0; i < hdr->n_boxes; i++){
		struct window_packet_box* bp = packet;
		window->boxes[i].color.r = bp->r;
		window->boxes[i].color.g = bp->g;
		window->boxes[i].color.b = bp->b;
		window->boxes[i].coord.x = bp->x;
		window->boxes[i].coord.y = bp->y;
		window->boxes[i].length = bp->length;
		window->boxes[i].heigth = bp->heigth;
		packet += sizeof(struct window_packet_box);
	}


	char* text_text = packet + hdr->n_text * sizeof(struct window_packet_text);

	for(uint64_t i = 0; i < hdr->n_text; i++){
		struct window_packet_text* pt = packet;
		window->texts[i].color.r = pt->r;
		window->texts[i].color.g = pt->g;
		window->texts[i].color.b = pt->b;
		window->texts[i].coord.x = pt->x;
		window->texts[i].coord.y = pt->y;
		window->texts[i].font = fonts[pt->font_ptr];
		uint64_t len = strlen(text_text);
		//printf("STRLEN:%ld str:%s\n", len, text_text);
		window->texts[i].text = (char*)calloc(len+1, 1);
		strcpy(window->texts[i].text, text_text);
		text_text += len+1;
		packet += sizeof(struct window_packet_text);
	}

}











