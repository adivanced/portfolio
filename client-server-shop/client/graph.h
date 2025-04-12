#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <GL/freeglut.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define SCR_W 1080
#define SCR_H 1080

struct button_return{
	uint8_t type;
	//uint64_t n;
	//void* data;
};

struct color{
	float r;
	float g;
	float b;
};

struct coord{
	float x;
	float y;
};

struct box{
	struct color color;
	struct coord coord;
	float length;
	float heigth;
};

struct button{
	struct color color;
	struct coord coord;
	char* text;	
	struct color text_color;
	float length;
	float heigth;
	struct color select_color;
	void* font;
	uint8_t selected;
	struct button_return button_return;
};

struct text{
	char* text;
	struct color color;
	struct coord coord;
	void* font;
};

struct text_field{
	struct color color;
	struct coord coord;
	char* text;
	struct color text_color;
	float length;
	float heigth;
	struct color select_color;
	void* font;
	uint8_t selected;
};


struct window{
	uint8_t n_text_fields;
	struct text_field* text_fields;

	uint8_t n_buttons;
	struct button* buttons;

	uint8_t n_text;
	struct text* texts;

	uint8_t n_boxes;
	struct box* boxes;

	struct color bckg_col;
}; // priority as follows: text box, button, text, box

void ui_init();

void plot_box(struct box* box);

void plot_button(struct button* button);

void plot_text_field(struct text_field* text_field);

void plot_window(struct window* win);

void handle_function();

void free_win();

#endif