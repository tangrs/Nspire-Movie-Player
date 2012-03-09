#include <os.h>
#include "inc.h"

static inline uint16_t convert_color_cx(uint8_t (*ptr)[3]) {
    uint8_t r,g,b, *colors = (uint8_t*)ptr;
    uint16_t final = 0;
    r = colors[0];
    g = colors[1];
    b = colors[2];

    final |= (r>>3)<<11; //Divide by 8 then bitshift to end
    final |= (g>>2)<<5;  //Divide by 4 then bitshift
    final |= (b>>3);     //Divide by 8

    return final;
}

static inline uint8_t convert_color_grey(uint8_t *grey) {
    return (*grey>>4); //Divide by 16
}

static void* fbuffer_ptr;

static inline void write_to_buffer_cx(uint8_t (*ptr)[3]) {
    *(uint16_t*)fbuffer_ptr = convert_color_cx(ptr);
    fbuffer_ptr = ((uint16_t*)fbuffer_ptr) + 1;
}

static inline void write_to_buffer_grey(uint8_t *grey) {
    static uint8_t nibble = 0;
    if (nibble) {
        *(uint8_t*)fbuffer_ptr |= convert_color_grey(grey);
        fbuffer_ptr = ((uint8_t*)fbuffer_ptr) + 1;
        nibble = 0;
    }else{
        *(uint8_t*)fbuffer_ptr = convert_color_grey(grey)<<4;
        nibble = 1;
    }
}

void process_next_frame(FILE*fp) {
    if (!init) return;
    uint32_t next_size = file_read_uint32(fp);
    int x,y,n;
    void* mem = malloc(next_size);
    if (!mem) return;
    fread(mem, 1, next_size, fp);

    void *data = stbi_load_from_memory(mem, next_size, &x, &y, &n, is_cx ? COMP_PER_PIXEL_CX : COMP_PER_PIXEL_GREY);
    if (data) {
        unsigned i = x*y;
        void* ptr = data;
        fbuffer_ptr = SCREEN_BASE_ADDRESS;

        if (is_cx) {
            while (i) {
                write_to_buffer_cx(ptr);
                ptr = ((char*)ptr) + COMP_PER_PIXEL_CX;
                i--;
            }
        }else{
            while (i) {
                write_to_buffer_grey(ptr);
                ptr = ((char*)ptr) + COMP_PER_PIXEL_GREY;
                i--;
            }
        }

        stbi_image_free(data);
    }
    free(mem);
}