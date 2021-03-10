#include <kinc/pch.h>


void update(){

}

int kickstart(int argc, char** argv) {
	kinc_init("Ironc", 1024, 768, NULL, NULL);
	kinc_set_update_callback(update);

	kinc_start();
	return 0;
}
