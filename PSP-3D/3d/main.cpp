extern "C"{
	#include "../common/callback.h"
}
#include "../common/graphics.h"
#include <unistd.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include "3d.h"
#include "3d.cpp"
#include <vector>
//#include <pspdebug.h>

extern inline void swap_buffers() noexcept;
extern inline void clear(uint32_t) noexcept;

#define VERS 1
#define REVS 0

PSP_MODULE_INFO("3D exp", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_MAX();


d3::vec_3d camera = {0.0f, 0.0f, 0.0f, 1.0f};
d3::vec_3d look_direction ;

float f_yaw;


int main(void){ 
	setup_exit_callback();

	disp::init();

	disp::init_depth_buffer();

	//pspDebugScreenInitEx(NULL, 1, 1);
	//pspDebugScreenEnableBackColor(1);

	long long int time_mem = 0;

	int running = is_running();

	d3::mesh cube;
	
	float f_theta;

	cube.load_obj_file("ms0:/PSP/GAME/Hlwd/models/test.obj");

	/*float f_near = 0.1f;
	float f_far = 1000.0f;
	float f_fov = 90.0f;
	float f_aspect_ratio = (float)(272) / (float)(480);
	float f_fov_rad = 1.0f / tanf(f_fov * 0.5f / 180.0f * 3.14159f);*/

	d3::proj_matrix matr_trans;

	d3::proj_matrix matrot_z, matrot_x;
	d3::proj_matrix matr_world, matr;

	matr = d3::matrix_projection(90.0f, 272.0f / 480.0f, 0.1f, 1000.0f);

	SceCtrlData button_input;
	
	//int cntr = 0;

	time_mem = current_timestamp();

	for(;;){
		sceCtrlPeekBufferPositive(&button_input, 1);

		if(button_input.Buttons != 0){
			if(button_input.Buttons & PSP_CTRL_START){
				running = 0;
				break;
			}
			if(button_input.Buttons & PSP_CTRL_UP){
				camera.y = camera.y - 0.000025f;
			}
			if(button_input.Buttons & PSP_CTRL_DOWN){
				camera.y = camera.y + 0.000025f;
			}
			if(button_input.Buttons & PSP_CTRL_RIGHT){
				camera.x = camera.x + 0.000025f;	
			}
			if(button_input.Buttons & PSP_CTRL_LEFT){
				camera.x = camera.x - 0.000025f;	
			}
			if(button_input.Buttons & PSP_CTRL_LTRIGGER){
				f_yaw += 0.00005f;
			}
			if(button_input.Buttons & PSP_CTRL_RTRIGGER){
				f_yaw -= 0.00005f;
			}
			if(button_input.Buttons & PSP_CTRL_TRIANGLE){
				d3::vec_3d v_forward = d3::vector_mul(look_direction, 0.0002f);
				camera = d3::vector_add(camera, v_forward);
			}
			if(button_input.Buttons & PSP_CTRL_CROSS){
				d3::vec_3d v_forward = d3::vector_mul(look_direction, 0.0002f);
				camera = d3::vector_sub(camera, v_forward);
			}
		}

		if(current_timestamp() - time_mem >= 16){
			//f_theta = cntr * 0.05f;
			//cntr++;

			// Rotation Z

			matrot_z = d3::matrix_make_rotation_z(f_theta);

			// Rotation X
			matrot_x = d3::matrix_make_rotation_x(f_theta * 0.5f);

			matr_trans = d3::matrix_make_translation(0.0f, 0.0f, 50.0f);

			matr_world = d3::matrix_make_identity();
			matr_world = d3::multiply_matrix_matr(matrot_z, matrot_x);
			matr_world = d3::multiply_matrix_matr(matr_world, matr_trans);

			//look_direction = {0, 0, 1};
			d3::vec_3d v_up = {0, 1, 0};
			//d3::vec_3d v_target = d3::vector_add(camera, look_direction);
			d3::vec_3d v_target = {0, 0, 1};
			d3::proj_matrix matr_camera_rot = d3::matrix_make_rotation_y(f_yaw);
			look_direction = d3::multiply_matrix_vect(matr_camera_rot, v_target);
			v_target = d3::vector_add(camera, look_direction);


			d3::proj_matrix matr_camera = d3::matrix_point_at(camera, v_target, v_up);
			d3::proj_matrix matr_view = d3::matrix_quick_inverse(matr_camera);


			disp::clear(0xffffffff);

			d3::draw_mesh(cube, 0x00ffa500, matr, matr_world, matr_view);

			disp::swap_buffers();
			sceDisplayWaitVblank();

			time_mem = current_timestamp();

			/*if(cntr >= 7200){
				cntr = 0;
			}*/
		}
	}
	

	sceKernelExitGame();
	return 0;
}