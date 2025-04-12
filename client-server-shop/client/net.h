#ifndef __NET_H__
#define __NET_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define SERVER_PORT 35103
#define SERVER_ADDR "127.0.0.1"//"77.234.196.4"

struct window_packet_header{
	uint8_t n_text_fields;
	uint8_t n_buttons;
	uint8_t n_text;
	uint8_t n_boxes;
	float r;
	float g;
	float b;
}__attribute__((packed));

struct window_packet_text_field{
	float r;
	float g;
	float b;
	float x;
	float y;
	float tr;
	float tg;
	float tb;
	float length;
	float heigth;
	float sr;
	float sg;
	float sb;
	uint8_t font_ptr;
}__attribute__((packed));

struct window_packet_button{
	float r;
	float g;
	float b;
	float x;
	float y;
	float tr;
	float tg;
	float tb;
	float length;
	float heigth;
	float sr;
	float sg;
	float sb;
	uint8_t font_ptr;
	uint8_t button_r_type;
	//uint64_t button_n_max_r;
}__attribute__((packed));

struct window_packet_box{
	float r;
	float g;
	float b;
	float x;
	float y;
	float length;
	float heigth;
}__attribute__((packed));

struct window_packet_text{
	float r;
	float g;
	float b;
	float x;
	float y;
	uint8_t font_ptr;
}__attribute__((packed));



void init_server_connection();

void conv_win(void* packet, uint64_t len);

#endif


/*
Window packet structure (max size - 2048 bytes):

window_packet_header
n_text_fields * window_packet_text_field
n_buttons * window_packet_button
... null term strings for each button
n_boxes * window_packet_box
n_texts * window_packet_text
... null term strings for each text
*/
