#include <os.h>
#include "stb_image.h"

#define COMP_PER_PIXEL 3

static int swap = 0, fps = 0, init = 0;

static uint16_t convert_color(unsigned char (*ptr)[3]) {
    unsigned char r,g,b, *colors = (unsigned char*)ptr;
    uint16_t final = 0;
    r = colors[0];
    g = colors[1];
    b = colors[2];

    final |= (r>>3)<<11; //Divide by 8 then bitshift to end
    final |= (g>>2)<<5;  //Divide by 4 then bitshift
    final |= (b>>3);     //Divide by 8

    return final;
}

/*
static uint16_t read_uint16(FILE*fp) {
    uint16_t ret;
    fread(&ret, 2, 1, fp);
    if (swap) bswap16(ret);
    return ret;
}
*/

static uint32_t read_uint32(FILE*fp) {
    uint32_t ret;
    fread(&ret, 4, 1, fp);
    if (swap) bswap32(ret);
    return ret;
}

static void header_read(FILE*fp) {
    char magic[4];
    uint32_t endianness;
    rewind(fp);

    fread(&magic, 1, 4, fp);
    if (memcmp(magic, "NDLM", 4) != 0) return;

    fread(&endianness, 1, 4, fp);
    if (endianness != 1) swap = 1;

    fps = read_uint32(fp);
    init = 1;
}

static void process_frame(FILE*fp) {
    uint32_t next_size = read_uint32(fp);
    int x,y,n;
    void* mem = malloc(next_size);
    if (!mem) return;

    fread(mem, 1, next_size, fp);
    unsigned char (*data)[3] = (unsigned char (*)[3])stbi_load_from_memory(mem, next_size, &x, &y, &n, COMP_PER_PIXEL);
    if (data) {
        unsigned i = x*y;
        unsigned char (*ptr)[3] = data;
        uint16_t *framebuffer = SCREEN_BASE_ADDRESS;
        while (i && init) {
            *framebuffer = convert_color(ptr);
            ptr++;
            framebuffer++;
            i--;
        }
        stbi_image_free(data);
    }
    free(mem);
}

static unsigned timer_load, timer_saved_control, timer_saved_load;

static void setup_timer(int fps) {
    volatile unsigned *control = (unsigned*)0x900D0008;
	volatile unsigned *load = (unsigned*)0x900D0000;
	volatile unsigned *value = (unsigned*)0x900D0004;
    timer_saved_control = *control;
    timer_saved_load = *load;
    *control = 0; // disable timer
	*control = 0b01100011; // disabled, TimerMode N/A, int, no prescale, 32-bit, One Shot (for the *value > 0 test) -> 32khz
	*control = 0b11100011; // enable timer
	timer_load = 32000/fps;
}

static void restore_timer() {
    volatile unsigned *control = (unsigned*)0x900D0008;
	volatile unsigned *load = (unsigned*)0x900D0000;
	volatile unsigned *value = (unsigned*)0x900D0004;
    *control = 0; // disable timer
	*control = timer_saved_control & 0b01111111; // timer still disabled
	*load = timer_saved_load;
	*control = timer_saved_control; // enable timer
}

static void tick_timer() {
    volatile unsigned *control = (unsigned*)0x900D0008;
	volatile unsigned *load = (unsigned*)0x900D0000;
	volatile unsigned *value = (unsigned*)0x900D0004;
	*load = timer_load;
	while (*value > 0)
			idle();
}

int main() {
    FILE *fp = fopen("/documents/Examples/jpegmov.tns","rb");
    if (!fp) return 0;

    header_read(fp);
    setup_timer(fps);

    while (!isKeyPressed(KEY_NSPIRE_5)) {
        if (!feof(fp)) {
            process_frame(fp);
        }else{
            rewind(fp);
        }
        tick_timer();
    }

    restore_timer();
    fclose(fp);
    return 0;
}