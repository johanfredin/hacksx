#ifndef PSX_CD_READER_H
#define PSX_CD_READER_H

#define CDR_SECTOR 0x800

#define DsClose() \
    logr_log(INFO, "LIBDS.h", "DsClose", "Mock DS closed")

int DsInit();

typedef struct CdrData {
    char *name;
    u_long *file;
    u_int sectors;
} CdrData;

#define CDR_INIT() {            \
    int ds_retval = DsInit();   \
    if(ds_retval != 1) {        \
        logr_log(INFO, "CdReader.h", "cdr_init", "LIBDS not initialized properly, return value from DsInit=%d, terminating...", ds_retval); \
        exit(1);                \
    }                           \
    logr_log(INFO, "CdReader.c", "cdr_init", "LIBDS initialized"); \
}

#define CDR_CLOSE() { \
    DsClose();        \
}

void cdr_close();

#define CDR_DATA_FREE(cdr_data) {           \
    MEM_FREE_3_AND_NULL((cdr_data)->file);  \
    MEM_FREE_3_AND_NULL(cdr_data);          \
}                                           \

CdrData *cdr_create_data_entry(char *name);
CdrData *cdr_find_data_entry(char *name, CdrData **assets, unsigned char assets_cnt);

/**
 * Read contents of the cd. CdOpen should be called prior to this and CdClose after
 * @param file_path location of file on cd
 * @return allocated CdrData struct
 */
CdrData *cdr_read_file(char *file_path);

#endif // PSX_CD_READER_H
