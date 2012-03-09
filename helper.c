#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    FILE *f, *output;
    char *filename;
    int i, verbose = 0, fps = 0;

    if (argc < 4) {
        printf(
            "Compile still frames into a nspire-movie-player file\n\n"
            "The infolder should contain files with this pattern frame-%%05d.jpg\n"
            "Output file should have extension .nmp.tns\n"
            "Usage: \n"
            "    %s infolder outfile fps [-v]\n\n",
                argv[0]
        );
        return 0;
    }

    fps = atoi(argv[3]);
    if (fps < 1) {
        printf("Invalid fps number\n");
        return -1;
    }

    if (argc > 4 && strncmp(argv[4],"-v", 2) == 0) {
        verbose = 1;
    }

    output = fopen(argv[2],"wb");
    if (!output) {
        printf("Cannot open output file!\n");
        return -1;
    }
    uint32_t end = 1;
    fwrite("NDLM", 1, 4, output);
    if (verbose) printf("Wrote magic number\n");
    fwrite(&end, 1, 4, output);
    if (verbose) printf("Wrote endian identifier\n");
    fwrite(&fps, 1, 4, output);
    if (verbose) printf("Wrote fps number %d\n",fps);

    for(i=1;;i++) {
        size_t fname_length;
        struct stat filestat;
        char buffer[16];

        fname_length = snprintf(NULL, 0, "%s/frame-%05d.jpg",argv[1],i);
        filename = malloc(fname_length+1);
        snprintf(filename, fname_length+1, "%s/frame-%05d.jpg",argv[1],i);
        f = fopen(filename, "rb+");
        if (verbose) printf("Opening %s\n",filename);
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
    if (verbose) printf("Wrote %d frames\n",i-1);
    fclose(output);
    return 0;
    err:
    fclose(f);
    printf("Error\n");
    return -1;

}