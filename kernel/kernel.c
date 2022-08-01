#include "../drivers/display.h"

#define VIDEO_MEM 0xb8000

void print_string_kernel(char *string){
	unsigned char *vidmem = (unsigned char*) VIDEO_MEM;
	int i=0;
	int offset=161;
	while(string[i]!=0){
		vidmem[offset] = string[i];
		vidmem[offset+1] = WHITE_ON_BLACK;
		i += 1;
		offset += 2;
	}
}

int main(){
	print_string_kernel("HELLO KERNEL!");	
}
