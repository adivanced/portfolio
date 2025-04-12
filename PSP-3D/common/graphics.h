#ifndef COMMON_GRAPHICS_H
#define COMMON_GRAPHICS_H

#include <stdint.h>
#include <pspge.h>
#include <pspdisplay.h>
#include <psputils.h>

#ifdef __cplusplus
namespace disp{
#endif

	extern uint32_t* draw_buffer;
	extern uint32_t* disp_buffer;
	extern float* __restrict__ z_buffer;

	void init();
	void init_depth_buffer();
	inline void pixel(int x, int y, uint32_t color){
		draw_buffer[x + (y*512)] = color;
	}
	inline void clear_z_buffer(){
		//STRAIGHT LOOP:
		//for(int i = 0; i < 512*272; i++){
		//	z_buffer[i] = 1000000.0f;
		//}
		//DUFF DEVICE UNROLLED LOOP:
		int n = ((512*272 + 15)>>4);
		int i = 0;
		switch(n%16){
			case 0: do{ z_buffer[i] = 1000000.0f;
						i++;
					case 15: z_buffer[i] = 1000000.0f;
							i++;
					case 14: z_buffer[i] = 1000000.0f;
							i++;
					case 13: z_buffer[i] = 1000000.0f;
							i++;
					case 12: z_buffer[i] = 1000000.0f;
							i++;
					case 11: z_buffer[i] = 1000000.0f;
							i++;
					case 10: z_buffer[i] = 1000000.0f;
							i++;
					case 9: z_buffer[i] = 1000000.0f;
							i++;
					case 8: z_buffer[i] = 1000000.0f;
							i++;

					case 7: z_buffer[i] = 1000000.0f;
							i++;
					case 6: z_buffer[i] = 1000000.0f;
							i++;
					case 5: z_buffer[i] = 1000000.0f;
							i++;
					case 4: z_buffer[i] = 1000000.0f;
							i++;
					case 3: z_buffer[i] = 1000000.0f;
							i++;
					case 2: z_buffer[i] = 1000000.0f;
							i++;
					case 1: z_buffer[i] = 1000000.0f;
							i++;
			}while(--n > 0);
		}
	}
	inline void clear(uint32_t color){
			//STRAIGHT LOOP:
		//for(int i = 0; i < 512 * 272; i++){
		//	draw_buffer[i] = color;
		//}

		//DUFF DEVICE UNROLLED LOOP
		int n = ((512*272 + 15)>>4);
		int i = 0;
		switch(n%16){
			case 0: do{ draw_buffer[i] = color;
						i++;
		
					case 15: draw_buffer[i] = color;
							i++;
					case 14: draw_buffer[i] = color;
							i++;
					case 13: draw_buffer[i] = color;
							i++;
					case 12: draw_buffer[i] = color;
							i++;
					case 11: draw_buffer[i] = color;
							i++;
					case 10: draw_buffer[i] = color;
							i++;
					case 9: draw_buffer[i] = color;
							i++;
					case 8: draw_buffer[i] = color;
							i++;

					case 7: draw_buffer[i] = color;
							i++;
					case 6: draw_buffer[i] = color;
							i++;
					case 5: draw_buffer[i] = color;
							i++;
					case 4: draw_buffer[i] = color;
							i++;
					case 3: draw_buffer[i] = color;
							i++;
					case 2: draw_buffer[i] = color;
							i++;
					case 1: draw_buffer[i] = color;
							i++;
			}while(--n > 0);
		}
	}
	inline void swap_buffers(){
		uint32_t* temp = disp_buffer;
		disp_buffer = draw_buffer;
		draw_buffer = temp;

		sceKernelDcacheWritebackInvalidateAll();
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_IMMEDIATE);
	}
	//inline void pixel_depth(int, int, uint32_t, float);
	inline void pixel_depth(int x, int y, uint32_t color, float depth){

		if(depth < z_buffer[(y * 512) + x]){
			z_buffer[(y * 512) + x] = depth;
			draw_buffer[x + (y*512)] = color;
		}
	} 
	void bresenham(int, int, int, int, uint32_t);
	//inline void bresenham_depth(int, int, int, int, uint32_t, float);
	inline void bresenham_depth(int x1, int y1, int x2, int y2, uint32_t color, float depth){
		/*if(x1 >= 480){
			x1 = 479;
		}else if(x1 < 0){
			x1 = 0;
		}
		if(x2 >= 480){
			x2 = 479;
		}else if(x2 < 0){
			x2 = 0;
		}
		if(y1 >= 272){
			y1 = 271;
		}else if(y1 < 0){
			y1 = 0;
		}
		if(y2 >= 272){
			y2 = 271;
		}else if(y2 < 0){
			y2 = 0;
		}*/
		int m_new = 2 * (y2 - y1);
		int slope_error_new = m_new - (x2 - x1);
   		for (int x = x1, y = y1; x <= x2; x++){
      		//cout << "(" << x << "," << y << ")\n";
      		pixel_depth(x, y, color, depth);
 
      		// Add slope to increment angle formed
      		slope_error_new += m_new;
 
      		// Slope error reached limit, time to
      		// increment y and update slope error.
      	
			if (slope_error_new >= 0){
         		y++;
         		slope_error_new  -= 2 * (x2 - x1);
      		}
   		}
	}

	inline void draw_line_depth(int x1, int y1, int x2, int y2, uint32_t base_color, float depth){
		unsigned short error_adj, error_acc;
   		unsigned short error_acc_temp;
    	short delta_x, delta_y, temp, x_dir;

    	/*if(x1 >= 480){
    		x1 = 479;
    	}else if(x1 < 0){
    		x1 = 0;
    	}
    	if(x2 >= 480){
    		x2 = 479;
    	}else if(x2 < 0){
    		x2 = 0;
    	}
    	if(y1 >= 272){
    		y1 = 271;
    	}else if(y1 < 0){
    		y1 = 0;
    	}
    	if(y2 >=272){
    		y2 = 271;
    	}else if(y2 < 0){
    		y2 = 0;
    	}*/

   		if (y1 > y2) {
      		temp = y1; y1 = y2; y2 = temp;
      		temp = x1; x1 = x2; x2 = temp;
   		}
   
   		pixel_depth(x1, y1, base_color, depth);

   		if ((delta_x = x2 - x1) >= 0) {
      		x_dir = 1;
   		} else {
      		x_dir = -1;
      		delta_x = -delta_x; 
   		}
   
   		if ((delta_y = y2 - y1) == 0) {
      
      		while (delta_x-- != 0) {
         		x1 += x_dir;
         		pixel_depth(x1, y1, base_color, depth);
      		}
      		return;
   		}
   		if (delta_x == 0) {
      
      		do {
         		y1++;
         		pixel_depth(x1, y1, base_color, depth);
      		} while (--delta_y != 0);
      		return;
   		}
   		if (delta_x == delta_y) {
      
      		do {
         		x1 += x_dir;
         		y1++;
         		pixel_depth(x1, y1, base_color, depth);
      		} while (--delta_y != 0);
      			return;
   		}
   
   		error_acc = 0;  
   
   		//intensity_shift = 32 - intensity_bits;
   
   		//weighting_complement_mask = num_levels - 1;
   
   		if (delta_y > delta_x) {
      
      		error_adj = ((unsigned long) delta_x << 16) / (unsigned long) delta_y;
      
      		while (--delta_y) {
         		error_acc_temp = error_acc;   
         		error_acc += error_adj;      
         		if (error_acc <= error_acc_temp) {
           
            		x1 += x_dir;
         		}
         		y1++; 
         
         		//weighting = error_acc >> intensity_shift;
         		pixel(x1, y1, base_color);
         		//pixel(x1 + x_dir, y1, base_color + (weighting ^ weighting_complement_mask));
      		}
      
      		//pixel(x2, y2, base_color);
      		return;
   		}
   
   		error_adj = ((unsigned long) delta_y << 16) / (unsigned long) delta_x;
   
   		while (--delta_x) {
      		error_acc_temp = error_acc;  
      		error_acc += error_adj;     
      		if (error_acc <= error_acc_temp) {
         
		         y1++;
      		}
      		x1 += x_dir; 
      		pixel(x1, y1, base_color);    
   		}
	}


	/*void draw_line (int x1, int y1, int x2, int y2, uint32_t base_color){
    	unsigned short error_adj, error_acc;
   		unsigned short error_acc_temp;
    	short delta_x, delta_y, temp, x_dir;

   		if (y1 > y2) {
      		temp = y1; y1 = y2; y2 = temp;
      		temp = x1; x1 = x2; x2 = temp;
   		}
   
   		pixel(x1, y1, base_color);

   		if ((delta_x = x2 - x1) >= 0) {
      		x_dir = 1;
   		} else {
      		x_dir = -1;
      		delta_x = -delta_x; 
   		}
   
   		if ((delta_y = y2 - y1) == 0) {
      
      		while (delta_x-- != 0) {
         		x1 += x_dir;
         		pixel(x1, y1, base_color);
      		}
      		return;
   		}
   		if (delta_x == 0) {
      
      		do {
         		y1++;
         		pixel(x1, y1, base_color);
      		} while (--delta_y != 0);
      		return;
   		}
   		if (delta_x == delta_y) {
      
      		do {
         		x1 += x_dir;
         		y1++;
         		pixel(x1, y1, base_color);
      		} while (--delta_y != 0);
      			return;
   		}
   
   		error_acc = 0;  
   
   		//intensity_shift = 32 - intensity_bits;
   
   		//weighting_complement_mask = num_levels - 1;
   
   		if (delta_y > delta_x) {
      
      		error_adj = ((unsigned long) delta_x << 16) / (unsigned long) delta_y;
      
      		while (--delta_y) {
         		error_acc_temp = error_acc;   
         		error_acc += error_adj;      
         		if (error_acc <= error_acc_temp) {
           
            		x1 += x_dir;
         		}
         		y1++; 
         
         		//weighting = error_acc >> intensity_shift;
         		pixel(x1, y1, base_color);
         		//pixel(x1 + x_dir, y1, base_color + (weighting ^ weighting_complement_mask));
      		}
      
      		//pixel(x2, y2, base_color);
      		return;
   		}
   
   		error_adj = ((unsigned long) delta_y << 16) / (unsigned long) delta_x;
   
   		while (--delta_x) {
      		error_acc_temp = error_acc;  
      		error_acc += error_adj;     
      		if (error_acc <= error_acc_temp) {
         
		         y1++;
      		}
      		x1 += x_dir; 
      		pixel(x1, y1, base_color);    
   		}
	}*/

	void fill_triangle(int, int, int, int, uint32_t);
	//void draw_line_smp(int, int, int, int, uint32_t);

#ifdef __cplusplus
};
#endif

#endif