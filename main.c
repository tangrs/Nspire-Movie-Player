#include <os.h>
#include "inc.h"

int fps = 0, init = 0;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        if (config_file_already_written()) {
            show_msgbox(
                "Nspire Movie Player",
                "The program plays .nmp.tns movies.\n"
                "To play a video, simply open a movie file.\n");
        }else{
            int index = show_msgbox_2b(
                "Nspire Movie Player",
                "The program plays .nmp.tns movies.\n"
                "To play a video, simply open a movie file.\n"
                "Note: You have not added the correct line to "
                "your ndless.cfg.tns file. Select the option "
                "below if you want me to do it for you.",
                    "Do it",
                    "No thank you");

            switch (index) {
                case 1:
                    write_config_file();
                    show_msgbox("Nspire Movie Player",
                        "Done. You may now open movie files");
                    break;
                case 2:
                default:
                    show_msgbox("Nspire Movie Player",
                        "No problems. Remember, you can't use this until "
                        "you add the appropriate line in your ndless.cfg.tns file");
                    break;
            }
        }
        return 0;
    }
    FILE *fp = fopen(argv[1],"rb");
    if (!fp) return 0;

    header_read(fp);
    setup_timer(fps);

    while (!isKeyPressed(KEY_NSPIRE_5)) {
        if (!feof(fp)) {
            process_next_frame(fp);
        }else{
            movie_rewind(fp);
        }
        tick_timer();
    }

    restore_timer();
    fclose(fp);
    return 0;
}