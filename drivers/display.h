#define VIDEO_ADDRESS 0xb8000
#define WHITE_ON_BLACK 0x0f
#define MAX_ROWS 25
#define MAX_COLS 80

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Public Kernel API */
void print_string(char *string);
void print_nl();
void clear_screen();
int scroll_ln(int offset);
void print_backspace();
void set_char_at_video_memory(char character, int offset);
