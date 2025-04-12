#include <pspkernel.h>

static int exit_request = 0;

int is_running(){
	return !exit_request;
}

int exit_callback(int arg1, int arg2, void* common){
	exit_request = 1;
	return 0;
}

int callback_thread(SceSize args, void* argp){
	int callback_id;

	callback_id = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(callback_id);

	sceKernelSleepThreadCB();

	return 0;
}

int setup_exit_callback(){
	int thread_id = 0;

	thread_id = sceKernelCreateThread("Callback Update Thread", callback_thread, 0x11, 0xfa0, THREAD_ATTR_USER, 0);

	if(thread_id >= 0){
		sceKernelStartThread(thread_id, 0, 0);
	}

	return thread_id;
}