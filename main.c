#include <os.h>
#include "inc.h"

int fps = 0, init = 0;

int main() {
    FILE *fp = fopen("/documents/Examples/jpegmov.tns","rb");
    if (!fp) return 0;

    header_read(fp);
    setup_timer(fps);

    while (!isKeyPressed(KEY_NSPIRE_5)) {
        if (!feof(fp)) {
            process_next_frame(fp);
        }else{
            rewind(fp);
        }
        tick_timer();
    }

    restore_timer();
    fclose(fp);
    return 0;
}