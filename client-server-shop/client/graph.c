#include "graph.h"
//#include "button_callback.c"

uint64_t text_field_selected = 0;
uint64_t i_text_field_selected = 0;
uint64_t window_updated = 1;
uint64_t got_pending_win = 0;


extern void conv_win(void* buf, uint64_t len);
extern char buf[4096];
extern void (*callbacks[30])();

struct window* window;

void* fonts[7] = {GLUT_BITMAP_8_BY_13, GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10, GLUT_BITMAP_TIMES_ROMAN_24, GLUT_BITMAP_HELVETICA_10, GLUT_BITMAP_HELVETICA_12, GLUT_BITMAP_HELVETICA_18};


void free_win(){
	free(window->boxes);

	for(uint64_t i = 0; i < window->n_text_fields; i++){
		free(window->text_fields[i].text);
	}
	free(window->text_fields);

	for(uint64_t i = 0; i < window->n_buttons; i++){
		free(window->buttons[i].text);
	}
	free(window->buttons);

	for(uint64_t i = 0; i < window->n_text; i++){
		free(window->texts[i].text);
	}
	free(window->texts);

	free(window);
}



void plot_box(struct box* box){
	glColor3f(box->color.r, box->color.g, box->color.b);
	glBegin(GL_QUADS);
	glVertex2f(box->coord.x, box->coord.y);
	glVertex2f(box->coord.x+box->length,  box->coord.y);
	glVertex2f(box->coord.x+box->length,  box->coord.y-box->heigth);
	glVertex2f(box->coord.x,  box->coord.y-box->heigth);
	glEnd();
}

void plot_text(struct text* text){
	//printf("Printing text: %s\n", text->text);
	glColor3f(text->color.r, text->color.g, text->color.b);
	glRasterPos2f(text->coord.x, text->coord.y);
	glutBitmapString(text->font, text->text);
}

void plot_button(struct button* button){
	glColor3f(button->color.r, button->color.g, button->color.b);
	glBegin(GL_QUADS);
	glVertex2f(button->coord.x, button->coord.y);
	glVertex2f(button->coord.x+button->length,  button->coord.y);
	glVertex2f(button->coord.x+button->length,  button->coord.y-button->heigth);
	glVertex2f(button->coord.x,  button->coord.y-button->heigth);
	glEnd();	
	if(button->selected){
		glColor3f(button->select_color.r, button->select_color.g, button->select_color.b);
		glBegin(GL_LINE_LOOP);
		glVertex2f(button->coord.x, button->coord.y);
		glVertex2f(button->coord.x+button->length,  button->coord.y);
		glVertex2f(button->coord.x+button->length,  button->coord.y-button->heigth);
		glVertex2f(button->coord.x, button->coord.y-button->heigth);		
		glEnd();
	}
	glColor3f(button->text_color.r, button->text_color.g, button->text_color.b);
	glRasterPos2f(button->coord.x+(button->length/10.0f), button->coord.y-(button->heigth/2.0f));
	glutBitmapString(button->font, button->text);
}

void plot_text_field(struct text_field* text_field){
	glColor3f(text_field->color.r, text_field->color.g, text_field->color.b);
	glBegin(GL_QUADS);
	glVertex2f(text_field->coord.x, text_field->coord.y);
	glVertex2f(text_field->coord.x+text_field->length,  text_field->coord.y);
	glVertex2f(text_field->coord.x+text_field->length,  text_field->coord.y-text_field->heigth);
	glVertex2f(text_field->coord.x,  text_field->coord.y-text_field->heigth);
	glEnd();	
	if(text_field->selected){
		glColor3f(text_field->select_color.r, text_field->select_color.g, text_field->select_color.b);
		glBegin(GL_LINE_LOOP);
		glVertex2f(text_field->coord.x, text_field->coord.y);
		glVertex2f(text_field->coord.x+text_field->length,  text_field->coord.y);
		glVertex2f(text_field->coord.x+text_field->length,  text_field->coord.y-text_field->heigth);
		glVertex2f(text_field->coord.x,  text_field->coord.y-text_field->heigth);		
		glEnd();
	}
	glColor3f(text_field->text_color.r, text_field->text_color.g, text_field->text_color.b);
	glRasterPos2f(text_field->coord.x+(text_field->length/10.0f), text_field->coord.y-(text_field->heigth/2.0f));
	glutBitmapString(text_field->font, text_field->text);	
}

void plot_window(struct window* win){
	printf("PRINTWIN: boxes:%d texts:%d buttons:%d fields:%d\n", win->n_boxes, win->n_text, win->n_buttons, win->n_text_fields);

	glClearColor(win->bckg_col.r, win->bckg_col.g, win->bckg_col.b, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT); // ??

	for(uint64_t i = 0; i < win->n_boxes; i++){
		plot_box(win->boxes+i);
	}

	for(uint64_t i = 0; i < win->n_text; i++){
		plot_text(win->texts+i);
	}

	for(uint64_t i = 0; i < win->n_buttons; i++){
		plot_button(win->buttons+i);
	}

	for(uint64_t i = 0; i < win->n_text_fields; i++){
		plot_text_field(win->text_fields+i);
	}	

	glutSwapBuffers();
}

