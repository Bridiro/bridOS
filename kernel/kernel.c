#define VIDEO_MEM 0xb8000

int main(){
	volatile unsigned char *vid = (unsigned char*) VIDEO_MEM;

	vid[161]='X';
	vid[162]=0x0f;
}
