#include "../drivers/display.h"

#define VIDEO_MEM 0xb8000

void set_char_at_video_memory_k(char character, int offset){
	unsigned char *vidmem = (unsigned char *) VIDEO_MEM;
	vidmem[offset] = character;
	vidmem[offset+1] = WHITE_ON_BLACK;
}

int main(){
	unsigned char *vidmem = (unsigned char*) VIDEO_MEM;

	set_char_at_video_memory_k('X', 161);
}
