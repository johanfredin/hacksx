#ifndef PSX_CD_READER_H
#define PSX_CD_READER_H

#define CDR_SECTOR 2048

typedef struct CdrData {
    char *name;
    unsigned long *file;
} CdrData;

void cdr_open();
void cdr_close();
void cdr_data_free(CdrData *cdr_data);

CdrData *cdr_create_data_entry(char *name);
CdrData *cdr_find_data_entry(char *name, CdrData **assets, unsigned char assets_cnt);

/**
 * Read contents of the cd. CdOpen should be called prior to this and CdClose after
 * @param file_path location of file on cd
 * @return allocated CdrData struct
 */
CdrData *cdr_read_file(char *file_path);

/**
 * Read content of one single asset from the cd calling #CdOpen before and #CdClose after
 * @param file_path location of file on cd
 * @return allocated CdrData struct
 */
CdrData *cdr_read_single_file(char *file_path);

#endif // PSX_CD_READER_H
