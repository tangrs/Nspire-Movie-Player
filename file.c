#include <os.h>
#include "inc.h"

static int swap = 0;

inline uint32_t file_read_uint32(FILE*fp) {
    uint32_t ret;
    fread(&ret, 4, 1, fp);
    if (swap) bswap32(ret);
    return ret;
}

void header_read(FILE*fp) {
    char magic[4];
    uint32_t endianness;
    rewind(fp);

    fread(&magic, 1, 4, fp);
    if (memcmp(magic, "NDLM", 4) != 0) return;

    fread(&endianness, 1, 4, fp);
    if (endianness != 1) swap = 1;

    fps = file_read_uint32(fp);
    init = 1;
}