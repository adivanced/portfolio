#include "graphics.h"
#include <stdlib.h>

#ifdef __cplusplus
namespace disp{
#endif

	uint32_t* draw_buffer;
	uint32_t* disp_buffer;
	float* __restrict__ z_buffer;

	void init(){
		draw_buffer = (uint32_t*)(sceGeEdramGetAddr());
		disp_buffer = (uint32_t*)(sceGeEdramGetAddr() + (272 * 512 * 4));



		sceDisplaySetMode(0, 480, 272);
		sceDisplaySetFrameBuf(disp_buffer, 512, PSP_DISPLAY_PIXEL_FORMAT_8888, 1);
	}

	void init_depth_buffer(){
		#ifdef __cplusplus
		z_buffer = new float[272 * 512];
		#endif

		#ifndef __cplusplus
		z_buffer = (float*)malloc(272*512*4);
		#endif

		clear_z_buffer();
	}
	

	/*inline void pixel_depth(int x, int y, uint32_t color, float depth){
		if(depth < z_buffer[(y * 512) + x]){
			z_buffer[(y * 512) + x] = depth;
			draw_buffer[x + (y*512)] = color;
		}
	} */

	void bresenham(int x1, int y1, int x2, int y2, uint32_t color){
		int tmp;
		if(y1 > y2){
			tmp = y1;
			y1 = y2;
			y2 = tmp;
		}
		if(x1 > x2){
			tmp = x1;
			x1 = x2;
			x2 = tmp;
		}
		if(x1 >= 480){
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
		}
		int m_new = 2 * (y2 - y1);
		int slope_error_new = m_new - (x2 - x1);
   		for (int x = x1, y = y1; x <= x2; x++){
      		//cout << "(" << x << "," << y << ")\n";
      		pixel(x, y, color);
 
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

	/*inline void bresenham_depth(int x1, int y1, int x2, int y2, uint32_t color, float depth){
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
	}*/


#ifdef __cplusplus
};
#endif