void handle_function(){
	if(got_pending_win){
		got_pending_win = 0;
		free_win();
		conv_win(buf, 0);
	}


// char tsttxt[2] = "|"; 
// char tsttxt2[10] = "AHAHAHAHA";
// char tsttxt3[6] = "Hello";

// struct text_field test_txt_fld = (struct text_field){color:(struct color){r:0.5f, g:0.5f, b:1.0f}, coord:(struct coord){x:0.5f, y:0.5f}, text:tsttxt, text_color:(struct color){r:0.0f, g:0.0f, b:0.0f}, length:0.2f, heigth:0.1f, select_color:(struct color){r:0.0f, g:0.0f, b:0.0f}, font:GLUT_BITMAP_HELVETICA_18, selected:0};
// struct button test_btn = (struct button){color:(struct color){r:0.0f, g:0.0f, b:1.0f}, coord:(struct coord){x:-0.5f, y:-0.5f}, text:tsttxt2, text_color:(struct color){r:1.0f, g:1.0f, b:1.0f}, length:0.2f, heigth:0.1f, select_color:(struct color){r:1.0f, g:0.0f, b:0.0f}, font:GLUT_BITMAP_HELVETICA_18, selected:1, button_return:(struct button_return){type:0, n:0, data:NULL}};
// struct box test_box = (struct box){color:(struct color){r:0.0f, g:1.0f, b:1.0f}, coord:(struct coord){x:0.9f, y:0.8f}, length:0.1f, heigth:0.1f};
// struct text test_text = (struct text){text:tsttxt3, color:(struct color){r:1.0f, g:0.0f, b:1.0f}, coord:(struct coord){x:0.0f, y:0.0f}, font:GLUT_BITMAP_HELVETICA_18};

// struct window testwin = (struct window){n_text_fields:1, text_fields:&test_txt_fld, n_buttons:1, buttons:&test_btn, n_text:1, texts:&test_text, n_boxes:1, boxes:&test_box, bckg_col:(struct color){r:0.0f, g:0.0f, b:0.0f}};

// window = &testwin;

	if(window_updated){
		plot_window(window);
		window_updated = 0;
	}
}

void keyboard(unsigned char key, int x, int y){
	uint64_t len;
	//printf("KEYPRESS\n");
	if(text_field_selected){
		printf("TXT FLD keyboard callback\n");
		switch(key){
			case 8: // backspace
				len = strlen(window->text_fields[i_text_field_selected].text);
				if(len != 1){
					window->text_fields[i_text_field_selected].text[len-2] = '|';
					window->text_fields[i_text_field_selected].text[len-1] = '\0';
					window->text_fields[i_text_field_selected].text = (char*)realloc(window->text_fields[i_text_field_selected].text, len);
					window_updated = 1;
				}
				break; 
			default:
				len = strlen(window->text_fields[i_text_field_selected].text);
				window->text_fields[i_text_field_selected].text = (char*)realloc(window->text_fields[i_text_field_selected].text, len+2);
				window->text_fields[i_text_field_selected].text[len] = '|';
				window->text_fields[i_text_field_selected].text[len+1] = '\0';
				window->text_fields[i_text_field_selected].text[len-1] = ((char)key);
				window_updated = 1;
		}
	}else{

	}
}

int rectangle_check(float x, float y, float rectx, float recty, float rectl, float recth){
	//printf("Checking: mx: %f, my: %f, rx: %f, ry: %f, rx1: %f, ry1: %f\n", x, y, rectx, recty, rectx+rectl, recty+recth);
	//printf("Y:%f rectY:%f\n", y, recty);
	if(x >= rectx && y <= recty){
		//printf("OK\n");
		if(x <= rectx + rectl && y >= recty - recth){
			//printf("recty-recth:%f\n", recty-recth);
			//printf("NNNN\n");
			return 1;
		}
	}
	return 0;
}

void mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){		

		//float fx = (float)x
		int w = glutGet(GLUT_WINDOW_WIDTH);
		int h = glutGet(GLUT_WINDOW_HEIGHT);

		float fx = (float)((float)x - (float)w/2) / (w/2);
		float fy = -1.0f * ((float)((float)y - (float)h/2) / (h/2));
		//printf("X:%f Y:%f\n", fx, fy);

		//float fx = (float)(((float)x - (float)SCR_W/2) / (SCR_W/2));// - 1.0f;
		//float fy = -1.0f*((float)(((float)y - (float)SCR_H/2)/ (SCR_H/2)));// - 0.05f;// - 1.0f;
		//printf("mousu x:%d y:%d %d %d\n", x, y, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

		for(uint64_t i = 0; i < window->n_buttons; i++){
			if(rectangle_check(fx, fy, window->buttons[i].coord.x, window->buttons[i].coord.y, window->buttons[i].length, window->buttons[i].heigth)){
				window->buttons[i].selected = 1;
				window_updated = 1;
				// corresponding button callback
				// return 0; probably
				printf("!!!BUTTON_CALLBACK!!! of button %ld, callback no %d\n", i, window->buttons[i].button_return.type);
				callbacks[window->buttons[i].button_return.type]();
				return;
					
			}
		}

		for(uint64_t i = 0; i < window->n_text_fields; i++){
			if(rectangle_check(fx, fy, window->text_fields[i].coord.x, window->text_fields[i].coord.y, window->text_fields[i].length, window->text_fields[i].heigth)){
				if(text_field_selected){
					window->text_fields[i_text_field_selected].selected = 0;
				}
				text_field_selected = 1;
				i_text_field_selected = i;
				window->text_fields[i_text_field_selected].selected = 1;
				window_updated = 1;
				return;
			}
		}
	}
}

void f(int val){


	handle_function();
	glutTimerFunc(50, f, 1);
}


void ui_init(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_BORDERLESS|GLUT_CAPTIONLESS);
	glutInitWindowSize(SCR_W,SCR_H);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Window");
	glutDisplayFunc(handle_function);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutTimerFunc(50, f, 1);
	glutMainLoop();
}
