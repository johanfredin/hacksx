#ifndef HACKSX_LIBSPU_H
#define HACKSX_LIBSPU_H

#define SPU_00CH 1
#define SPU_01CH 2

#define SpuSetKey(key, channel) \
    logr_log(INFO, "LIBSPU.h", "SpuSetKey", "Mock SPU key set")

#define SpuFree(spu_address) \
    logr_log(INFO, "LIBSPU.h", "SpuFree", "Mock SPU free")

#endif //HACKSX_LIBSPU_H
