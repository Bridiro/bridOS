#include "../drivers/display.h"

#define VIDEO_MEM 0xb8000

int main(){
	unsigned char *vidmem = (unsigned char*) VIDEO_MEM;

	print_string("--Da kernel--");
}
