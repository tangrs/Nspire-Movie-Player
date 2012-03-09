#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

/* File reading */
inline uint32_t file_read_uint32(FILE*fp);
void header_read(FILE*fp);

/* Frame decoding */
void process_next_frame(FILE*fp);

/* Timers */
void setup_timer(int fps);
void restore_timer();
void tick_timer();

/* Globals */
extern int fps, init;

#define COMP_PER_PIXEL_CX 3
#define COMP_PER_PIXEL_GREY 1