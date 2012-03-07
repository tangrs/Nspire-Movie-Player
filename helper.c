#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    FILE *f, *output;
    char *filename;
    int i;

    output = fopen("jpegmov.tns","wb");
    uint32_t end = 1;
    fwrite("NDLM", 1, 4, output);
    fwrite(&end, 1, 4, output);
    end = 15;
    fwrite(&end, 1, 4, output);

    for(i=1;;i++) {
        size_t fname_length;
        struct stat filestat;
        char buffer[16];

        fname_length = snprintf(NULL, 0, "frame-%05d.jpg",i);
        filename = malloc(fname_length+1);
        snprintf(filename, fname_length+1, "frame-%05d.jpg",i);
        f = fopen(filename, "rb+");
        printf("Opening %s\n",filename);
        if (!f) {
            free(filename);
            break;
        }

        if (stat(filename, &filestat) != 0) {
            free(filename);
            fclose(f);
            break;
        }

        void *buf = malloc(filestat.st_size);
        if (buf) {
            uint32_t length = filestat.st_size;
            fwrite(&length, 1, 4, output);

            fread(buf, 1, filestat.st_size, f);
            fwrite(buf, 1, filestat.st_size, output);

            free(buf);
        }

        free(filename);
        fclose(f);
    }


    return 0;
    err:
    fclose(f);
    printf("Error\n");
    return -1;

}